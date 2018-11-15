/*
 * Representación de imágenes.
 *
 */
#pragma once

#include <string>
#include "../linearAlg/linearAlg.h"

class Image {
   private:
    Vector cells;
    int rows;
    int columns;

   public:
    Image() = default;
    Image(std::string& file, int rows, int columns);
    Image(std::istream& stream, int rows, int columns);
    Image(Vector&& cells, int rows, int columns);

    void write(std::ostream& file) const;
};

typedef std::pair<double, double> ImgPoint;
