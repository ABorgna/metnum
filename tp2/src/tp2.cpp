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
    vocabFilename : "data/vocab.csv",
    minVocabFreq : 0.01,
    maxVocabFreq : 0.99,

    maxTestEntries : -1,

    method : PCAKNN,
    k : 3,
    alpha : 10,

    // Print all the info by default (TODO: remove this later)
    debug : true,
    dontTest : false,
};

void readEntries(const Options& opts, entry::Vocabulary& vocabulary,
                 entry::Entries& trainEntries, entry::Entries& testEntries) {
    auto vocabFile = Input(opts.vocabFilename);
    vocabulary = entry::read_vocabulary(
        vocabFile, entry::filterPassBand(opts.minVocabFreq, opts.maxVocabFreq));
    vocabFile.close();

    entry::TokenizedEntries trainTokenized;
    entry::TokenizedEntries testTokenized;
    if (opts.trainFilename == opts.testFilename) {
        auto file = Input(opts.testFilename);
        entry::read_entries(file, trainTokenized, testTokenized);
        file.close();
    } else {
        auto trainFile = Input(opts.trainFilename);
        entry::read_entries(trainFile, trainTokenized);
        trainFile.close();

        auto testFile = Input(opts.testFilename);
        entry::read_entries(testFile, testTokenized);
        testFile.close();
    }

    trainEntries = entry::vectorize(vocabulary, trainTokenized);
    testEntries = entry::vectorize(vocabulary, testTokenized);
}

const Model* makeModel(const Options& opts, entry::Entries&& entries) {
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
               const entry::Entries& testEntries) {
    int total = 0;
    int trueP = 0;
    int falseP = 0;
    int trueN = 0;
    int falseN = 0;

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

        // TODO: Print each test result to a "classifications" file

        if (total >= opts.maxTestEntries)
            break;
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

    if (not parseArguments(argc, argv, defaultOptions, options)) {
        printHelp(cmd, defaultOptions);
        return -1;
    }
    debugging_enabled = (bool)options.debug;

    /*************** Read the entries ********************/
    DEBUG("---------------- Loading data ------------");

    entry::Entries trainEntries;
    entry::Entries testEntries;
    entry::Vocabulary vocabulary;

    readEntries(options, vocabulary, trainEntries, testEntries);

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    DEBUG("    Train entries count: " << trainEntries.size());
    DEBUG("    Test entries count: " << testEntries.size());

    /*************** Train the model ********************/
    DEBUG("---------------- Training ----------------");
    const Model* model = makeModel(options, move(trainEntries));

    /*************** Test the model ********************/
    if (not options.dontTest) {
        DEBUG("---------------- Testing -----------------");
        testModel(options, model, testEntries);
    }

    delete model;

    return 0;
}
