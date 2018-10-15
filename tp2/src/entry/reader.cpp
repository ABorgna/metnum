#include "reader.h"

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../debug.h"

namespace entry {

void read_entry(std::string& line, TokenizedEntriesMap& train_entries,
                TokenizedEntriesMap& test_entries, EntryType type) {
    // Leo una línea y cargo una entrada
    int review_id(stoi(std::string(strtok(&line[0u], ","))));
    std::string dataset(strtok(NULL, ","));
    std::string polarity(strtok(NULL, ","));

    TokenizedEntry entry;
    char* pch = strtok(NULL, ",");
    while (pch != NULL) {
        entry.tokens.push_back(stoi(std::string(pch)));
        pch = strtok(NULL, ",");
    }

    entry.is_positive = polarity == "pos";
    if (dataset == "test") {
        if(type == ENTRY_ALL || type == ENTRY_TEST)
            test_entries[review_id] = entry;
    } else {
        if(type == ENTRY_ALL || type == ENTRY_TRAIN)
            train_entries[review_id] = entry;
    }
}

// Version privada con todos los parámetros posibles.
void read_entries(Input& file, TokenizedEntriesMap& train_entries,
                  TokenizedEntriesMap& test_entries, EntryType type) {
    DEBUG("Reading entries.");
    std::string line;
    if (file.fail())
        throw std::runtime_error(
            "An error occurred while opening the entries file.");
    int lineCount = 0;

    while (std::getline(file.stream(), line)) {
        lineCount++;
        read_entry(line, train_entries, test_entries, type);
    }

    if (&train_entries == &test_entries) {
        DEBUG(train_entries.size() << " entries read.");
    } else {
        DEBUG(train_entries.size() << " train entries read.");
        DEBUG(test_entries.size() << " test entries read.");
    }
}

void read_entries(Input& file, TokenizedEntriesMap& entries) {
    read_entries(file, entries, entries);
}

void read_entries(Input& file, TokenizedEntriesMap& entries, EntryType type) {
    read_entries(file, entries, entries, type);
}

void read_entries(Input& file, TokenizedEntriesMap& train_entries,
                  TokenizedEntriesMap& test_entries) {
    read_entries(file, train_entries, test_entries, ENTRY_ALL);
}

VocabToken read_vocab_token(std::string& line) {
    // Leo las últimas dos columnas
    std::string penultime, last;
    char* pch = strtok(&line[0u], " ,");
    while (pch != NULL) {
        penultime = last;
        last = pch;
        pch = strtok(NULL, " ,");
    }
    int token_id = stoi(last);
    double frequency = stof(penultime);
    return {token_id, frequency};
}

FrecuencyVocabularyMap read_vocabulary(Input& file, const VocabFilter& filter) {
    DEBUG("Reading the vocabulary.");
    FrecuencyVocabularyMap vocabulary;
    std::string line;
    if (file.fail())
        throw std::runtime_error(
            "An error occurred while opening the vocabulary file.");
    int lineCount = 0;

    std::getline(file.stream(), line);           // Sacamos el header del csv
    while (std::getline(file.stream(), line)) {  // Leemos una línea
        lineCount++;
        auto token = read_vocab_token(line);
        if (not filter(token)) {
            vocabulary.push_back(token);
        }
    }
    sort(vocabulary.begin(), vocabulary.end());

    const int sz = vocabulary.size();
    DEBUG(sz << " tokens read (" << (lineCount - sz) << " filtered).");

    return vocabulary;
}

FrecuencyVocabularyMap read_vocabulary(Input& file) {
    auto vocabulary = read_vocabulary(file, [](auto) { return false; });
    return vocabulary;
}

VocabFilter filterFrequent(double p) {
    return [p](const VocabToken& t) { return t.frequency > p; };
}

VocabFilter filterInfrequent(double p) {
    return [p](const VocabToken& t) { return t.frequency < p; };
}

VocabFilter filterPassBand(double minP, double maxP) {
    return [minP, maxP](const VocabToken& t) {
        return t.frequency < minP || t.frequency > maxP;
    };
}

}  // namespace entry
