#pragma once

#include <string>
#include "model/ray.h"
#include "model/error.h"
#include "model/lsq.h"

struct Options {
    // Image input and output
    std::string inputFilename;
    std::string outputFilename;
    std::string cachePath;
    std::string raysOutFilename;

    // Rays
    RayGenerator rayGenerator;
    size_t rayCount;
    size_t cellsPerRow;

    // Measurement error
    ErrorGenerator errorGenerator;
    double errorSigma;

    // LSQ solver
    LSQMethod lsqMethod;
    double alpha; // Variable de compresión del SVD

    int debug;
    int nThreads;
    size_t seed;
};

void printHelp(const std::string& cmd, const Options& defaultOptions);

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& out);

// Get a cache key from the model training options
size_t trainingCacheKey(const Options&);

// Derive the filename from the cachePath and a computed key
std::string cacheFilename(const Options&);
