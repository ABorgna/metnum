#include "error.h"
#include <random>

std::string showErrorGenerator(ErrorGenerator eg) {
    switch (eg) {
        case GAUSSIAN_ERROR:
            return "Gaussian Error";
        case NO_ERROR:
            return "No Error";
        default:
            return "Unknown error distribution";
    }
}

Vector addGaussNoise(double errorSigma, unsigned seed,
                const Vector& v) {
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0, errorSigma);

    Vector res(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        double err = distribution(generator);
        res[i] = v[i] + err;
    }

    return res;
}

Vector addNoise(ErrorGenerator eg, double errorSigma, unsigned seed,
                const Vector& v) {
    switch (eg) {
        case GAUSSIAN_ERROR:
            return addGaussNoise(errorSigma, seed, v);
        case NO_ERROR:
            return v;
        default:
            throw "Unknown error distribution";
    }
}
