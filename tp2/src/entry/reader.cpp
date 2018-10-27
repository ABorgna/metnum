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

void read_entry(std::string& line, TokenizedEntries& train_entries,
                TokenizedEntries& test_entries, EntryType type) {
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
    entry.id = review_id;
    if (dataset == "test") {
        if (type == ENTRY_ALL || type == ENTRY_TEST)
            test_entries.push_back(entry);
    } else {
        if (type == ENTRY_ALL || type == ENTRY_TRAIN)
            train_entries.push_back(entry);
    }
}

// Version privada con todos los parámetros posibles.
void read_entries(std::istream& file, TokenizedEntries& train_entries,
                  TokenizedEntries& test_entries, EntryType type) {
    DEBUG("Reading entries.");
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
        read_entry(line, train_entries, test_entries, type);
    }

    DEBUG(train_entries.size() << " entries read.");
}

void read_entries(std::istream& file, TokenizedEntries& entries,
                  EntryType type) {
    read_entries(file, entries, entries, type);
}

void read_entries(std::istream& file, TokenizedEntries& train_entries,
                  TokenizedEntries& test_entries) {
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

Vocabulary read_vocabulary(std::istream& file, const VocabFilter& filter) {
    DEBUG("Reading the vocabulary.");
    Vocabulary vocabulary;
    std::string line;
    int lineCount = 0;

    std::getline(file, line);           // Sacamos el header del csv
    while (std::getline(file, line)) {  // Leemos una línea
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

Vocabulary read_vocabulary(std::istream& file) {
    auto vocabulary = read_vocabulary(file, [](auto) { return false; });
    return vocabulary;
}

VocabFilter filterPassBand(double minP, double maxP) {
    return [minP, maxP](const VocabToken& t) {
        return t.frequency < minP || t.frequency > maxP;
    };
}

}  // namespace entry
