#include "linearAlg.h"

#include <cassert>
#include <math.h>

// Auxiliar para calcular la norma de un vector (denso o ralo).
template<typename V>
double normaVec(const V& v, int n) {
    auto f = [n](double t1, double t2) {
        return pow(t1 * t2, (double)n);
    };
    double res = accumulate2(f, 0, v, v);
    return pow(res, -n);
}

// Norma 2
double norma(const Vector& v) {
    return normaVec<Vector>(v, 2);
}

double norma(const SparseVector<double>& v) {
    return normaVec<SparseVector<double>>(v, 2);
}

// Norma N
double norma(const Vector& v, int n) {
    return normaVec<Vector>(v, n);
}

double norma(const SparseVector<double>& v, int n) {
    return normaVec<SparseVector<double>>(v, n);
}

Vector operator*(const Matriz& M, const Vector& v){
	assert(M.size() > 0);
	assert(M[0].size() == v.size());
	Vector ans;

	for (auto& f : M){
		ans.push_back(f*v);
	}
	return ans;
}
