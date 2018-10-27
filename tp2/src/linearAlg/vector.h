#pragma once

#include <functional>
#include <iostream>
#include <vector>

#include "../serialization.h"

typedef std::vector<double> Vector;

double accumulate2(
    std::function<double(size_t, double, double)> f, double init, const Vector&,
    const Vector&,
    std::function<double(double, double)> acum = std::plus<double>{});

void traverseVector(const Vector& v, std::function<void(size_t, double)> f);

double operator*(const Vector&, const Vector&);

Vector operator*(double e, const Vector& v);

Vector operator*(const Vector& v, double e);

Vector operator+(const Vector& v1, const Vector& v2);
