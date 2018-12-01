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

std::vector<Ray> randomRays(int count, size_t seed) {
    std::vector<Ray> res;
    res.reserve(count);

    srand(seed);
    for (int i = 0; i < count; i++) {
        res.push_back(generarRayoRandom());
    }

    return res;
}

std::vector<Ray> makeRays(RayGenerator rg, int count, size_t seed) {
    switch (rg) {
        case RAY_AXIAL:
            return axialRays(count);
        case RAY_SIDES:
            return sideRays(count);
        case RAY_RANDOM:
            return randomRays(count, seed);
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
    ImgPoint a = {r.start.x * columns, r.start.y * rows};
    ImgPoint b = {r.end.x * columns, r.end.y * rows};

    std::map<size_t, double> res;

    const auto fpart = [](double x) { return x - std::floor(x); };
    const auto rfpart = [&fpart](double x) { return 1.0 - fpart(x); };
    const auto inRange = [&](int x, int y) {
        return x >= 0 and x < columns and y >= 0 and y < rows;
    };
    const auto plot = [&](int x, int y, double val) {
        if (inRange(x, y))
            res[y * columns + x] = val;
    };

    bool steep = abs(b.y - a.y) > abs(b.x - a.x);
    if (steep) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }
    if (a.x > b.x) {
        std::swap(a, b);
    }

    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double gradient = fabs(dx) < 1e-4 ? 1 : dy / dx;

    // handle first endpoint
    int xend = std::round(a.x);
    double yend = a.y + gradient * (xend - a.x);
    double xgap = rfpart(a.x + 0.5);
    int xpxl1 = xend;
    int ypxl1 = std::floor(yend);

    if (steep) {
        plot(ypxl1, xpxl1, rfpart(yend) * xgap);
        plot(ypxl1 + 1, xpxl1, fpart(yend) * xgap);
    } else {
        plot(xpxl1, ypxl1, rfpart(yend) * xgap);
        plot(xpxl1, ypxl1 + 1, fpart(yend) * xgap);
    }
    double intery = yend + gradient;  // first y-intersection for the main loop

    // handle second endpoint
    xend = std::round(b.x);
    yend = b.y + gradient * (xend - b.x);
    xgap = rfpart(b.x + 0.5);
    int xpxl2 = xend;
    int ypxl2 = std::floor(yend);
    if (steep) {
        plot(ypxl2, xpxl2, rfpart(yend) * xgap);
        plot(ypxl2 + 1, xpxl2, fpart(yend) * xgap);
    } else {
        plot(xpxl2, ypxl2, rfpart(yend) * xgap);
        plot(xpxl2, ypxl2 + 1, fpart(yend) * xgap);
    }

    // main loop
    for (int x = xpxl1 + 1; x < xpxl2; x++) {
        int y = std::floor(intery);
        if (steep) {
            plot(y, x, rfpart(intery));
            plot(y + 1, x, fpart(intery));
        } else {
            plot(x, y, rfpart(intery));
            plot(x, y + 1, fpart(intery));
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

void writeRays(std::ostream& stream, const SpMatriz& mtx,
               const std::vector<Ray>& rays, int rows, int columns) {
    assert(mtx.size() == rays.size());
    const size_t rowLength = rows * columns;
    for (size_t row = 0; row < mtx.size(); row++) {
        Ray ray = rays[row];
        const auto& v = mtx[row];
        // Ray info
        stream << "(" << ray.start.x << "," << ray.start.y << ")"
               << ",";
        stream << "(" << ray.end.x << "," << ray.end.y << ")"
               << ",";
        // Img data
        for (size_t i = 0; i < rowLength; i++) {
            stream << v[i];
            stream << (i == rowLength - 1 ? '\n' : ',');
        }
    }
}
