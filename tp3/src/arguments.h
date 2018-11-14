#pragma once

#include <string>

// TODO: Define these somewhere
extern enum RayGenerator;
extern enum ErrorGenerator;
extern enum LSQMethod; // SVD - QR - Something else

struct Options {
    // Image input and output
    std::string inputFilename;
    std::string outputFilename;
    std::string cachePath;

    // Rays
    RayGenerator rayGenerator;
    int rayCount;
    int cellsPerRow;

    // Measurement error
    ErrorGenerator errorGenerator;
    double errorSigma;

    // LSQ solver
    LSQMethod lsqMethod;

    int debug;
    int nThreads;
};

void printHelp(const std::string& cmd, const Options& defaultOptions);

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& out);

// Get a cache key from the model training options
size_t trainingCacheKey(const Options&);

// Derive the filename from the cachePath and a computed key
std::string cacheFilename(const Options&);
