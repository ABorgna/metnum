#include "model.h"

Model::~Model(){};

/********** KNN model ************/

ModelKNN::ModelKNN(entry::SpEntries&& entries, int k)
    : trainEntries(entries), k(k) {};

bool ModelKNN::analize(const entry::SpEntry& test) const {
    return dumbKnn(trainEntries, test, k);
}

/********** Inverted KNN model ************/

ModelKNNInv::ModelKNNInv(entry::SpEntries&& entries, int k)
    : invKnn(move(entries)), k(k) {};

bool ModelKNNInv::analize(const entry::SpEntry& test) const {
    return invKnn.knn(test, k);
}

/********** PCA+KNN model ************/

ModelPCA::ModelPCA(entry::SpEntries&& entries, int k, int alpha)
    : PCTrans(entries, alpha), analyzer(PCTrans.tcs(entries), k){};

bool ModelPCA::analize(const entry::SpEntry& test) const {
    return analyzer.analize(PCTrans.tc(test));
}
