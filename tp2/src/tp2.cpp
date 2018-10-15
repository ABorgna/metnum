#include <getopt.h>
#include <string.h>
#include <iostream>

#include "debug.h"
#include "entry/reader.h"
#include "entry/vector_builder.h"
#include "files.h"

using namespace std;

const string defaultTrainFile = "data/imdb_tokenized.csv";
const string defaultTestFile = "data/imdb_tokenized.csv";
const string defaultOutFile = "-";

const string defaultVocabFile = "data/vocab.csv";
const double defaultMinVocabFreq = 0.01;
const double defaultMaxVocabFreq = 0.99;

void printHelp(const string& cmd) {
    cerr << "Usage: " << cmd << " [OPTIONS]" << endl
         << endl
         << "  Options:" << endl
         << "    -h, --help     Show this help message." << endl
         << "    -v, --verbose  Print debug info to stderr." << endl
         << "        --quiet    Do not print debug info." << endl
         << "    -m #           Method:" << endl
         << "                     0: KNN (default)." << endl
         << "                     1: PCA + KNN." << endl
         << "    -t <file>      File with the training set. Use '-' for stdin."
         << endl
         << "                   (Default: '" << defaultTrainFile << "')" << endl
         << "    -q <file>      File with the testing set. Use '-' for stdin."
         << endl
         << "                   (Default: '" << defaultTestFile << "')" << endl
         << "    -p, --vocabulary <file>"
         << "                   File with the vocabulary." << endl
         << "                   (Default: '" << defaultVocabFile << "')" << endl
         << "        --minVocabFreq <number>" << endl
         << "                   Trim the tokens in the vocabulary with small "
            "frequency."
         << endl
         << "                   (Default: " << defaultMinVocabFreq << ")"
         << endl
         << "        --maxVocabFreq <number>" << endl
         << "                   Trim the tokens in the vocabulary with large "
            "frequency."
         << endl
         << "                   (Default: " << defaultMaxVocabFreq << ")"
         << endl
         << "    -o <file>      Output file with the test file result. Use '-' "
            "for stdout."
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
    const string cmd = argv[0];

    // Print all the info by default (TODO: remove this later)
    debugging_enabled = true;

    Method method = KNN;
    string trainFilename = defaultTrainFile;
    string testFilename = defaultTestFile;
    string outFilename = defaultOutFile;
    string cacheFilename;

    string vocabFilename = defaultVocabFile;
    double minVocabFreq = defaultMinVocabFreq;
    double maxVocabFreq = defaultMaxVocabFreq;

    int debugFlag = (int)debugging_enabled;  // We need an int reference.
    int dontTest = false;

    const char* const short_opts = "hvm:t:Qq:p:o:c:";
    const option long_opts[] = {/* These options set a flag. */
                                {"verbose", no_argument, &debugFlag, 1},
                                {"quiet", no_argument, &debugFlag, 0},
                                {"no-test", no_argument, &dontTest, 1},
                                /* These options receive a parameter. */
                                {"help", required_argument, nullptr, 'h'},
                                {"vocabulary", required_argument, nullptr, 'p'},
                                {"minVocabFreq", required_argument, nullptr, 1},
                                {"maxVocabFreq", required_argument, nullptr, 2},
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
                minVocabFreq = freq;
            } break;
            case 2: {
                // Max vocabulary frequency
                double freq = stod(optarg);
                if (freq < 0 || freq > 1) {
                    cerr << "Invalid maximum frequency: " << freq << endl;
                    return -3;
                }
                maxVocabFreq = freq;
            } break;
            case 'h':
                printHelp(cmd);
                return -2;
            case 'v':
                debugFlag = true;
                break;
            case 'm': {
                Method m = (Method)stoi(optarg);
                if (m < METHODS_COUNT)
                    method = m;
            } break;
            case 't':
                trainFilename = optarg;
                break;
            case 'Q':
                dontTest = 1;
                break;
            case 'q':
                testFilename = optarg;
                break;
            case 'p':
                vocabFilename = optarg;
                break;
            case 'o':
                outFilename = optarg;
                break;
            case 'c':
                cacheFilename = optarg;
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
    debugging_enabled = (bool)debugFlag;

    /*************** Read the entries ********************/
    entry::FrecuencyVocabularyMap vocabulary;
    entry::VectorizedEntriesMap trainEntries;
    entry::VectorizedEntriesMap testEntries;

    auto vocabFile = Input(vocabFilename);
    vocabulary = entry::read_vocabulary(
        vocabFile, entry::filterPassBand(minVocabFreq, maxVocabFreq));
    vocabFile.close();

    entry::TokenizedEntriesMap trainTokenized;
    entry::TokenizedEntriesMap testTokenized;
    if (trainFilename == testFilename) {
        auto file = Input(testFilename);
        entry::read_entries(file, trainTokenized, testTokenized);
        file.close();
    } else {
        auto trainFile = Input(trainFilename);
        auto testFile = Input(testFilename);
        entry::read_entries(trainFile, trainTokenized);
        entry::read_entries(testFile, testTokenized);
        testFile.close();
        trainFile.close();
    }

    trainEntries = entry::vectorizeMap(vocabulary, trainTokenized);
    testEntries = entry::vectorizeMap(vocabulary, testTokenized);

    DEBUG("Finished preprocessing the data.");
    DEBUG("    Vocabulary size: " << vocabulary.size());
    DEBUG("    Train entries count: " << trainEntries.size());
    DEBUG("    Test entries count: " << testEntries.size());

    /*************** Train the model ********************/
    DEBUG("---------------- Training ----------------");

    /*************** Test the model ********************/
    DEBUG("---------------- Testing ----------------");
    auto outFile = Output(outFilename);

    outFile.close();

    return 0;
}
