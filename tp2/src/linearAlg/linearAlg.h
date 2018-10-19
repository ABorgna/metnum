#pragma once

#include <vector>

typedef std::pair<double, std::vector<double>> EigenValue;

//TODO borrar esto
typedef std::vector<double> Vector;

typedef std::vector<Vector> Matriz; // vector de filas

double operator*(const Vector& V1, const Vector& V2);
double Norma(const Vector& V);
Vector operator*(const Matriz& M, const Vector& V);
