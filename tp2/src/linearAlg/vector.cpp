#include "vector.h"

#include <cassert>

double accumulate2(std::function<double(double, double)> f, double init,
                   const Vector& v1, const Vector& v2) {
    double res = init;
    size_t n = std::min(v1.size(), v2.size());
    for (size_t i = 0; i < n; i++) {
        res += f(v1[i], v2[i]);
    }
    return res;
}

void traverseVector(const Vector& v, std::function<void(size_t, double)> f) {
    for (size_t i = 0; i < v.size(); i++) f(i, v[i]);
}

double operator*(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());
    double ans = 0;
    for (size_t i = 0; i < v1.size(); i++) {
        ans += v1[i] * v2[i];
    }
    return ans;
}
