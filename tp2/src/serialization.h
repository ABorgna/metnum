#pragma once

/*
 * Serialization of vectors, pairs, and tuples.
 *
 * We probably shouldn't be writing this for all the template instantiations,
 * but we'd need an external library otherwise.
 *
 * */

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "debug.h"

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

// Writes the tuple as "(x1,x2,...)"
template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& tup);

// Reads a tuple encoded as "(x1,x2,...)"
template <typename... T>
std::istream& operator>>(std::istream& is, const std::tuple<T...>& tup);

// Writes the tuple as "name(x1,x2,...)"
template <typename... T>
void writeNamedTuple(std::ostream& os, const std::string& name,
                     const std::tuple<T...>& tup);

// Reads a tuple encoded as "name(x1,x2,...)"
template <typename... T>
void readNamedTuple(std::istream& is, const std::string& name,
                    std::tuple<T...>& tup);

inline void dropWhitespace(std::istream& is) {
    while(is.peek() == ' ' or is.peek() == '\n')
        is.get();
}

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
    dropWhitespace(is);
    is >> c;
    if (c != '[') {
        DEBUG("Parser: Invalid vector start delimiter '" << c << "'");
        v.resize(0);
        return is;
    }

    // size
    // Don't reserve more than 10k, to prevent malformed input DoS.
    size_t sz;
    is >> sz;
    v.reserve(std::min(sz, (size_t)10000));

    dropWhitespace(is);
    is >> c;
    if (c != ';') {
        DEBUG("Parser: Invalid vector size delimiter '" << c << "'");
        v.resize(0);
        return is;
    }

    // data
    if (sz > 0) {
        for (c = ','; c == ','; is >> c) {
            T x;
            is >> x;
            v.push_back(std::move(x));
            dropWhitespace(is);
        }
    } else {
        dropWhitespace(is);
        is >> c;
    }

    // ]
    if (c != ']') {
        DEBUG("Parser: Invalid vector end delimiter '" << c << "'");
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
    dropWhitespace(is);
    is >> c;
    if (c != '(')
        return is;

    dropWhitespace(is);
    is >> p.first;

    // ','
    dropWhitespace(is);
    is >> c;
    if (c != ',')
        return is;

    dropWhitespace(is);
    is >> p.second;

    dropWhitespace(is);
    is >> c;  // ')'
    return is;
}

template <size_t n, typename... T>
typename std::enable_if<(n >= sizeof...(T))>::type print_tuple(
    std::ostream&, const std::tuple<T...>&) {}

template <size_t n, typename... T>
typename std::enable_if<(n < sizeof...(T))>::type print_tuple(
    std::ostream& os, const std::tuple<T...>& tup) {
    if (n != 0)
        os << ",";
    os << std::get<n>(tup);
    print_tuple<n + 1>(os, tup);
}

template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& tup) {
    os << "(";
    print_tuple<0>(os, tup);
    return os << ")";
}

template <size_t n, typename... T>
typename std::enable_if<(n >= sizeof...(T))>::type read_tuple(
    std::istream&, std::tuple<T...>&) {}

template <size_t n, typename... T>
typename std::enable_if<(n < sizeof...(T))>::type read_tuple(
    std::istream& is, std::tuple<T...>& tup) {
    if (n != 0) {
        char c;
        dropWhitespace(is);
        is >> c;
        if (c != ',') {
            DEBUG("Parser: Invalid tuple separator '" << c << "'");
            return;
        }
    }
    is >> std::get<n>(tup);
    read_tuple<n + 1>(is, tup);
}

template <typename... T>
std::istream& operator>>(std::istream& is, std::tuple<T...>& tup) {
    dropWhitespace(is);
    tup = {};
    char c;
    is >> c;
    if (c != '(')
        return is;
    read_tuple<0>(is, tup);
    dropWhitespace(is);
    is >> c;  // ')'
    return is;
}

template <typename... T>
void writeNamedTuple(std::ostream& os, const std::string& name,
                     const std::tuple<T...>& tup) {
    os << name << tup;
}

template <typename... T>
void readNamedTuple(std::istream& is, const std::string& name,
                    std::tuple<T...>& tup) {
    const size_t nameLen = name.size();
    dropWhitespace(is);

    std::string s(nameLen + 1, ' ');
    is.get(&s[0], nameLen + 1, '(');
    s.pop_back();
    if (s != name) {
        tup = {};
        DEBUG("Parser: Invalid named tuple " << s);
        return;
    }
    is >> tup;
}
