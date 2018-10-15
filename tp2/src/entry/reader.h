#ifndef READER__H
#define READER__H

#include "../files.h"
#include "types.h"

namespace entry {

enum EntryType {
    ENTRY_ALL,
    ENTRY_TRAIN,
    ENTRY_TEST,
};

/**
 *  Parsea el archivo de reviews tokenizadas.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
void read_entries(Input& file, TokenizedEntriesMap&);

/**
 *  Parsea el archivo de reviews tokenizadas, filtrando por el tipo de entry.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
void read_entries(Input& file, TokenizedEntriesMap&, EntryType);

/**
 *  Parsea el archivo de reviews tokenizadas,
 *  dividiendo las entradas entre testing y training.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
void read_entries(Input& file, TokenizedEntriesMap& train_entries,
                  TokenizedEntriesMap& test_entries);

/**
 *  Parsea el archivo de vocabulario.
 *  El archivo en cuestión no debe tener una línea vacía al final.
 **/
FrecuencyVocabularyMap read_vocabulary(Input& file);
FrecuencyVocabularyMap read_vocabulary(Input& file, const VocabFilter&);

/*
 * Filtro de vocabulario frecuente.
 */
VocabFilter filterFrequent(double p);

/*
 * Filtro de vocabulario infrecuente.
 */
VocabFilter filterInfrequent(double p);

/*
 * Filtro de vocabulario frecuente e infrecuente.
 */
VocabFilter filterPassBand(double minP, double maxP);

}  // namespace entry
#endif  // READER__H
