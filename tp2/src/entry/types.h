#ifndef TYPES__H
#define TYPES__H

#include <functional>
#include <unordered_map>
#include <vector>

namespace entry {

// Estructuras para las entradas tokenizadas
struct TokenizedEntry {
    bool is_positive;
    std::vector<int> tokens;
};
using TokenizedEntriesMap = std::unordered_map<int, TokenizedEntry>;

// Estructuras para las entradas vectorizadas
struct VectorizedEntry {
    bool is_positive;
    std::vector<double> bag_of_words;
};
using VectorizedEntriesMap = std::unordered_map<int, VectorizedEntry>;

// Token del vocabulario, con su frecuencia en el corpus.
struct VocabToken {
    int token;
    double frequency;
};

bool operator<(const VocabToken& x, const VocabToken& y);

// Estructura para el vocabulario.
// Vector de tokens ordenado por ids.
using FrecuencyVocabularyMap = std::vector<VocabToken>;

/**
 *  Lambda para usar como filtro de vocabulario
 *  Retorna `true` si `token` debe eliminarse
 *  Retorna `false` si `token` no debe eliminarse
 **/
typedef std::function<bool(const VocabToken&)> VocabFilter;

}  // namespace entry

#endif  // TYPES__H
