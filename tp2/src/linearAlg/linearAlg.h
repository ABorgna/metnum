#pragma once

#include "../serialization.h"
#include "sparse_vector.h"
#include "vector.h"

typedef std::pair<double, Vector> EigenValue;

typedef std::vector<Vector> Matriz;  // vector de filas

// Normas (L2 si no se especifica)
double norma(const Vector&);
double norma(const Vector&, int);
double norma(const SparseVector&);
double norma(const SparseVector&, int);

// Distancia norma N a la N
double distanciaN(const Vector&, const Vector&, int);
double distanciaN(const SparseVector&, const Vector&, int);
double distanciaN(const Vector&, const SparseVector&, int);
double distanciaN(const SparseVector&, const SparseVector&, int);

Vector operator*(const Matriz& M, const Vector& V);
