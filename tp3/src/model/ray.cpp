#include "ray.h"

std::string showRayGenerator(RayGenerator rg) {
    switch (rg) {
        case RAY_AXIAL:
            return "Axial rays";
        default:
            return "Unknown ray type";
    }
}

/********************* Ray generators ************************************/

std::vector<Ray> axialRays(int rows, int columns, int count) {
    // # of possible spots
    const int spots = rows + columns;
    const int loops = count / spots;
    int remaining = count % spots;

    std::vector<Ray> res;
    res.reserve(count);

    for (int y = 0; y < rows; y++) {
        int cnt = loops;
        if (remaining > 0) {
            cnt++;
            remaining--;
        }
        for (int i = 0; i < cnt; i++) {
            Ray ray = {{0, y}, {columns + 1, y}};
            res.push_back(ray);
        }
    }

    for (int x = 0; x < columns; x++) {
        int cnt = loops;
        if (remaining > 0) {
            cnt++;
            remaining--;
        }
        for (int i = 0; i < cnt; i++) {
            Ray ray = {{x, 0}, {x, rows + 1}};
            res.push_back(ray);
        }
    }

    return res;
}

std::vector<Ray> makeRays(RayGenerator rg, int rows, int columns, int count) {
    switch (rg) {
        case RAY_AXIAL:
            return axialRays(rows, columns, count);
        default:
            throw std::invalid_argument("Invalid ray type.");
    }
};

/********************* Ray processing ************************************/

SparseVector singleRayCells(const Ray& r, int rows, int columns) {
    // Using Bresenham's line algorithm
    // to plot a line in a grid.
    ImgPoint a = r.start;
    ImgPoint b = r.end;

    ImgPoint d = {b.x - a.x, b.y - a.y};
    d.x = abs(d.x);
    d.y = abs(d.y);

    ImgPoint s = {a.x < b.x ? 1 : -1, a.y < b.y ? 1 : -1};
    int err = d.x - d.y;

    std::map<size_t, double> res;
    while (1) {
        int cell = a.y * columns + a.x;
        double len = 1; // TODO: Calculate distance
        res[cell] = len;

        if (a.x == b.x and a.y == b.y)
            break;

        int e2 = err;
        if (e2 >= 0) {
            err -= 2 * d.y;
            a.x += s.x;
        }
        if (e2 <= 0) {
            err += 2 * d.x;
            a.y += s.y;
        }
    }

    return SparseVector(res, rows * columns);
}

SpMatriz rayCells(const std::vector<Ray>& rays, int rows, int columns){
    SpMatriz mtx;
    for(const auto& ray : rays) {
        mtx.push_back(singleRayCells(ray, rows, columns));
    }
    return mtx;
};

Vector rayResults(const Image& img, const SpMatriz& mtx) {
    return mtx * img.cells();
};
