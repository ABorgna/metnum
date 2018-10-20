#pragma once

#include <cassert>
#include <functional>
#include <limits>
#include <map>
#include <vector>

#include "vector.h"

/*
 * Constant sparse vector structure.
 *
 * Modifications are not allowed once the vector has been constructed.
 *
 * Based on a vector of pairs since it's more cache-friendly than a map
 * for iteration.
 */

const double Epsilon = 1e-14;

class SparseVector {
    typedef std::vector<std::pair<size_t, double>> Container;
    Container elems;
    size_t sz;

   public:
    // Iterators

    typedef typename Container::const_iterator const_iterator;

    // Constructors
    SparseVector() : sz(0){};

    SparseVector(const std::map<size_t, double>& m, size_t sz);

    SparseVector(const Vector& v);

    // Casting
    Vector toVector() const;

    // Queries

    size_t size() const;

    const_iterator at(size_t j) const;

    double operator[](size_t j) const;

    const_iterator begin() const noexcept;

    const_iterator end() const noexcept;
};

// Zip two vectors together and reduce the result.
double accumulate2(std::function<double(double, double)> f, double init,
                   const SparseVector& v1, const SparseVector& v2);
double accumulate2(std::function<double(double, double)> f, double init,
                   const SparseVector& v1, const Vector& v2);
double accumulate2(std::function<double(double, double)> f, double init,
                   const Vector& v1, const SparseVector& v2);

void traverseVector(const SparseVector& v, std::function<void(size_t, double)> f);

// Operations with Vector
Vector operator+(const Vector&, const SparseVector&);
Vector& operator+=(Vector&, const SparseVector&);
Vector operator-(const Vector&, const SparseVector&);
Vector& operator-=(Vector&, const SparseVector&);
