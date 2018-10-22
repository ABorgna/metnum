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

// Distancias (L2 si no se especifica)
double distancia(const Vector&, const Vector&, int = 2);
double distancia(const SparseVector&, const Vector&, int = 2);
double distancia(const Vector&, const SparseVector&, int = 2);
double distancia(const SparseVector&, const SparseVector&, int = 2);

Vector operator*(const Matriz& M, const Vector& V);
