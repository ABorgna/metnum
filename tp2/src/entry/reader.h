#ifndef READER__H
#define READER__H

#include "types.h"

#include <iostream>

namespace entry {

enum EntryType {
    ENTRY_ALL,
    ENTRY_TRAIN,
    ENTRY_TEST,
};

/**
 *  Parsea el archivo de reviews tokenizadas, filtrando por el tipo de entry.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
void read_entries(std::istream& file, TokenizedEntries&, EntryType = ENTRY_ALL);

/**
 *  Parsea el archivo de reviews tokenizadas,
 *  dividiendo las entradas entre testing y training.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
void read_entries(std::istream& file, TokenizedEntries& train_entries,
                  TokenizedEntries& test_entries);

/**
 *  Parsea el archivo de vocabulario.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
Vocabulary read_vocabulary(std::istream& file);
Vocabulary read_vocabulary(std::istream& file, const VocabFilter&);

/*
 * Filtro de vocabulario frecuente e infrecuente.
 */
VocabFilter filterPassBand(double minP = 0, double maxP = 1);

}  // namespace entry
#endif  // READER__H
