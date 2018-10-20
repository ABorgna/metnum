#include "linearAlg.h"

#include <math.h>
#include <algorithm>
#include <cassert>

double norma(const Vector& v) { return norma(v, 2); }

double norma(const Vector& v, int n) {
    double res = 0;
    for_each(v.begin(), v.end(), [&res, n](double x) { res += pow(x, n); });
    return res;
}

double norma(const SparseVector& v) { return norma(v, 2); }

double norma(const SparseVector& v, int n) {
    double res = 0;
    for_each(v.begin(), v.end(),
             [&res, n](const auto& p) { res += pow(p.second, n); });
    return res;
}

// Auxiliar para calcular la distancia entre vectores (densos o ralos).
template <typename V>
double distVec(const V& v1, const V& v2, int n) {
    auto f = [n](double t1, double t2) {
        return pow(fabs(t1 - t2), (double)n);
    };
    double res = accumulate2(f, 0, v1, v2);
    return pow(res, -n);
}

// Distancias (L2 si no se especifica)
double distancia(const Vector& v1, const Vector& v2) {
    return distVec(v1, v2, 2);
}

double distancia(const Vector& v1, const Vector& v2, int n) {
    return distVec(v1, v2, n);
}

double distancia(const SparseVector& v1, const SparseVector& v2) {
    return distVec(v1, v2, 2);
}

double distancia(const SparseVector& v1, const SparseVector& v2, int n) {
    return distVec(v1, v2, n);
}

Vector operator*(const Matriz& M, const Vector& v) {
    assert(M.size() > 0);
    assert(M[0].size() == v.size());
    Vector ans;

    for (auto& f : M) {
        ans.push_back(f * v);
    }
    return ans;
}
