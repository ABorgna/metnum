#include "sparse_vector.h"

// SparceVector class
SparseVector::SparseVector(const std::map<size_t, double>& m, size_t sz)
    : sz(sz) {
    elems.reserve(m.size());
    for (auto it : m) {
        const double& x = it.second;
        const bool isZero = -Epsilon <= x && x <= Epsilon;

        if (it.first < sz and not isZero)
            elems.emplace_back(it.first, x);
    }
}

size_t SparseVector::size() const { return sz; }

SparseVector::const_iterator SparseVector::at(size_t j) const {
    if (j >= sz)
        return elems.end();

    std::pair<size_t, double> target = {j,
                                        std::numeric_limits<double>::lowest()};
    auto it = lower_bound(elems.begin(), elems.end(), target);
    if (it == elems.end() || it->first > j) {
        return elems.end();
    } else {
        return it;
    }
}

double SparseVector::operator[](size_t j) const {
    auto it = at(j);
    if (it == elems.end()) {
        return 0;
    } else {
        return it->second;
    }
}

SparseVector::const_iterator SparseVector::begin() const noexcept {
    return elems.begin();
}

SparseVector::const_iterator SparseVector::end() const noexcept {
    return elems.end();
}

// Zip two vectors together and reduce the result.
double accumulate2(std::function<double(double, double)> f, double init,
                   const SparseVector& v1, const SparseVector& v2) {
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    double res = init;

    while (it1 != v1.end() and it2 != v2.end()) {
        if (it1->first < it2->first) {
            res += f((++it1)->second, 0);
        } else if (it1->first > it2->first) {
            res += f(0, (++it2)->second);
        } else {
            res += f((++it1)->second, (++it2)->second);
        }
    }
    for (; it1 != v1.end(); it1++) {
        res += f(it1->second, 0);
    }
    for (; it2 != v2.end(); it2++) {
        res += f(0, it2->second);
    }
    return res;
}

Vector operator+(const Vector& v1, const SparseVector& v2) {
    Vector res(v1);
    res += v2;
    return res;
}

Vector& operator+=(Vector& v1, const SparseVector& v2) {
    for (auto it = v2.begin(); it != v2.end(); it++) {
        const size_t pos = it->first;
        const double value = it->second;
        if (pos >= v1.size())
            break;
        v1[pos] += value;
    }
    return v1;
}

Vector operator-(const Vector& v1, const SparseVector& v2) {
    Vector res(v1);
    res -= v2;
    return res;
}

Vector& operator-=(Vector& v1, const SparseVector& v2) {
    for (auto it = v2.begin(); it != v2.end(); it++) {
        const size_t pos = it->first;
        const double value = it->second;
        if (pos >= v1.size())
            break;
        v1[pos] -= value;
    }
    return v1;
}
