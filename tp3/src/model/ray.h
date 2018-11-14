/*
 * Generadores de rayos para la imagen.
 *
 */
#pragma once

#include <string>

enum RayGenerator {
    RAY_HORIZONTAL = 0,

    // Dejar este siempre al final
    RAY_GEN_COUNT
};

std::string showRayGenerator(const RayGenerator&);
