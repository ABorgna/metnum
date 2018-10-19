#ifndef TYPES__H
#define TYPES__H

#include <Eigen/SparseCore>
#include <functional>
#include <vector>

namespace entry {

// Estructuras para las entradas tokenizadas
struct TokenizedEntry {
    int id;
    bool is_positive;
    std::vector<int> tokens;
};
using TokenizedEntries = std::vector<TokenizedEntry>;

// Estructuras para las entradas vectorizadas
struct Entry {
    int id;
    bool is_positive;
    Eigen::SparseVector<double> bag_of_words;
};
using Entries = std::vector<Entry>;

// Token del vocabulario, con su frecuencia en el corpus.
struct VocabToken {
    int token;
    double frequency;
};

bool operator<(const VocabToken& x, const VocabToken& y);

// Estructura para el vocabulario.
// Vector de tokens ordenado por ids.
using Vocabulary = std::vector<VocabToken>;

/**
 *  Lambda para usar como filtro de vocabulario
 *  Retorna `true` si `token` debe eliminarse
 *  Retorna `false` si `token` no debe eliminarse
 **/
typedef std::function<bool(const VocabToken&)> VocabFilter;

}  // namespace entry

#endif  // TYPES__H
