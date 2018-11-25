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

typedef std::vector<Matriz> USVt;

USVt SVD(Matriz &A); 