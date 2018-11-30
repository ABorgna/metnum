/*
 * Métodos para correr cuadrados mínimos
 *
 */
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <chrono>

#include "../linearAlg/linearAlg.h"
using namespace std;
typedef std::tuple<Matriz, vector<double>, Matriz> USVt; //tenemos la V, y la sigma guardando solo la diagonal

typedef vector<double> Diag;

USVt descomposicionSVD(const SpMatriz &A, double alpha); 

Vector cuadradosMinimosConSVD(const USVt &A, Vector b);
Diag inversaDiagonalNoNula(Diag& D);