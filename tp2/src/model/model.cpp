#include "model.h"

template <typename Te>
Model<Te>::~Model(){};

/********** KNN model ************/

// Default constructor
template <typename Tr, typename Te>
ModelKNNtmp<Tr, Te>::ModelKNNtmp(){};

template <typename Tr, typename Te>
ModelKNNtmp<Tr, Te>::ModelKNNtmp(entry::Entries<Tr>&& entries, int k, Norm norm)
    : trainEntries(entries), k(k), norm(norm) {
    sumVocab = entry::sumEntries(trainEntries);
};

template <typename Tr, typename Te>
ModelKNNtmp<Tr, Te>::ModelKNNtmp(std::istream& is, int k, Norm norm)
    : k(k), norm(norm) {
    is >> trainEntries;
    is >> sumVocab;
}

template <typename Tr, typename Te>
bool ModelKNNtmp<Tr, Te>::shouldCache() const {
    return false;
}

template <typename Tr, typename Te>
void ModelKNNtmp<Tr, Te>::saveCache(std::ostream& os) const {
    os << trainEntries << std::endl;
    os << sumVocab << std::endl;
}

template <typename Tr, typename Te>
bool ModelKNNtmp<Tr, Te>::analyze(const entry::Entry<Te>& test) const {
    return dumbKnn<Tr, Te>(trainEntries, test, k, norm, sumVocab);
}

template class ModelKNNtmp<SparseVector, SparseVector>;
template class ModelKNNtmp<Vector, Vector>;

/********** Inverted KNN model ************/

// Default constructor
template <typename Tr, typename Te>
ModelKNNInvtmp<Tr, Te>::ModelKNNInvtmp(){};

template <typename Tr, typename Te>
ModelKNNInvtmp<Tr, Te>::ModelKNNInvtmp(entry::Entries<Tr>&& entries, int k,
                                       Norm norm)
    : invKnn(move(entries)), k(k), norm(norm){};

template <typename Tr, typename Te>
ModelKNNInvtmp<Tr, Te>::ModelKNNInvtmp(std::istream& is, int k, Norm norm)
    : k(k), norm(norm) {
    is >> invKnn;
}

template <typename Tr, typename Te>
bool ModelKNNInvtmp<Tr, Te>::shouldCache() const {
    return false;
}

template <typename Tr, typename Te>
void ModelKNNInvtmp<Tr, Te>::saveCache(std::ostream& os) const {
    os << invKnn << std::endl;
}

template <typename Tr, typename Te>
bool ModelKNNInvtmp<Tr, Te>::analyze(const entry::Entry<Te>& test) const {
    return invKnn.knn(test, k, norm);
}

template class ModelKNNInvtmp<SparseVector, SparseVector>;
template class ModelKNNInvtmp<Vector, Vector>;

/********** PCA+KNN model ************/

template <typename T>
ModelPCA<T>::ModelPCA(entry::SpEntries&& entries, int k, int alpha, Norm norm,
                      int nthreads)
    : PCTrans(entries, alpha, nthreads),
      analyzer(PCTrans.tcs(entries), k, norm){};

template <typename T>
ModelPCA<T>::ModelPCA(std::istream& is, int k, Norm norm) {
    is >> PCTrans;
    analyzer = T(is, k, norm);
}

template <typename T>
bool ModelPCA<T>::shouldCache() const {
    return true;
}

template <typename T>
void ModelPCA<T>::saveCache(std::ostream& os) const {
    os << PCTrans << std::endl;
    analyzer.saveCache(os);
}

template <typename T>
bool ModelPCA<T>::analyze(const entry::SpEntry& test) const {
    return analyzer.analyze(PCTrans.tc(test));
}

// Compilo ambos casos
template class ModelPCA<ModelKNNtmp<Vector, Vector>>;
template class ModelPCA<ModelKNNInvtmp<Vector, Vector>>;
