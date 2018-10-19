#include "sparse_vector.h"

// Zip two vectors together and reduce the result.
double accumulate2(std::function<double(double, double)> f, double init,
              const SparseVector<double>& v1, const SparseVector<double>& v2) {
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

Vector operator+(const Vector& v1, const SparseVector<double>& v2) {
    Vector res(v1);
    res += v2;
    return res;
}

Vector& operator+=(Vector& v1, const SparseVector<double>& v2) {
    for (auto it = v2.begin(); it != v2.end(); it++) {
        const size_t pos = it->first;
        const double value = it->second;
        if (pos >= v1.size())
            break;
        v1[pos] += value;
    }
    return v1;
}

Vector operator-(const Vector& v1, const SparseVector<double>& v2) {
    Vector res(v1);
    res -= v2;
    return res;
}

Vector& operator-=(Vector& v1, const SparseVector<double>& v2) {
    for (auto it = v2.begin(); it != v2.end(); it++) {
        const size_t pos = it->first;
        const double value = it->second;
        if (pos >= v1.size())
            break;
        v1[pos] -= value;
    }
    return v1;
}
