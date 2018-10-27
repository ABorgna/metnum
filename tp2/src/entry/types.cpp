#include "types.h"

namespace entry {

bool operator<(const VocabToken& x, const VocabToken& y) {
    return x.token < y.token;
}

// Super ad-hoc auxiliary function
template <typename V>
Vector sumEntries(const Entries<V>& es) {
    if(es.size() == 0) return Vector();
    Vector res(es[0].bag_of_words.size());
    for(const auto& e : es) {
      res += e.bag_of_words;
    }
    return res;
}

template Vector sumEntries(const Entries<Vector>&);
template Vector sumEntries(const Entries<SparseVector>&);

}  // namespace entry

// We use the same tag no matter the instantiation of V.
// This is not optimal, but meh.
template <typename V>
std::ostream& operator<<(std::ostream& os, const entry::Entry<V>& e) {
    std::tuple<int, bool, V> tup (e.id, e.is_positive, e.bag_of_words);
    writeNamedTuple(os, "Entry", tup);
    return os;
}

template <typename V>
std::istream& operator>>(std::istream& is, entry::Entry<V>& e) {
    std::tuple<int, bool, V> tup;
    readNamedTuple(is, "Entry", tup);
    e = {std::get<0>(tup), std::get<1>(tup), std::get<2>(tup)};
    return is;
}

// Explicit instantiations for the exported functions
template std::ostream& operator<<(std::ostream& os,
                                  const entry::Entry<Vector>& e);
template std::ostream& operator<<(std::ostream& os,
                                  const entry::Entry<SparseVector>& e);
template std::istream& operator>>(std::istream& is, entry::Entry<Vector>& e);
template std::istream& operator>>(std::istream& is,
                                  entry::Entry<SparseVector>& e);
