#pragma once

#include <functional>
#include <vector>

typedef std::vector<double> Vector;

double accumulate2(std::function<double(double, double)> f, double init,
                   const Vector&, const Vector&);

void traverseVector(const Vector& v, std::function<void(size_t, double)> f);

double operator*(const Vector&, const Vector&);
