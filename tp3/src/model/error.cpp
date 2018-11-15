#include "error.h"
#include <random>

std::string showErrorGenerator(ErrorGenerator eg) {
    switch (eg) {
        case GAUSSIAN_ERROR:
            return "Gaussian Error";
        default:
            return "Unknown error distribution";
    }
}

Vector addNoise(ErrorGenerator, double errorSigma, unsigned seed,
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
