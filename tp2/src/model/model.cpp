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
    : PCTrans(entries, alpha), analyzer(PCTrans.tcs(entries), k){};

bool ModelPCA::analize(const entry::Entry& test) const {
    return analyzer.analize(PCTrans.tc(test));
}
