#include "arguments.h"

#include <getopt.h>
#include <iostream>

using namespace std;

std::string showMethod(Method m) {
    switch (m) {
        case (KNN):
            return "KNN";
        case (KNN_INVERTED):
            return "Inverted Index KNN";
        case (PCAKNN):
            return "PCA+KNN";
        case (PCAKNN_INVERTED):
            return "PCA+Inverted index KNN";
        default:
            return "unknown method";
    }
}

void printHelp(const string& cmd, const Options& defaults) {
    cerr << "Usage: " << cmd << " [OPTIONS]" << endl
         << endl
         << "Options:" << endl
         << "    -h, --help     Show this help message." << endl
         << "    -v, --verbose  Print debug info to stderr." << endl
         << "        --quiet    Do not print debug info." << endl
         << "  MODEL" << endl
         << "    -m #           Method:" << endl
         << "                     0: kNN." << endl
         << "                     1: Inverted index kNN." << endl
         << "                     2: PCA + kNN." << endl
         << "                     3: PCA + Inverted Index kNN (default)." << endl
         << "    -k #           K hyper-parameter for kNN (Default: "
         << defaults.k << ")" << endl
         << "    -a #           Alpha hyper-parameter for PCA (Default: "
         << defaults.alpha << ")" << endl
         << "    -c <file>      Cache the trained model in a file." << endl
         << "    -Q, --no-test  Only run the training step. Save the model "
            "using -c."
         << endl
         << "  ENTRIES" << endl
         << "    -t <file>      File with the training set. Use '-' for stdin."
         << endl
         << "                   (Default: '" << defaults.trainFilename << "')"
         << endl
         << "    -q <file>      File with the testing set. Use '-' for stdin."
         << endl
         << "                   (Default: '" << defaults.testFilename << "')"
         << endl
         << "    -o <file>      Output file with the test entries result. Use "
            "'-' "
            "for stdout."
         << endl
         << "                   (Default: stdout)" << endl
         << "        --train-entries n" << endl
         << "                   Train at most n entries." << endl
         << "        --test-entries n" << endl
         << "                   Test at most n entries." << endl
         << "  VOCABULARY" << endl
         << "    -p, --vocabulary <file>"
         << "                   File with the vocabulary." << endl
         << "                   (Default: '" << defaults.vocabFilename << "')"
         << endl
         << "        --minVocabFreq <number>" << endl
         << "                   Trim the tokens in the vocabulary with small "
            "frequency."
         << endl
         << "                   (Default: " << defaults.minVocabFreq << ")"
         << endl
         << "        --maxVocabFreq <number>" << endl
         << "                   Trim the tokens in the vocabulary with large "
            "frequency."
         << endl
         << "                   (Default: " << defaults.maxVocabFreq << ")"
         << endl
         << endl;
}

bool parseArguments(int argc, char* argv[], const Options& defaults,
                    Options& opt) {
    const string cmd = argv[0];
    opt = defaults;

    const char* const short_opts = "hvm:t:Qq:p:o:c:a:k:";
    const option long_opts[] = {/* These options set a flag. */
                                {"verbose", no_argument, &opt.debug, 1},
                                {"quiet", no_argument, &opt.debug, 0},
                                {"no-test", no_argument, &opt.dontTest, 1},
                                /* These options receive a parameter. */
                                {"help", required_argument, nullptr, 'h'},
                                {"vocabulary", required_argument, nullptr, 'p'},
                                {"minVocabFreq", required_argument, nullptr, 1},
                                {"maxVocabFreq", required_argument, nullptr, 2},
                                {"train-entries", required_argument, nullptr, 3},
                                {"test-entries", required_argument, nullptr, 4},
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
            case 1: {
                // Min vocabulary frequency
                double freq = stod(optarg);
                if (freq < 0 || freq > 1) {
                    cerr << "Invalid minimum frequency: " << freq << endl;
                    return -3;
                }
                opt.minVocabFreq = freq;
            } break;
            case 2: {
                // Max vocabulary frequency
                double freq = stod(optarg);
                if (freq < 0 || freq > 1) {
                    cerr << "Invalid maximum frequency: " << freq << endl;
                    return -3;
                }
                opt.maxVocabFreq = freq;
            } break;
            case 3: {
                // Max train entries
                int n = stoi(optarg);
                // Negative means no limit
                if (n < 0)
                  n = -1;
                opt.maxTrainEntries = n;
            } break;
            case 4: {
                // Max test entries
                int n = stoi(optarg);
                // Negative means no limit
                if (n < 0)
                  n = -1;
                opt.maxTestEntries = n;
            } break;
            case 'h':
                return false;
            case 'v':
                opt.debug = true;
                break;
            case 'm': {
                Method m = (Method)stoi(optarg);
                if (m < METHODS_COUNT)
                    opt.method = m;
            } break;
            case 'k': {
                int tmp = stoi(optarg);
                if (tmp > 0)
                    opt.k = tmp;
            } break;
            case 'a': {
                int tmp = stoi(optarg);
                if (tmp > 0)
                    opt.alpha = tmp;
            } break;
            case 't':
                opt.trainFilename = optarg;
                break;
            case 'Q':
                opt.dontTest = 1;
                break;
            case 'q':
                opt.testFilename = optarg;
                break;
            case 'p':
                opt.vocabFilename = optarg;
                break;
            case 'o':
                opt.outFilename = optarg;
                break;
            case 'c':
                opt.cacheFilename = optarg;
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
                return false;
            default:
                abort();
        }
    }
    // Check if there are any leftover arguments
    if (argc - optind > 0) {
        return false;
    }

    return true;
}
