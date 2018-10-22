#pragma once

#include <string>

enum Method {
    KNN,
    KNN_INVERTED,
    PCAKNN,
    PCAKNN_INVERTED,

    // Add new methods before this entry
    METHODS_COUNT
};

std::string showMethod(Method);

struct Options {
    std::string trainFilename;
    std::string testFilename;
    std::string outFilename;
    std::string cachePath;
    std::string classifFilename;

    std::string vocabFilename;
    double minVocabFreq;
    double maxVocabFreq;

    int maxTrainEntries;
    int maxTestEntries;

    Method method;
    int k;      // kNN hyper-parameter
    int alpha;  // PCA hyper-parameter

    int debug;
    int dontTest;
    int nThreads;
};

void printHelp(const std::string& cmd, const Options& defaultOptions);

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& out);

// Get a cache key from the model training options
size_t trainingCacheKey(const Options&);

// Derive the filename from the cachePath and a computed key
std::string cacheFilename(const Options&);
