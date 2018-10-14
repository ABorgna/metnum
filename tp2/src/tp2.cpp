#include <getopt.h>
#include <iostream>
#include "files.h"
using namespace std;

void printHelp(const char* cmd) {
    cerr << "Usage: " << cmd << " [OPTIONS]" << endl
         << endl
         << "  Options:" << endl
         << "    -h             Show this help message." << endl
         << "    -v, verbose    Print debug info to stderr." << endl
         << "    -m #           Method:" << endl
         << "                     0: KNN (default)." << endl
         << "                     1: PCA + KNN." << endl
         << "    -t <file>      File with the training set. Use '-' for stdin."
         << endl
         << "                   (Default: 'train.csv')" << endl
         << "    -q <file>      File with the testing set. Use '-' for stdin."
         << endl
         << "                   (Default: 'test.csv')" << endl
         << "    -o <file>      Output file with the test file result. Use '-' "
            "for "
            "stdout."
         << endl
         << "                   (Default: stdout)" << endl
         << "    -c <file>      Cache the trained model in a file." << endl
         << "    -Q, --no-test  Only run the training step. Save the model "
            "using -c."
         << endl
         << endl;
}

enum Method {
    KNN,
    PCAKNN,

    // Add new methods before this entry
    METHODS_COUNT
};

int main(int argc, char* argv[]) {
    const char* const defaultTrainFile = "train.csv";
    const char* const defaultTestFile = "test.csv";
    const char* const defaultOutFile = "-";
    const char* const cmd = argv[0];

    Method method = KNN;
    const char* trainFile = defaultTrainFile;
    const char* testFile = defaultTestFile;
    const char* outFile = defaultOutFile;
    const char* cacheFile = nullptr;
    int debug = false;
    int dontTest = false;

    const char* const short_opts = "hvm:t:Qq:o:c:";
    const option long_opts[] = {/* These options set a flag. */
                                {"verbose", no_argument, &debug, 1},
                                {"no-test", no_argument, &dontTest, 1},
                                {0, 0, 0, 0}};

    while (true) {
        int option_index = 0;
        char flag =
            getopt_long(argc, argv, short_opts, long_opts, &option_index);
        if (flag == -1)
            break;
        switch (flag) {
            case 0:
                // Long option
                if (long_opts[option_index].flag != 0)
                    break;
                break;
            case 'h':
                printHelp(cmd);
                return -2;
            case 'v':
                debug = true;
                break;
            case 'm': {
                Method m = (Method)stoi(optarg);
                if (m < METHODS_COUNT)
                    method = m;
            } break;
            case 't':
                trainFile = optarg;
                break;
            case 'q':
                testFile = optarg;
                break;
            case 'Q':
                dontTest = 1;
                break;
            case 'o':
                outFile = optarg;
                break;
            case 'c':
                cacheFile = optarg;
                break;
            case '?':
                if (optopt == 't' || optopt == 'q' || optopt == 'o' ||
                    optopt == 'c') {
                    cerr << "Option -" << optopt << " needs an argument."
                         << endl;
                } else if (isprint(optopt)) {
                    cerr << "Unknown option `-" << (char)optopt << "'." << endl;
                } else {
                    cerr << "Unknown option." << endl;
                }
                cerr << endl;
                printHelp(cmd);
                return -2;
            default:
                abort();
        }
    }
    // Check if there are any leftover arguments
    if (argc - optind > 0) {
        printHelp(cmd);
        return -2;
    }

    // Open the files
    auto train = Input(trainFile);
    auto test = Input(testFile);
    auto out = Output(outFile);

    return 0;
}
