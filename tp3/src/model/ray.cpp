#include "ray.h"
#include <cmath>
#include <random>

std::string showRayGenerator(RayGenerator rg) {
    switch (rg) {
        case RAY_AXIAL:
            return "Axial rays";
        case RAY_SIDES:
            return "Side rays";
        case RAY_RANDOM:
            return "Random rays";
        default:
            return "Unknown ray type";
    }
}

/********************* Ray generators ************************************/

std::vector<Ray> axialRays(int count) {
    // # of possible spots
    const int vertical = count / 2;
    const int horizontal = count - vertical;

    std::vector<Ray> res;
    res.reserve(count);

    const double deltaY = 1.0 / horizontal;
    double y = deltaY / 2;
    for (int r = 0; r < horizontal; r++) {
        Ray ray = {{0, y}, {1, y}};
        res.push_back(ray);
        y += deltaY;
    }

    const double deltaX = 1.0 / vertical;
    double x = deltaX / 2;
    for (int c = 0; c < vertical; c++) {
        Ray ray = {{x, 0}, {x, 1}};
        res.push_back(ray);
        x += deltaX;
    }

    return res;
}

std::vector<Ray> sideRays(int count) {
    const int ends = std::ceil(std::sqrt(count));
    const int starts = ends;

    std::vector<Ray> res;
    res.reserve(count);

    int current = 0;

    const double deltaSt = 1.0 / starts;
    double st = deltaSt / 2;
    for (int a = 0; a < starts; a++) {
        const double deltaEnd = 1.0 / ends;
        double end = deltaEnd / 2;
        for (int b = 0; b < ends; b++) {
            current++;
            if (current > count)
                break;

            Ray ray = {{0, st}, {1, end}};
            res.push_back(ray);

            end += deltaEnd;
        }

        st += deltaSt;
    }

    return res;
}

ImgPoint randomPointInSide(int side) {
    double r = rand() / (RAND_MAX + 1.);

    switch (side) {
        case 0:
            return {0, r};
        case 1:
            return {r, 0};
        case 2:
            return {1, r};
        case 3:
            return {r, 1};
        default:
            throw "Invalid side";
    }
}

Ray generarRayoRandom() {
    int inicial = std::rand() % 4;
    int fin = (inicial + std::rand() % 3 + 1) % 4;

    return {randomPointInSide(inicial), randomPointInSide(fin)};
}

std::vector<Ray> randomRays(int count) {
    std::vector<Ray> res;
    res.reserve(count);
    for (int i = 0; i < count; i++) {
        Ray rayo = generarRayoRandom();
    }

    return res;
}

std::vector<Ray> makeRays(RayGenerator rg, int count) {
    switch (rg) {
        case RAY_AXIAL:
            return axialRays(count);
        case RAY_SIDES:
            return sideRays(count);
        case RAY_RANDOM:
            return randomRays(count);
        default:
            throw std::invalid_argument("Invalid ray type.");
    }
};

/********************* Ray processing ************************************/

// Punto con coordenadas en Z
struct IntCoords {
    int x;
    int y;
};

IntCoords fromImgPoint(ImgPoint p, int rows, int columns) {
    assert(rows > 0 and columns > 0);
    IntCoords res = {
        (int)std::floor(p.x * columns),
        (int)std::floor(p.y * rows),
    };
    if (res.x == columns)
        res.x--;
    if (res.y == rows)
        res.y--;
    return res;
}

// Calculate the cells crossed by the ray,
// with the exact distance traversed on each cell.
//
// Uses Xiaolin Wu's line algorithm for line antialiasing.
SparseVector crossedCellsExact(const Ray& r, int rows, int columns) {
    // TODO: Use doubles in the calculations
    IntCoords a = fromImgPoint(r.start, rows, columns);
    IntCoords b = fromImgPoint(r.end, rows, columns);
    std::map<size_t, double> res;

    bool steep = abs(b.y - a.y) > abs(b.x - a.x);
    if (steep) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }
    if (a.x > b.x) {
        std::swap(a, b);
    }

    int dx = b.x - a.x;
    int dy = b.y - a.y;
    double gradient = dx != 0 ? (double)dy / dx : 1;

    // handle first endpoint
    if (steep) {
        res[a.x * columns + a.y] = 1;
    } else {
        res[a.y * columns + a.x] = 1;
    }

    // handle second endpoint
    if (steep) {
        res[b.x * columns + b.y] = 1;
    } else {
        res[b.y * columns + b.x] = 1;
    }

    double intery = a.y + gradient;  // first y-intersection for the main loop

    // main loop
    for (int x = a.x + 1; x < b.x; x++) {
        int intPart = std::floor(intery);
        int fPart = intery - intPart;
        if (steep) {
            res[x * columns + intPart] = 1.0 - fPart;
            res[x * columns + (intPart + 1)] = fPart;
        } else {
            res[intPart * columns + x] = 1.0 - fPart;
            res[(intPart + 1) * columns + x] = fPart;
        }
        intery = intery + gradient;
    }

    return SparseVector(res, rows * columns);
}

// Calculate the cells crossed by the ray,
// assuming the distance traversed on each cell is always 1.
//
// Uses Bresenham's line algorithm to plot a line in a grid.
SparseVector crossedCellsBinary(const Ray& r, int rows, int columns) {
    IntCoords a = fromImgPoint(r.start, rows, columns);
    IntCoords b = fromImgPoint(r.end, rows, columns);

    IntCoords d = {b.x - a.x, b.y - a.y};
    d.x = abs(d.x);
    d.y = abs(d.y);

    IntCoords s = {a.x < b.x ? 1 : -1, a.y < b.y ? 1 : -1};
    int err = d.x - d.y;

    std::map<size_t, double> res;
    while (1) {
        int cell = a.y * columns + a.x;
        res[cell] = 1.0;

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

SpMatriz rayCells(const std::vector<Ray>& rays, int rows, int columns) {
    SpMatriz mtx;
    for (const auto& ray : rays) {
        mtx.push_back(crossedCellsExact(ray, rows, columns));
    }
    return mtx;
};

Vector rayResults(const Image& img, const SpMatriz& mtx) {
    return mtx * img.cells();
};
