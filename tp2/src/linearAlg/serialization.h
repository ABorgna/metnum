#pragma once

/*
 * Serialization of vector and pair.
 *
 * We probably shouldn't be writing this for all the template instantiations,
 * but it's way easier this way...
 *
 * */

#include <iostream>
#include <vector>

// Writes the vector as "[size;x0,x1,x2,...]"
template <typename T>
std::ostream& operator<<(std::ostream&, const std::vector<T>&);

// Reads a vector encoded as "[size;x0,x1,x2,...]"
template <typename T>
std::istream& operator>>(std::istream&, std::vector<T>&);

// Writes the pair as "(x,y)"
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p);

// Reads a pair encoded as "(x,y)"
template <typename T, typename U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& p);

/******************** Template implementations *********************/

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    bool first = true;
    os << '[' << v.size() << ';';
    for (const T& x : v) {
        if (not first)
            os << ',';
        else
            first = false;
        os << x;
    }
    return os << ']';
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    char c;

    // '['
    is >> c;
    if (c != '[') {
        v.resize(0);
        return is;
    }

    // size
    // Don't reserve more than 10k, to prevent malformed input DoS.
    size_t s;
    is >> s;
    v.reserve(std::min(s, (size_t)10000));

    is >> c;
    if (c != ';') {
        v.resize(0);
        return is;
    }

    // data
    for (c = ','; c == ','; is >> c) {
        T x;
        is >> x;
        v.push_back(std::move(x));
    }

    // ]
    if (c != ']') {
        v.resize(0);
        return is;
    }

    return is;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    return os << '(' << p.first << ',' << p.second << ')';
}

template <typename T, typename U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& p) {
    char c;
    p.first = {};
    p.second = {};

    // '('
    is >> c;
    if (c != '(')
        return is;

    is >> p.first;

    // ','
    is >> c;
    if (c != ',')
        return is;

    is >> p.second;

    // ')'
    is >> c;
    if (c != ')')
        return is;
    return is;
}
