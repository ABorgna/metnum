#include <string.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <queue>
#include <thread>

#include "arguments.h"
#include "model/potencia.h"
#include "debug.h"
#include "files.h"
#include "timer.h"

using namespace std;

const size_t SEED = 42;
// Used to invalidate the models cache
const size_t VERSION = 1;

const Options defaultOptions = {
    // Image input and output
    inputFilename : "",
    outputFilename : "",
    cachePath : "",

    // Rays
    rayGenerator : RAY_HORIZONTAL,
    rayCount : 100,
    cellsPerRow : 16,

    // Measurement error
    errorGenerator : GAUSSIAN_ERROR,
    errorSigma : 1.0,

    // LSQ solver
    lsqMethod : SVD,

    debug : true,
    nThreads: -1,
};

void showTimes(std::ostream& outStream, const TimeKeeper& timeKeeper) {
    for (const auto& p : timeKeeper.registry) {
        const std::string& label = p.first;
        const auto& millis = p.second;
        outStream << "time-" << label << ": " << millis.count() << "ms" << endl;
    }
}

int main(int argc, char* argv[]) {
    Options options;
    TimeKeeper timeKeeper;
    const string cmd = argv[0];
    string fullCmd = string(argv[0]);
    for (int i = 1; i < argc; i++) fullCmd += " " + string(argv[i]);

    if (not parseArguments(argc, argv, defaultOptions, options)) {
        printHelp(cmd, defaultOptions);
        return -1;
    }
    debugging_enabled = (bool)options.debug;

    /*****************************************************************/
    DEBUG("---------------- Reducing image size ------------");
    timeKeeper.start("reduceImage");
    // TODO
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Measuring rays ------------");

    timeKeeper.start("rayMeasurement");
    // TODO
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Reconstructing image ------------");
    auto outFile = Output(options.outputFilename);
    auto& outStream = outFile.stream();

    timeKeeper.start("reconstruction");
    // TODO

    timeKeeper.stop();

    outFile.close();

    /*****************************************************************/
    DEBUG("---------------- Results -----------------");
    showTimes(std::cout, timeKeeper);

    return 0;
}
