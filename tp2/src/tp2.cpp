#include <string.h>
#include <iostream>

#include "arguments.h"
#include "debug.h"
#include "entry/reader.h"
#include "entry/vector_builder.h"
#include "files.h"
#include "model.h"

using namespace std;

const Options defaultOptions = {
    trainFilename : "data/imdb_tokenized.csv",
    testFilename : "data/imdb_tokenized.csv",
    outFilename : "-",
    cacheFilename : "",
    vocabFilename : "data/vocab.csv",
    minVocabFreq : 0.01,
    maxVocabFreq : 0.99,

    method : PCAKNN,
    k : 3,
    alpha : 10,

    // Print all the info by default (TODO: remove this later)
    debug : true,
    dontTest : false,
};

void readEntries(const Options& opts, entry::FrecuencyVocabularyMap& vocabulary,
                 entry::VectorizedEntriesMap& trainEntries,
                 entry::VectorizedEntriesMap& testEntries) {
    auto vocabFile = Input(opts.vocabFilename);
    vocabulary = entry::read_vocabulary(
        vocabFile, entry::filterPassBand(opts.minVocabFreq, opts.maxVocabFreq));
    vocabFile.close();

    entry::TokenizedEntriesMap trainTokenized;
    entry::TokenizedEntriesMap testTokenized;
    if (opts.trainFilename == opts.testFilename) {
        auto file = Input(opts.testFilename);
        entry::read_entries(file, trainTokenized, testTokenized);
        file.close();
    } else {
        auto trainFile = Input(opts.trainFilename);
        auto testFile = Input(opts.testFilename);
        entry::read_entries(trainFile, trainTokenized);
        entry::read_entries(testFile, testTokenized);
        testFile.close();
        trainFile.close();
    }

    trainEntries = entry::vectorizeMap(vocabulary, trainTokenized);
    testEntries = entry::vectorizeMap(vocabulary, testTokenized);
}

const Model* makeModel(const Options& opts, entry::VectorizedEntriesMap&& entries) {
    switch (opts.method) {
        case KNN:
            return new ModelKNN(move(entries), opts.k);
        case PCAKNN:
            return new ModelPCA(move(entries), opts.k, opts.alpha);
        default:
            (throw std::runtime_error("Invalid method!"));
    }
}

void testModel(const Options& opts, const Model* model,
               const entry::VectorizedEntriesMap& testEntries) {
    int total = 0;
    int trueP = 0;
    int falseP = 0;
    int trueN = 0;
    int falseN = 0;

    for (const auto& test : testEntries) {
        const entry::VectorizedEntry& entry = test.second;

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

        // TODO: Print each test result to a "classifications" file

        if(total > 8) break;
    }

    // TODO: Output statistics about the model instead of this
    auto outFile = Output(opts.outFilename);
    auto& outStream = outFile.stream();
    outStream << "total: " << total << endl;
    outStream << "trueP: " << trueP << endl;
    outStream << "falseP: " << falseP << endl;
    outStream << "trueN: " << trueN << endl;
    outStream << "falseN: " << falseN << endl;
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

    entry::VectorizedEntriesMap trainEntries;
    entry::VectorizedEntriesMap testEntries;
    entry::FrecuencyVocabularyMap vocabulary;

    readEntries(options, vocabulary, trainEntries, testEntries);

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    DEBUG("    Train entries count: " << trainEntries.size());
    DEBUG("    Test entries count: " << testEntries.size());

    /*************** Train the model ********************/
    DEBUG("---------------- Training ----------------");
    const Model* model = makeModel(options, move(trainEntries));

    /*************** Test the model ********************/
    DEBUG("---------------- Testing -----------------");
    if (not options.dontTest) {
        testModel(options, model, testEntries);
    }


    delete model;

    return 0;
}
