/*
 * Generadores de errores de medicion para los rayos.
 *
 */
#pragma once

#include <string>

enum ErrorGenerator {
    GAUSSIAN_ERROR = 0,

    // Dejar este siempre al final
    ERROR_GEN_COUNT
};

std::string showErrorGenerator(const ErrorGenerator&);
