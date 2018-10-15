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

Model makeModel(const Options& opts, entry::VectorizedEntriesMap&& entries) {
    switch (opts.method) {
        case KNN:
            return (Model)ModelKNN(move(entries), opts.k);
        case PCAKNN:
            return (Model)ModelPCA(move(entries), opts.k, opts.alpha);
        default:
            (throw std::runtime_error("Invalid method!"));
    }
}

int main(int argc, char* argv[]) {
    const string cmd = argv[0];
    Options options;

    if (not parseArguments(argc, argv, defaultOptions, options)) {
        printHelp(cmd, defaultOptions); return -1;
    }
    debugging_enabled = (bool)options.debug;

    /*************** Read the entries ********************/
    entry::FrecuencyVocabularyMap vocabulary;
    entry::VectorizedEntriesMap trainEntries;
    entry::VectorizedEntriesMap testEntries;

    auto vocabFile = Input(options.vocabFilename);
    vocabulary = entry::read_vocabulary(
        vocabFile,
        entry::filterPassBand(options.minVocabFreq, options.maxVocabFreq));
    vocabFile.close();

    entry::TokenizedEntriesMap trainTokenized;
    entry::TokenizedEntriesMap testTokenized;
    if (options.trainFilename == options.testFilename) {
        auto file = Input(options.testFilename);
        entry::read_entries(file, trainTokenized, testTokenized);
        file.close();
    } else {
        auto trainFile = Input(options.trainFilename);
        auto testFile = Input(options.testFilename);
        entry::read_entries(trainFile, trainTokenized);
        entry::read_entries(testFile, testTokenized);
        testFile.close();
        trainFile.close();
    }

    trainEntries = entry::vectorizeMap(vocabulary, trainTokenized);
    testEntries = entry::vectorizeMap(vocabulary, testTokenized);

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    DEBUG("    Train entries count: " << trainEntries.size());
    DEBUG("    Test entries count: " << testEntries.size());

    /*************** Train the model ********************/
    DEBUG("---------------- Training ----------------");
    Model model = makeModel(options, move(trainEntries));

    /*************** Test the model ********************/
    DEBUG("---------------- Testing ----------------");
    auto outFile = Output(options.outFilename);

    outFile.close();

    return 0;
}
