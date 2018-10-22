#include <string.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <queue>
#include <thread>

#include "arguments.h"
#include "debug.h"
#include "entry/reader.h"
#include "entry/vector_builder.h"
#include "files.h"
#include "model/model.h"

using namespace std;

const size_t SEED = 42;
// Used to invalidate the models cache
const size_t VERSION = 1;

const Options defaultOptions = {
    trainFilename : "data/imdb_tokenized.csv",
    testFilename : "data/imdb_tokenized.csv",
    outFilename : "-",
    cachePath : "cache",
    classifFilename : "",
    vocabFilename : "data/vocab.csv",
    minVocabFreq : 0.01,
    maxVocabFreq : 0.99,

    maxTrainEntries : -1,
    maxTestEntries : -1,

    method : PCAKNN,
    k : 3,
    alpha : 10,

    // Print all the info by default (TODO: set this to false later?)
    debug : true,
    dontTest : false,
    nThreads : -1,
};

bool readEntries(const Options& opts, entry::Vocabulary& vocabulary,
                 entry::SpEntries& trainEntries, entry::SpEntries& testEntries,
                 bool readTraining, bool readTest) {
    auto vocabFile = Input(opts.vocabFilename);
    if (vocabFile.fail()) {
        cerr << "Could not open the vocabulary file" << endl;
        return false;
    }
    vocabulary = entry::read_vocabulary(
        vocabFile.stream(),
        entry::filterPassBand(opts.minVocabFreq, opts.maxVocabFreq));
    vocabFile.close();

    entry::TokenizedEntries trainTokenized;
    entry::TokenizedEntries testTokenized;
    if (readTraining and readTest and opts.trainFilename == opts.testFilename) {
        auto file = Input(opts.testFilename);
        if (file.fail()) {
            cerr << "Could not open the test/training file" << endl;
            return false;
        }
        entry::read_entries(file.stream(), trainTokenized, testTokenized);
        file.close();
    } else {
        if (readTraining) {
            auto trainFile = Input(opts.trainFilename);
            if (trainFile.fail()) {
                cerr << "Could not open the training file" << endl;
                return false;
            }
            entry::read_entries(trainFile.stream(), trainTokenized,
                                entry::ENTRY_TRAIN);
            trainFile.close();
        }

        if (readTest) {
            auto testFile = Input(opts.testFilename);
            if (testFile.fail()) {
                cerr << "Could not open the testing file" << endl;
                return false;
            }
            entry::read_entries(testFile.stream(), testTokenized,
                                entry::ENTRY_TEST);
            testFile.close();
        }
    }

    if (readTraining) {
        trainEntries = entry::vectorize(vocabulary, trainTokenized);
        std::srand(SEED);
        std::random_shuffle(trainEntries.begin(), trainEntries.end());
        if (opts.maxTrainEntries > 0)
            trainEntries.resize(
                min(trainEntries.size(), (size_t)opts.maxTrainEntries));
    }
    if (readTest) {
        testEntries = entry::vectorize(vocabulary, testTokenized);
        std::srand(SEED);
        std::random_shuffle(testEntries.begin(), testEntries.end());
        if (opts.maxTestEntries > 0)
            testEntries.resize(
                min(testEntries.size(), (size_t)opts.maxTestEntries));
    }
    return true;
}

bool fromCache(const Options& opts, const Model<SparseVector>*& model) {
    if (opts.cachePath == "")
        return false;
    auto cacheFile = Input(cacheFilename(opts));
    if (cacheFile.fail()) {
        DEBUG("No cached model found");
        return false;
    }
    auto& stream = cacheFile.stream();

    // Check the caché version
    size_t version;
    stream >> version;
    if (version < VERSION) {
        DEBUG("Old cache version "
              << version << ",ignoring. (Current: " << VERSION << ")");
        return false;
    }
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Drop the first line (the calling command)
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool res = false;
    try {
        switch (opts.method) {
            case KNN:
                model = new ModelKNN(stream, opts.k);
                res = true;
                break;
            case KNN_INVERTED:
                model = new ModelKNNInv(stream, opts.k);
                res = true;
                break;
            case PCAKNN:
                model =
                    new ModelPCA<ModelKNNtmp<Vector, Vector>>(stream, opts.k);
                res = true;
                break;
            case PCAKNN_INVERTED:
                model = new ModelPCA<ModelKNNInvtmp<Vector, Vector>>(stream,
                                                                     opts.k);
                res = true;
                break;
            default:
                break;
        }
    } catch (const std::invalid_argument&) {
        DEBUG("Could not parse the cached model.");
    }
    cacheFile.close();
    return res;
}

const Model<SparseVector>* makeModel(const Options& opts,
                                     entry::SpEntries&& entries) {
    switch (opts.method) {
        case KNN:
            return new ModelKNN(move(entries), opts.k);
        case KNN_INVERTED:
            return new ModelKNNInv(move(entries), opts.k);
        case PCAKNN:
            return new ModelPCA<ModelKNNtmp<Vector, Vector>>(
                move(entries), opts.k, opts.alpha);
        case PCAKNN_INVERTED:
            return new ModelPCA<ModelKNNInvtmp<Vector, Vector>>(
                move(entries), opts.k, opts.alpha);
        default:
            (throw std::runtime_error("Invalid method!"));
    }
}

