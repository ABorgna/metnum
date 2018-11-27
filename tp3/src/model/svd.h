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
typedef std::pair<Matriz, vector<double>> USVt; //tenemos la U, y la sigma guardando solo la diagonal

typedef vector<double> Diag;

USVt descomposicionSVD(Matriz &&A); 

Vector cuadradosMinimosConSVD(const SpMatriz &A, vector<double> b);
Diag inversaDiagonalNoNula(Diag& D);