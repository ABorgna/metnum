#pragma once

#include "vector.h"
#include "sparse_vector.h"

typedef std::pair<double, Vector> EigenValue;

typedef std::vector<Vector> Matriz; // vector de filas

// Norma 2
double norma(const Vector&);
double norma(const SparseVector<double>&);

// Norma N
double norma(const Vector&, int);
double norma(const SparseVector<double>&, int);

Vector operator*(const Matriz& M, const Vector& V);
