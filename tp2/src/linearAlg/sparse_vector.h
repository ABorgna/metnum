#pragma once

#include <limits>
#include <map>
#include <vector>

/*
 * Constant sparse vector structure.
 *
 * Modifications are not allowed once the vector has been constructed.
 *
 * Based on a vector of pairs since it's more cache-friendly than a map
 * for iteration.
 */

const double Epsilon = 1e-14;

template <typename T>
class SparseVector {
    typedef std::vector<std::pair<size_t, T>> Container;
    Container elems;
    size_t sz;

   public:
    // Iterators

    typedef typename Container::const_iterator const_iterator;

    // Constructors
    SparseVector() : sz(0){};

    SparseVector(const std::map<size_t, T>& m, size_t sz) : sz(sz) {
        elems.reserve(m.size());
        for (auto it : m) {
            const T& x = it.second;
            const bool isZero = -Epsilon <= x && x <= Epsilon;

            if (it.first < sz and not isZero)
                elems.emplace_back(it.first, x);
        }
    }

    //

    size_t size() const { return sz; }

    // Element access

    const_iterator at(size_t j) const {
        if (j >= sz)
            return elems.end();

        auto target = {j, std::numeric_limits<T>::lowest()};
        auto it = lower_bound(elems.begin(), elems.end(), target);
        if (it == elems.end() || it->first > j) {
            return elems.end();
        } else {
            return it;
        }
    }

    T operator[](size_t j) const {
        auto it = at(j);
        if (it == elems.end()) {
            return (T)0;  // T tiene que ser casteable desde 0
        } else {
            return it->second;
        }
    }

    const_iterator begin() const noexcept { return elems.begin(); }

    const_iterator end() const noexcept { return elems.end(); }
};
