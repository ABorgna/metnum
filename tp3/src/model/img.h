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
    size_t rows;
    size_t columns;

   public:
    Image() = default;
    Image(std::string& file, size_t rows, size_t columns);
    Image(std::istream& stream, size_t rows, size_t columns);
    Image(Vector&& cells, size_t rows, size_t columns);

    void write(std::string& file) const;
    void write(std::ostream& stream) const;
};

typedef std::pair<double, double> ImgPoint;
