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
    Vector v;
    if (mat.isContinuous()) {
        v.assign((float*)mat.datastart, (float*)mat.dataend);
    } else {
        for (int i = 0; i < mat.rows; ++i) {
            v.insert(v.end(), mat.ptr<float>(i), mat.ptr<float>(i) + mat.cols);
        }
    }
    return v;
}

Image::Image(std::string& file, int rows, int columns)
    : rows(rows), columns(columns) {
    cv::Mat imgCv = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);

    if (imgCv.empty()) {
        throw std::invalid_argument("Could not read the input image.");
    }

    cv::Mat resized;
    cv::resize(imgCv, resized, cv::Size(columns, rows));

    this->cells = matToVector(resized);
}

Image::Image(std::istream& stream, int rows, int columns)
    : rows(rows), columns(columns) {
    auto buf = streamToBuffer(stream);
    cv::Mat imgCv = cv::imdecode(buf, CV_LOAD_IMAGE_GRAYSCALE);

    if (imgCv.empty()) {
        throw std::invalid_argument("Could not read the input image.");
    }

    cv::Mat resized;
    cv::resize(imgCv, resized, cv::Size(columns, rows));

    this->cells = matToVector(resized);
}

Image::Image(Vector&& cells, int rows, int columns)
    : cells(cells), rows(rows), columns(columns) {
    if ((size_t)(rows * columns) != cells.size()) {
        throw std::invalid_argument(
            "Trying to construct an image with an invalid number of cells!\n");
    }
}

void Image::write(std::ostream& file) const {}
