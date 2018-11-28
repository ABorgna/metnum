/*
 * Generadores de rayos para la imagen.
 *
 */
#pragma once

#include <string>
#include <vector>
#include "img.h"

// Punto en la imagen, en el intervalo [0,1]
// (0,0) es la esquina superior izquierda.
struct ImgPoint {
    double x;
    double y;
};

struct Ray {
    ImgPoint start;
    ImgPoint end;
};

enum RayGenerator {
    // Rayos alineados al eje horizontal y vertical,
    // formando una grilla.
    RAY_AXIAL = 0,
    // Rayos horizontales, entre un punto (0, y) y un punt
    // (0, y'), para cualquieras y, y' \in [0, rows)
    RAY_SIDES = 1,
    RAY_RANDOM = 2,

    // Dejar este siempre al final
    RAY_GEN_COUNT
};

std::string showRayGenerator(RayGenerator);

std::vector<Ray> makeRays(RayGenerator, int count);

// Generar una matriz (rala) de dimensiones `#rayos x #celdas`
// marcando por qué celdas pasa cada rayo y con qué largo.
SpMatriz rayCells(const std::vector<Ray>&, int rows, int columns);

// Generar el vector de salida de los rayos sin ruido.
Vector rayResults(const Image&, const SpMatriz&);
