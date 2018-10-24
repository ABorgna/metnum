#pragma once

#include "../serialization.h"
#include "sparse_vector.h"
#include "vector.h"

typedef std::pair<double, Vector> EigenValue;

typedef std::vector<Vector> Matriz;  // vector de filas

// Vector norms
enum Norm {
    NORM_CHI2 = -1,
    NORM_INF = 0,
    NORM_P = 0,  // NORM_P = p
};

inline Norm normP(int n) { return (Norm)(NORM_P + n); };

std::string showNorm(Norm);

// Normas (L2 si no se especifica)
double norma(const Vector&, int = 2);
double norma(const SparseVector&, int = 2);

// Distancia vectorial norma N a la N
template <typename V, typename W>
double distanciaN(const V&, const W&, Norm = normP(2));

// Distancia vectorial chi cuadrado
template <typename V, typename W>
double distanciaChi2(const V&, const W&, const Vector& sumEntries);

Vector operator*(const Matriz& M, const Vector& V);

Matriz operator*(const Matriz& M1, const Matriz& M2);

Matriz Id(int n);

Matriz logexp(const Matriz& M, int exp);
