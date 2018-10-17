#pragma once

#include <string>

enum Method {
    KNN,
    PCAKNN,

    // Add new methods before this entry
    METHODS_COUNT
};

std::string showMethod(Method);

struct Options {
    std::string trainFilename;
    std::string testFilename;
    std::string outFilename;
    std::string cacheFilename;

    std::string vocabFilename;
    double minVocabFreq;
    double maxVocabFreq;

    Method method;
    int k;      // kNN hyper-parameter
    int alpha;  // PCA hyper-parameter

    int debug;
    int dontTest;
};

void printHelp(const std::string& cmd, const Options& defaultOptions);

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& out);
