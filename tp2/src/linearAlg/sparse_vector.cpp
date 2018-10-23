#include "sparse_vector.h"

#include <cstring>
#include <tuple>

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

SparseVector::SparseVector(const Vector& v) {
    for (size_t i = 0; i < v.size(); i++) {
        const double& x = v[i];
        const bool isZero = -Epsilon <= x && x <= Epsilon;

        if (not isZero)
            elems.emplace_back(i, x);
    }
}

size_t SparseVector::size() const { return sz; }

Vector SparseVector::toVector() const {
    Vector ans(size(), 0.0);
    for (const auto& it : elems) {
        ans[it.first] = it.second;
    }

    return ans;
}

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
                   const SparseVector& v1, const SparseVector& v2,
                   std::function<double(double, double)> op) {
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    double res = init;

    while (it1 != v1.end() and it2 != v2.end()) {
        if (it1->first < it2->first) {
            res = op(res, f((it1++)->second, 0));
        } else if (it1->first > it2->first) {
            res = op(res, f(0, (it2++)->second));
        } else {
            res = op(res, f((it1++)->second, (it2++)->second));
        }
    }
    for (; it1 != v1.end(); it1++) {
        res = op(res, f(it1->second, 0));
    }
    for (; it2 != v2.end(); it2++) {
        res = op(res, f(0, it2->second));
    }
    return res;
}

// Zip two vectors together and reduce the result.
double accumulate2(std::function<double(double, double)> f, double init,
                   const SparseVector& v1, const Vector& v2,
                   std::function<double(double, double)> op) {
    auto it1 = v1.begin();
    double res = init;

    for (size_t i = 0; i < v2.size(); i++) {
        if (it1 == v1.end()) {
            res = op(res, f(0, v2[i]));
            continue;
        }
        if (it1->first == i) {
            res = op(res, f((it1++)->second, v2[i]));
        } else {
            res = op(res, f(0, v2[i]));
        }
    }
    return res;
}

double accumulate2(std::function<double(double, double)> f, double init,
                   const Vector& v1, const SparseVector& v2,
                   std::function<double(double, double)> op) {
    return accumulate2([&f](double x, double y) { return f(y, x); }, init, v2,
                       v1, op);
}

void traverseVector(const SparseVector& v,
                    std::function<void(size_t, double)> f) {
    for (const auto& p : v) f(p.first, p.second);
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

std::ostream& operator<<(std::ostream& os, const SparseVector& v) {
    std::tuple<size_t, SparseVector::Container> tup(v.sz, v.elems);
    writeNamedTuple(os, "SparseVector", tup);
    return os;
}

std::istream& operator>>(std::istream& is, SparseVector& v) {
    std::tuple<size_t, SparseVector::Container> tup;
    readNamedTuple(is, "SparseVector", tup);
    std::tie(v.sz, v.elems) = tup;

    // Validate the data
    long long last = -1;
    for (auto p : v.elems) {
        if ((long long)p.first <= last or p.first >= v.sz) {
            // Invalid data
            v.sz = 0;
            v.elems = {};
            break;
        }
        last = p.first;
    }

    return is;
}
