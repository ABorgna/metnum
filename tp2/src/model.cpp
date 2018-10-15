#include "model.h"

Model::~Model(){};
bool Model::analize(const entry::VectorizedEntry& e __attribute__((unused))) {
    return true;
};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::VectorizedEntriesMap&& entries, int k)
    : trainEntries(entries), k(k){};

bool ModelKNN::analize(const entry::VectorizedEntry& entry
                       __attribute__((unused))) {
    // return runKNN(entry, k);
    return true;
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::VectorizedEntriesMap&& entries, int k, int alpha)
    : trainEntries(entries), k(k), alpha(alpha){};

bool ModelPCA::analize(const entry::VectorizedEntry& entry
                       __attribute__((unused))) {
    // ???
    return true;
}
