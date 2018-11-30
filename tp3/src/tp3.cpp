#include <string.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <queue>
#include <thread>

#include "arguments.h"
#include "cache.h"
#include "debug.h"
#include "files.h"
#include "model/potencia.h"
#include "model/svd.h"
#include "timer.h"

using namespace std;

// Used to invalidate the models cache
const size_t VERSION = 3;

const Options defaultOptions = {
    // Image input and output
    inputFilename : "",
    outputFilename : "",
    cachePath : "cache",
    raysOutFilename : "",

    // Rays
    rayGenerator : RAY_AXIAL,
    rayCount : 100,
    cellsPerRow : 16,

    // Measurement error
    errorGenerator : GAUSSIAN_ERROR,
    errorSigma : 1.0,

    // LSQ solver
    lsqMethod : SVD,
    alpha : 10,

    debug : true,
    nThreads : -1,
    seed : 42,
    runLsq : true,
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

    SpMatriz measurements;
    USVt descomposicion;
    bool validCache = false;
    if (opt.cachePath != "") {
        DEBUG("---------------- Reading cache ------------");
        DEBUG("Loading from " << cacheFilename(opt));

        timeKeeper.start("loadCache");
        validCache = fromCache(opt, VERSION, measurements, descomposicion);
        timeKeeper.stop();
    }

    /*****************************************************************/
    DEBUG("---------------- Creating rays and preprocessing LSQ ------------");

    if (not validCache) {
        timeKeeper.start("createRays");
        auto rays = makeRays(opt.rayGenerator, opt.rayCount, opt.seed);
        measurements = rayCells(rays, opt.cellsPerRow, opt.cellsPerRow);
        timeKeeper.stop();

        timeKeeper.start("lsqPreprocessing");
        descomposicion = descomposicionSVD(measurements, opt.alpha);
        timeKeeper.stop();
    }

    timeKeeper.start("rayResults");
    Vector pureResults = rayResults(img, measurements);
    timeKeeper.stop();

    /*****************************************************************/
    if (opt.cachePath != "" and not validCache) {
        DEBUG("---------------- Storing cache ------------");

        timeKeeper.start("writeCache");
        saveCache(opt, VERSION, fullCmd, measurements, descomposicion);
        timeKeeper.stop();
    }

    /*****************************************************************/
    if (opt.raysOutFilename != "") {
        DEBUG("---------------- Storing rays file ------------");

        timeKeeper.start("writeRays");
        auto rays = makeRays(opt.rayGenerator, opt.rayCount, opt.seed);
        auto raysFile = Output(opt.raysOutFilename);
        if (raysFile.fail()) {
            DEBUG("Could not open the rays file: " << opt.raysOutFilename);
        } else {
            writeRays(raysFile.stream(), measurements, rays, opt.cellsPerRow,
                      opt.cellsPerRow);
            raysFile.close();
        }
        timeKeeper.stop();
    }

    /*****************************************************************/
    if (opt.runLsq) {
        DEBUG("---------------- Runing LSQ ------------");

        timeKeeper.start("addingNoise");
        Vector results =
            addNoise(opt.errorGenerator, opt.errorSigma, opt.seed, pureResults);
        timeKeeper.stop();

        timeKeeper.start("lsq");
        Vector x = cuadradosMinimosConSVD(descomposicion, results);
        timeKeeper.stop();

        DEBUG("---------------- Writing image ------------");

        timeKeeper.start("writeImg");
        Image res(move(x), opt.cellsPerRow, opt.cellsPerRow);
        if (opt.inputFilename == "-") {
            // We need to write binary data to stdout
            freopen(NULL, "wb", stdout);
            res.write(std::cout);
        } else {
            res.write(opt.outputFilename);
        }
        timeKeeper.stop();
    }

    /*****************************************************************/
    DEBUG("---------------- Results -----------------");
    showTimes(std::cerr, timeKeeper);
    std::cerr << "sing-values: " << get<1>(descomposicion).size() << std::endl;

    return 0;
}
