/*
 * Métodos para correr cuadrados mínimos
 *
 */
#pragma once

#include <string>

enum LSQMethod {
    SVD = 0,

    // Dejar este siempre al final
    LSQ_METHOD_COUNT
};

std::string showLSQMethod(const LSQMethod&);
