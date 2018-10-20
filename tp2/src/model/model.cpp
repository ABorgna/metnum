#include "model.h"

Model::~Model(){};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::SpEntries&& entries, int k)
    : invKnn(move(entries)), k(k) {};

bool ModelKNN::analize(const entry::SpEntry& test) const {
    return invKnn.knn(test, k);
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::SpEntries&& entries, int k, int alpha)
    : trainEntries(entries), k(k), alpha(alpha){};

bool ModelPCA::analize(const entry::SpEntry& test) const {
    // TODO: PSA
    return dumbKnn(trainEntries, test, k);
}
