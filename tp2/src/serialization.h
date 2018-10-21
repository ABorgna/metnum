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
        is >> c;
        if (c != ',')
            return;
    }
    is >> std::get<n>(tup);
    read_tuple<n + 1>(is, tup);
}

template <typename... T>
std::istream& operator>>(std::istream& is, std::tuple<T...>& tup) {
    tup = {};
    char c;
    is >> c;
    if (c != '(')
        return is;
    read_tuple<0>(is, tup);
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
    std::string s(nameLen + 1, ' ');
    is.get(&s[0], nameLen + 1, '(');
    s.pop_back();
    if (s != name) {
        tup = {};
        return;
    }
    is >> tup;
}
