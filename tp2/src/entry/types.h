#ifndef TYPES__H
#define TYPES__H

#include <functional>
#include <vector>

#include "../linearAlg/linearAlg.h"

namespace entry {

// Estructuras para las entradas tokenizadas
struct TokenizedEntry {
    int id;
    bool is_positive;
    std::vector<int> tokens;
};
using TokenizedEntries = std::vector<TokenizedEntry>;

// Estructuras para las entradas vectorizadas
template <typename V>
struct Entry {
    int id;
    bool is_positive;
    V bag_of_words;
};

template <typename V>
using Entries = std::vector<Entry<V>>;

// Sparse entries
using SpEntry = Entry<SparseVector>;
using SpEntries = Entries<SparseVector>;

// Dense entries
using DnsEntry = Entry<Vector>;
using DnsEntries = Entries<Vector>;

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

// Input/output operators
template <typename V>
std::ostream& operator<<(std::ostream&, const entry::Entry<V>&);
template <typename V>
std::istream& operator>>(std::istream&, entry::Entry<V>&);

#endif  // TYPES__H
