#include "img.h"
#include "../linearAlg/linearAlg.h"
#include <sstream>
#include <fstream>

Vector csvToVector(std::istream& stream){
    std::string line;
    Vector res;
    while(getline(stream, line)){
        std::istringstream tokenStream(line);
        std::string token;
        while (std::getline(tokenStream, token, ',')){
            res.push_back(std::stoi(token));
        }
    }
    return res;
}

void vectorToCsv(std::ostream& stream, const Vector& v, size_t rows, size_t columns){
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < columns-1; j++){
            stream << ' ' << v[i*columns + j] << ',';
        }
        stream << ' ' << v[i*(columns + 1) - 1] << std::endl;
    }

}


Image::Image(std::string& file, size_t rows, size_t columns)
    : _rows(rows), _columns(columns) {
    std::fstream fs;
    fs.open (file, std::fstream::in);

    this->_cells = csvToVector(fs);
    fs.close();
    assert(this->_cells.size() == rows * columns);
}

Image::Image(std::istream& stream, size_t rows, size_t columns)
    : _rows(rows), _columns(columns) {
    this->_cells = csvToVector(stream);
    assert(this->_cells.size() == rows * columns);
}

Image::Image(Vector&& cells, size_t rows, size_t columns)
    : _cells(cells), _rows(rows), _columns(columns) {
    if ((size_t)(rows * columns) != cells.size()) {
        throw std::invalid_argument(
            "Trying to construct an image with an invalid number of cells!\n");
    }
}

void Image::write(std::string& file) const {
    std::fstream fs;
    fs.open(file, std::fstream::out);
    write(fs);
    fs.close();
}

void Image::write(std::ostream& stream) const {
    vectorToCsv(stream, _cells, _rows, _columns);
}

const Vector& Image::cells() const { return this->_cells; }

size_t Image::rows() const { return this->_rows; }

size_t Image::columns() const { return this->_columns; }
