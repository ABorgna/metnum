#include "img.h"
#include <opencv2/opencv.hpp>
#include "../linearAlg/linearAlg.h"

std::vector<char> streamToBuffer(std::istream& stream) {
    size_t readSz = 4096;
    size_t readOff = 0;
    size_t bCount = 0;
    std::vector<char> buf(readSz);
    while (stream.read(&(buf[0]) + readOff, readSz)) {
        bCount += readSz;
        readOff += readSz;

        readSz = buf.size();
        buf.resize(buf.size() + readSz);
    }
    bCount += stream.gcount();
    buf.resize(bCount);
    return buf;
}

Vector matToVector(const cv::Mat& mat) {
    std::vector<unsigned char> v;
    if (mat.isContinuous()) {
        v.assign(mat.datastart, mat.dataend);
    } else {
        for (int i = 0; i < mat.rows; ++i) {
            v.insert(v.end(), mat.ptr(i), mat.ptr(i) + mat.cols);
        }
    }

    Vector res(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        res[i] = (double)v[i] / 256.0;
    }
    return res;
}

cv::Mat vectorToMat(const Vector& v, size_t rows, size_t columns) {
    assert(v.size() == rows * columns);

    std::vector<unsigned char> chars(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        int x = round(v[i] * 256.0);
        chars[i] = std::min(std::max(x, 0), 255);
    }

    cv::Mat mat(rows, columns, CV_8UC1);
    memcpy(mat.data, &(chars[0]), chars.size());
    return mat;
}

Image::Image(std::string& file, size_t rows, size_t columns)
    : _rows(rows), _columns(columns) {
    cv::Mat imgCv = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);

    if (imgCv.empty()) {
        throw std::invalid_argument("Could not read the input image.");
    }

    cv::Mat resized;
    cv::resize(imgCv, resized, cv::Size(columns, rows));

    this->_cells = matToVector(resized);

    assert(this->_cells.size() == rows * columns);
}

Image::Image(std::istream& stream, size_t rows, size_t columns)
    : _rows(rows), _columns(columns) {
    auto buf = streamToBuffer(stream);
    cv::Mat imgCv = cv::imdecode(buf, CV_LOAD_IMAGE_GRAYSCALE);

    if (imgCv.empty()) {
        throw std::invalid_argument("Could not read the input image.");
    }

    cv::Mat resized;
    cv::resize(imgCv, resized, cv::Size(columns, rows));

    this->_cells = matToVector(resized);

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
    auto img = vectorToMat(this->_cells, this->_rows, this->_columns);
    cv::imwrite(file, img);
}

void Image::write(std::ostream& stream) const {
    auto img = vectorToMat(this->_cells, this->_rows, this->_columns);
    std::vector<unsigned char> buf;
    cv::imencode("png", img, buf);
    stream.write((char*)&(buf[0]), buf.size());
}

const Vector& Image::cells() const { return this->_cells; }

size_t Image::rows() const { return this->_rows; }

size_t Image::columns() const { return this->_columns; }
