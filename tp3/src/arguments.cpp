#include "arguments.h"

#include <getopt.h>
#include <algorithm>
#include <iostream>

using namespace std;

void printHelp(const string& cmd, const Options& defaults) {
    cerr << "Usage: " << cmd << " inputImage resImage [OPTIONS]" << endl
         << endl

         << "    inputImage     Input image file. Use '-' for stdin." << endl
         << "    resImage       Generated image file. Use '-' for stdout."
         << endl
         << "                   csv only!!" << endl

         << endl
         << "Options:" << endl
         << "    -h, --help     Show this help message." << endl
         << "    -v, --verbose  Print debug info to stderr." << endl
         << "        --quiet    Do not print debug info." << endl
         << "    -j <threads>   Number of threads to utilize. (Default: # of "
            "cores)" << endl
         << "    -s, --seed #   Random seed." << endl

         << endl
         << "  RAYS" << endl
         << "    -r, --ray-type #" << endl
         << "                   Type of emulated rays:" << endl
         << "                     0: Axial rays (default)." << endl
         << "                     1: Side rays." << endl
         << "                     2: Random rays." << endl
         << "    -R, --ray-count #" << endl
         << "                   Number of rays "
         << " (Default:" << defaults.rayCount << ")" << endl
         << "    -n, --num-cells #" << endl
         << "                   Number of cells per row and per column."
         << " (Default:" << defaults.cellsPerRow << ")" << endl
         << "        --ray-file file.csv" << endl
         << "                   Store the generated rays in a csv file."

         << endl
         << "  MEASUREMENT ERROR" << endl
         << "    -e, --error-method #" << endl
         << "                   Emulated error type:" << endl
         << "                     0: Gaussian (default)." << endl
         << "                     1: No added noise." << endl
         << "    -E, --error #" << endl
         << "                   Standard deviation of the generated error. "
         << " (Default:" << defaults.errorSigma << ")" << endl

         << endl
         << "  LEAST SQUARES" << endl
         << "    -l, --lsq-method #" << endl
         << "                   Least squares method:" << endl
         << "                     0: SDV (default)." << endl
         << "    -a, --alpha #  SVD compression parameter (Default: "
         << defaults.alpha << ")." << endl

         << endl
         << "  CACHE" << endl
         << "    -c, --cache <path>" << endl
         << "                   Directory for storing and reading the trained "
            "model cache."
         << endl
         << "        --no-cache" << endl
         << "                   Don't use a cache for the model." << endl
         << endl;
}

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& opt) {
    const string cmd = argv[0];
    opt = defaults;

    const char* const short_opts = "hvj:r:R:n:e:E:l:c:s:a:";
    const option long_opts[] = {
        /* These options set a flag. */
        {"verbose", no_argument, &opt.debug, 1},
        {"quiet", no_argument, &opt.debug, 0},
        /* These options receive a parameter. */
        {"help", no_argument, nullptr, 'h'},

        {"ray-type", required_argument, nullptr, 'r'},
        {"ray-count", required_argument, nullptr, 'R'},
        {"num-cells", required_argument, nullptr, 'n'},
        {"ray-file", required_argument, nullptr, 2},

        {"error-method", required_argument, nullptr, 'e'},
        {"error", required_argument, nullptr, 'E'},

        {"lsq-method", required_argument, nullptr, 'l'},
        {"alpha", required_argument, nullptr, 'a'},

        {"cache", required_argument, nullptr, 'c'},
        {"no-cache", no_argument, nullptr, 1},

        {"seed", required_argument, nullptr, 's'},
        {0, 0, 0, 0}};

    while (true) {
        int option_index = 0;
        char flag =
            getopt_long(argc, argv, short_opts, long_opts, &option_index);
        if (flag == -1)
            break;
        switch (flag) {
            case 1:  // no-cache
                opt.cachePath = "";
                break;
            case 'h':
                return false;
            case 'v':
                opt.debug = true;
                break;
            case 'j': {
                int tmp = stoi(optarg);
                opt.nThreads = tmp;
            } break;
            case 's': {
                size_t tmp = stoi(optarg);
                opt.seed = tmp;
            } break;

            case 'r': {
                RayGenerator rg = (RayGenerator)stoi(optarg);
                if (rg < RAY_GEN_COUNT)
                    opt.rayGenerator = rg;
            } break;
            case 'R': {
                opt.rayCount = stoi(optarg);
            } break;
            case 2: {
                opt.raysOutFilename = optarg;
            } break;
            case 'n': {
                opt.cellsPerRow = stoi(optarg);
            } break;
            case 'e': {
                ErrorGenerator eg = (ErrorGenerator)stoi(optarg);
                if (eg < ERROR_GEN_COUNT)
                    opt.errorGenerator = eg;
            } break;
            case 'E': {
                double tmp = stod(optarg);
                if (tmp >= 0)
                    opt.errorSigma = tmp;
            } break;
            case 'l': {
                LSQMethod lg = (LSQMethod)stoi(optarg);
                if (lg < LSQ_METHOD_COUNT)
                    opt.lsqMethod = lg;
            } break;
            case 'a': {
                double tmp = stod(optarg);
                if (tmp >= 0)
                    opt.alpha = tmp;
            } break;
            case 'c': {
                opt.cachePath = optarg;
            } break;
            case '?': {
                const std::string withOpts = "jrRneElc";
                bool needsArgument = false;
                for (auto c : withOpts) needsArgument |= optopt == c;

                if (needsArgument) {
                    cerr << "Option -" << optopt << " needs an argument."
                         << endl;
                } else if (isprint(optopt)) {
                    cerr << "Unknown option `-" << (char)optopt << "'." << endl;
                } else {
                    cerr << "Unknown option." << endl;
                }
                cerr << endl;
                return false;
            }
            default:
                abort();
        }
    }
    // Check if there are any leftover arguments
    if (argc - optind != 2) {
        cerr << "Expecting two arguments, got " << (argc - optind) << "."
             << endl;
        cerr << endl;
        return false;
    }

    opt.inputFilename = argv[optind];
    opt.outputFilename = argv[optind + 1];

    return true;
}

size_t trainingCacheKey(const Options& o) {
    return std::hash<size_t>{}((size_t)o.rayGenerator) ^
           std::hash<size_t>{}((size_t)o.rayCount) ^
           std::hash<size_t>{}((size_t)o.cellsPerRow) ^
           std::hash<size_t>{}((size_t)o.errorGenerator) ^
           std::hash<double>{}(o.errorSigma) ^
           std::hash<size_t>{}((size_t)o.lsqMethod) ^
           std::hash<size_t>{}(o.seed) ^
           std::hash<double>{}(o.alpha);
}

std::string cacheFilename(const Options& o) {
    return o.cachePath + "/" + to_string(trainingCacheKey(o));
}
