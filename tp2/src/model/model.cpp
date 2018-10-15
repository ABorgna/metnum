#include "model.h"

Model::~Model(){};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::Entries&& entries, int k)
    : invKnn(move(entries)), k(k) {};

bool ModelKNN::analize(const entry::Entry& test) const {
    return invKnn.knn(test, k);
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::Entries&& entries, int k, int alpha)
    : trainEntries(entries), k(k), alpha(alpha){};

bool ModelPCA::analize(const entry::Entry& test) const {
    // TODO: PSA
    return dumbKnn(trainEntries, test, k);
}
