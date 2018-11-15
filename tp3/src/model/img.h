/*
 * Representación de imágenes.
 *
 */
#pragma once

#include <string>
#include "../linearAlg/linearAlg.h"

class Image {
   private:
    int rows;
    int columns;
    Vector cells;

   public:
    // Read a image from a file
    Image(std::istream& file);
    Image(Vector, int rows, int columns);

    void write(std::ostream& file);
};

typedef std::pair<double, double> ImgPoint;
