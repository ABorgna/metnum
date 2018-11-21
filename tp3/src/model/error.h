/*
 * Generadores de errores de medicion para los rayos.
 *
 */
#pragma once

#include <string>
#include "../linearAlg/linearAlg.h"

enum ErrorGenerator {
    GAUSSIAN_ERROR = 0,
    NO_ERROR = 1,

    // Dejar este siempre al final
    ERROR_GEN_COUNT
};

std::string showErrorGenerator(ErrorGenerator);

Vector addNoise(ErrorGenerator, double errorSigma, unsigned seed,
                const Vector&);