void testModel(const Options& opts, const Model<SparseVector>* model,
               const entry::SpEntries& testEntries) {
    struct Stats {
        int total;
        int trueP;
        int falseP;
        int trueN;
        int falseN;
    };

    const bool writeClassif = opts.classifFilename != "";

    // Number of threads to utilize
    size_t nThreads;
    if (opts.nThreads > 0) {
        nThreads = opts.nThreads;
    } else {
        unsigned concurentThreadsSupported =
            std::thread::hardware_concurrency();
        nThreads = std::max(concurentThreadsSupported, (unsigned)2);
    }
    DEBUG("Running on " << nThreads << " threads.");

    auto analizeSome = [&testEntries, &model, writeClassif](
                           size_t from, size_t to, Stats* outStats,
                           std::queue<bool>* classifications) {
        Stats s{0, 0, 0, 0, 0};
        // Get the nearest k polarities
        for (size_t i = from; i < to; i++) {
            const auto& entry = testEntries[i];
            bool expected = entry.is_positive;
            bool result = model->analize(entry);

            if (writeClassif)
                classifications->push(result);

            s.total++;
            if (expected and result)
                s.trueP++;
            else if (not expected and not result)
                s.trueN++;
            else if (not expected and result)
                s.falseP++;
            else if (expected and not result)
                s.falseN++;
        }
        *outStats = s;
    };

    // Analyze the entries in multiple threads
    const size_t step = ((testEntries.size() - 1) / nThreads) + 1;
    std::vector<Stats> threadStats = std::vector<Stats>(nThreads);
    std::vector<std::queue<bool>> threadClassif =
        std::vector<std::queue<bool>>(nThreads);
    std::vector<std::thread> threads;
    for (size_t i = 0; i < nThreads; i++) {
        size_t from = step * i;
        size_t to = min(step * (i + 1), testEntries.size());
        std::thread t(analizeSome, from, to, &threadStats[i],
                      &threadClassif[i]);
        threads.push_back(std::move(t));
    }

    // Wait for everyone
    for (auto& t : threads) t.join();

    // Merge the statistics
    int total = 0;
    int trueP = 0;
    int falseP = 0;
    int trueN = 0;
    int falseN = 0;
    for (const auto& s : threadStats) {
        total += s.total;
        trueP += s.trueP;
        falseP += s.falseP;
        trueN += s.trueN;
        falseN += s.falseN;
    }

    // Print each test result to a "classifications" file
    if (writeClassif) {
        auto classifFile = Output(opts.classifFilename);
        for (auto& q : threadClassif) {
            while (not q.empty()) {
                classifFile.stream() << (int)q.front() << endl;
                q.pop();
            }
        }
        classifFile.close();
    }

    const double accuracy = (double)trueP / (trueP + falseP);
    const double recall = (double)trueP / (trueP + falseN);

    // TODO: Output statistics about the model instead of this
    auto outFile = Output(opts.outFilename);
    auto& outStream = outFile.stream();
    outStream << "method: " << showMethod(opts.method) << endl;
    outStream << "k: " << opts.k << endl;
    if (opts.method == PCAKNN)
        outStream << "alpha: " << opts.alpha << endl;
    outStream << "countEntries: " << total << endl;
    outStream << "trueP: " << trueP << endl;
    outStream << "falseP: " << falseP << endl;
    outStream << "trueN: " << trueN << endl;
    outStream << "falseN: " << falseN << endl;
    outStream << "accuracy: " << accuracy << endl;
    outStream << "recall: " << recall << endl;
    outFile.close();
}

int main(int argc, char* argv[]) {
    const string cmd = argv[0];
    Options options;
    string fullCmd = string(argv[0]);
    for (int i = 1; i < argc; i++) fullCmd += " " + string(argv[i]);

    if (not parseArguments(argc, argv, defaultOptions, options)) {
        printHelp(cmd, defaultOptions);
        return -1;
    }
    debugging_enabled = (bool)options.debug;

    /*************** Read a cached version of the model ********************/
    const Model<SparseVector>* model;
    bool validCache = false;
    if (options.cachePath != "") {
        DEBUG("---------------- Reading model cache ------------");
        DEBUG("Loading from " << cacheFilename(options));
        validCache = fromCache(options, model);
    }

    /*************** Read the entries ********************/
    DEBUG("---------------- Loading data ------------");

    entry::SpEntries trainEntries;
    entry::SpEntries testEntries;
    entry::Vocabulary vocabulary;

    const bool doTrain = not validCache;
    const bool doTest = not options.dontTest;
    bool res = readEntries(options, vocabulary, trainEntries, testEntries,
                           doTrain, doTest);
    if (not res)
        return -2;

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    if (doTrain) {
        DEBUG("    Train entries count: " << trainEntries.size());
        DEBUG("        Positive train entries: " << std::count_if(
                  trainEntries.begin(), trainEntries.end(),
                  [](const entry::SpEntry& e) { return e.is_positive; }));
    }
    if (doTest) {
        DEBUG("    Test entries count: " << testEntries.size());
        DEBUG("        Positive test entries: " << std::count_if(
                  testEntries.begin(), testEntries.end(),
                  [](const entry::SpEntry& e) { return e.is_positive; }));
    }

    if (doTrain) {
        /*************** Train the model ********************/
        DEBUG("---------------- Training ----------------");
        model = makeModel(options, move(trainEntries));

        if (options.cachePath != "") {
            DEBUG("---------------- Storing cache ------------");
            auto cacheFile = Output(cacheFilename(options));
            if (cacheFile.fail()) {
                DEBUG("Could not open the cache file: "
                      << cacheFilename(options));
            }
            cacheFile.stream() << VERSION << endl;
            cacheFile.stream() << fullCmd << endl;
            model->saveCache(cacheFile.stream());
            cacheFile.close();
            DEBUG("Stored in " << cacheFilename(options));
        }
    }

    /*************** Test the model ********************/
    if (doTest) {
        DEBUG("---------------- Testing -----------------");
        testModel(options, model, testEntries);
    }

    delete model;

    return 0;
}
