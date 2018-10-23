#include "linearAlg.h"

#include <math.h>
#include <algorithm>
#include <cassert>

double norma(const Vector& v, int n) {
    double res = 0;
    for_each(v.begin(), v.end(), [&res, n](double x) { res += pow(x, n); });
    return pow(res, 1.0 / ((double)n));
}

double norma(const SparseVector& v, int n) {
    double res = 0;
    for_each(v.begin(), v.end(),
             [&res, n](const auto& p) { res += pow(p.second, n); });
    return pow(res, 1.0 / ((double)n));
}

// Auxiliar para calcular la distancia entre vectores (densos o ralos).
template <typename V, typename W>
double distanciaN(const V& v1, const W& v2, Norm norm) {
    double res;
    switch (norm) {
        case NORM_CHI2:
            res = 1;
            break;
        case NORM_INF: {
            auto f = [](double t1, double t2) { return fabs(t1 - t2); };
            res = accumulate2(f, 0, v1, v2, [](double a, double b) {
                return std::max(a, b);
            });
        } break;
        default: {  // p-norm
            assert((int)norm > 0);
            auto f = [norm](double t1, double t2) {
                return pow(fabs(t1 - t2), (double)norm);
            };
            res = accumulate2(f, 0, v1, v2);
        } break;
    }
    return res;
}

template double distanciaN(const Vector& v1, const Vector& v2, Norm norm);
template double distanciaN(const SparseVector& v1, const Vector& v2, Norm norm);
template double distanciaN(const Vector& v1, const SparseVector& v2, Norm norm);
template double distanciaN(const SparseVector& v1, const SparseVector& v2,
                           Norm norm);

Vector operator*(const Matriz& M, const Vector& v) {
    assert(M.size() > 0);
    assert(M[0].size() == v.size());
    Vector ans;

    for (auto& f : M) {
        ans.push_back(f * v);
    }
    return ans;
}

Matriz operator*(const Matriz& M1, const Matriz& M2) {
    assert(M1[0].size() == M2.size());
    Matriz ans(M1.size(), Vector(M2[0].size()));

    for (size_t i = 0; i < ans.size(); i++) {
        for (size_t j = 0; j < ans[i].size(); j++) {
            for (size_t k = 0; k < M2.size(); k++) {
                ans[i][j] += M1[i][k] * M2[k][j];
            }
        }
    }
    return ans;
}

Matriz Id(int n) {
    Matriz ans(n, Vector(n, 0));

    for (int i = 0; i < n; i++) ans[i][i] = 1;

    return ans;
}

Matriz logexp(const Matriz& M, int exp) {
    assert(M.size() == M[0].size());
    if (exp == 0)
        return Id(M.size());

    Matriz rec(logexp(M, exp >> 1));

    if (exp & 1) {
        return M * rec * rec;
    } else {
        return rec * rec;
    }
}
