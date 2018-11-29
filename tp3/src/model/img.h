/*
 * Representación de imágenes.
 *
 */
#pragma once

#include <string>
#include "../linearAlg/linearAlg.h"

class Image {
   private:
    Vector _cells;
    size_t _rows;
    size_t _columns;

   public:
    Image() = default;
    Image(std::string& file, size_t rows, size_t columns);
    Image(std::istream& stream, size_t rows, size_t columns);
    Image(Vector&& cells, size_t rows, size_t columns);

    void write(std::string& file) const;
    void write(std::ostream& stream) const;

    // Accessors
    const Vector& cells() const;
    size_t rows() const;
    size_t columns() const;
};
