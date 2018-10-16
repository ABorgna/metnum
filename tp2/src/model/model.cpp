#include "model.h"

#include "knn.h"

Model::~Model(){};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::Entries&& entries, int k)
    : trainEntries(entries), k(k){};

bool ModelKNN::analize(const entry::Entry& test) const {
    return dumbKnn(trainEntries, test, k);
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::Entries&& entries, int k, int alpha)
    : trainEntries(entries), k(k), alpha(alpha){};

bool ModelPCA::analize(const entry::Entry& test) const {
    // TODO: PSA
    return dumbKnn(trainEntries, test, k);
}
