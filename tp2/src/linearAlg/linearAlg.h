#pragma once

#include "../serialization.h"
#include "sparse_vector.h"
#include "vector.h"

typedef std::pair<double, Vector> EigenValue;

typedef std::vector<Vector> Matriz;  // vector de filas

// Normas (L2 si no se especifica)
double norma(const Vector&, int = 2);
double norma(const SparseVector&, int = 2);

// Distancia norma N a la N
double distanciaN(const Vector&, const Vector&, int = 2);
double distanciaN(const SparseVector&, const Vector&, int = 2);
double distanciaN(const Vector&, const SparseVector&, int = 2);
double distanciaN(const SparseVector&, const SparseVector&, int = 2);

Vector operator*(const Matriz& M, const Vector& V);

Matriz operator*(const Matriz& M1, const Matriz& M2);

Matriz Id(int n);

Matriz logexp(const Matriz& M, int exp);
