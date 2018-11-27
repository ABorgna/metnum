#include <string.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <queue>
#include <thread>

#include "arguments.h"
#include "debug.h"
#include "files.h"
#include "model/svd.h"
#include "model/potencia.h"
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
    rayGenerator : RAY_AXIAL,
    rayCount : 100,
    cellsPerRow : 16,

    // Measurement error
    errorGenerator : GAUSSIAN_ERROR,
    errorSigma : 1.0,

    // LSQ solver
    lsqMethod : SVD,

    debug : true,
    nThreads : -1,
};

int main(int argc, char* argv[]) {
    Options opt;
    TimeKeeper timeKeeper;
    const string cmd = argv[0];
    string fullCmd = string(argv[0]);
    for (int i = 1; i < argc; i++) fullCmd += " " + string(argv[i]);

    if (not parseArguments(argc, argv, defaultOptions, opt)) {
        printHelp(cmd, defaultOptions);
        return -1;
    }
    debugging_enabled = (bool)opt.debug;

    /*****************************************************************/
    DEBUG("---------------- Processing image ------------");

    timeKeeper.start("readImage");
    Image img;
    if (opt.inputFilename == "-") {
        // We need to read binary data from stdin
        freopen(NULL, "rb", stdin);
        img = Image(std::cin, opt.cellsPerRow, opt.cellsPerRow);
        freopen(NULL, "r", stdin);
    } else {
        img = Image(opt.inputFilename, opt.cellsPerRow, opt.cellsPerRow);
    }
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Creating rays and preprocessing LSQ ------------");

    timeKeeper.start("createRays");
    auto rays = makeRays(opt.rayGenerator, opt.cellsPerRow, opt.cellsPerRow,
                         opt.rayCount);
    SpMatriz measurements = rayCells(rays, opt.cellsPerRow, opt.cellsPerRow);
    Vector pureResults = rayResults(img, measurements);
    timeKeeper.stop();

    timeKeeper.start("lsqPreprocessing");
    // TODO
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Storing LSQ cache ------------");

    timeKeeper.start("writeCache");
    // TODO
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Runing LSQ ------------");

    timeKeeper.start("addingNoise");
    Vector results =
        addNoise(opt.errorGenerator, opt.errorSigma, SEED, pureResults);
    timeKeeper.stop();

    timeKeeper.start("lsq");
    Vector x;
    x = cuadradosMinimosConSVD(measurements, results);
    // TODO: run LSQ for `measurements * x = results`

    timeKeeper.stop();

    DEBUG("---------------- Writing image ------------");

    timeKeeper.start("writeImg");
    // TODO: Uncomment this.
    Image res(move(x), opt.cellsPerRow, opt.cellsPerRow);
    //Image res = img;
    if (opt.inputFilename == "-") {
        // We need to write binary data to stdout
        freopen(NULL, "wb", stdout);
        res.write(std::cout);
    } else {
        res.write(opt.outputFilename);
    }
    timeKeeper.stop();

    /*****************************************************************/
    DEBUG("---------------- Results -----------------");
    showTimes(std::cerr, timeKeeper);

    return 0;
}
