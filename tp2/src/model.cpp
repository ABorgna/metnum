#include "model.h"

#include "knn.h"

Model::~Model(){};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::VectorizedEntriesMap&& entries, int k)
    : trainEntries(entries), k(k){};

bool ModelKNN::analize(const entry::VectorizedEntry& test) const {
    return dumbKnn(trainEntries, test, k);
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::VectorizedEntriesMap&& entries, int k, int alpha)
    : trainEntries(entries), k(k), alpha(alpha){};

bool ModelPCA::analize(const entry::VectorizedEntry& test) const {
    // TODO: PSA
    return dumbKnn(trainEntries, test, k);
}
