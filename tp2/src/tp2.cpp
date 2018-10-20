#include <string.h>
#include <iostream>

#include "arguments.h"
#include "debug.h"
#include "entry/reader.h"
#include "entry/vector_builder.h"
#include "files.h"
#include "model/model.h"

using namespace std;

const Options defaultOptions = {
    trainFilename : "data/imdb_tokenized.csv",
    testFilename : "data/imdb_tokenized.csv",
    outFilename : "-",
    cacheFilename : "",
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
};

bool readEntries(const Options& opts, entry::Vocabulary& vocabulary,
                 entry::SpEntries& trainEntries,
                 entry::SpEntries& testEntries) {
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
    if (opts.trainFilename == opts.testFilename) {
        auto file = Input(opts.testFilename);
        if (file.fail()) {
            cerr << "Could not open the test/training file" << endl;
            return false;
        }
        entry::read_entries(file.stream(), trainTokenized, testTokenized);
        file.close();
    } else {
        auto trainFile = Input(opts.trainFilename);
        if (trainFile.fail()) {
            cerr << "Could not open the training file" << endl;
            return false;
        }
        entry::read_entries(trainFile.stream(), trainTokenized);
        trainFile.close();

        auto testFile = Input(opts.testFilename);
        if (testFile.fail()) {
            cerr << "Could not open the testing file" << endl;
            return false;
        }
        entry::read_entries(testFile.stream(), testTokenized);
        testFile.close();
    }

    trainEntries = entry::vectorize(vocabulary, trainTokenized);
    if (opts.maxTrainEntries > 0)
        trainEntries.resize(
            min(trainEntries.size(), (size_t)opts.maxTrainEntries));
    testEntries = entry::vectorize(vocabulary, testTokenized);
    return true;
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
    int total = 0;
    int trueP = 0;
    int falseP = 0;
    int trueN = 0;
    int falseN = 0;

    auto classifFile = Output(opts.classifFilename);
    for (const auto& entry : testEntries) {
        bool expected = entry.is_positive;
        bool result = model->analize(entry);

        total++;
        if (expected and result)
            trueP++;
        else if (not expected and not result)
            trueN++;
        else if (not expected and result)
            falseP++;
        else if (expected and not result)
            falseN++;

        // Print each test result to a "classifications" file
        classifFile.stream() << (int)result << endl;

        if (opts.maxTestEntries > 0 and total >= opts.maxTestEntries)
            break;
    }
    classifFile.close();

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

    if (not parseArguments(argc, argv, defaultOptions, options)) {
        printHelp(cmd, defaultOptions);
        return -1;
    }
    debugging_enabled = (bool)options.debug;

    /*************** Read the entries ********************/
    DEBUG("---------------- Loading data ------------");

    entry::SpEntries trainEntries;
    entry::SpEntries testEntries;
    entry::Vocabulary vocabulary;

    bool res = readEntries(options, vocabulary, trainEntries, testEntries);
    if (not res)
        return -2;

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    DEBUG("    Train entries count: " << trainEntries.size());
    DEBUG("    Test entries count: " << testEntries.size());

    /*************** Train the model ********************/
    DEBUG("---------------- Training ----------------");
    const Model<SparseVector>* model = makeModel(options, move(trainEntries));

    /*************** Test the model ********************/
    if (not options.dontTest) {
        DEBUG("---------------- Testing -----------------");
        testModel(options, model, testEntries);
    }

    delete model;

    return 0;
}
