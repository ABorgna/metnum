#pragma once

#include "sparse_vector.h"
#include "vector.h"

typedef std::pair<double, Vector> EigenValue;

typedef std::vector<Vector> Matriz;  // vector de filas

// Normas (L2 si no se especifica)
double norma(const Vector&);
double norma(const Vector&, int);
double norma(const SparseVector<double>&);
double norma(const SparseVector<double>&, int);

// Distancias (L2 si no se especifica)
double distancia(const Vector&, const Vector&);
double distancia(const Vector&, const Vector&, int);
double distancia(const SparseVector<double>&, const SparseVector<double>&);
double distancia(const SparseVector<double>&, const SparseVector<double>&, int);

Vector operator*(const Matriz& M, const Vector& V);
