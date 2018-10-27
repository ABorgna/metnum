#pragma once

#include <string.h>

#include "../entry/types.h"
#include "knn.h"
#include "pca.h"

template <typename TestVector>
class Model {
   public:
    virtual ~Model();

    // Indicates if this model should be cached.
    //
    // This should return false when it is cheap to retrain the model
    // and we can avoid storing the cache.
    virtual bool shouldCache() const = 0;

    // Store the trained model in a cache file
    virtual void saveCache(std::ostream&) const = 0;

    // Analize an entry and decide its polarity, using kNN(k).
    virtual bool analyze(const entry::Entry<TestVector>&) const = 0;
};

template <typename TrainVector, typename TestVector>
class ModelKNNtmp : public Model<TestVector> {
   private:
    entry::Entries<TrainVector> trainEntries;
    Vector sumVocab;
    int k;  // Number of neighbours to use with kNN
    Norm norm;

   public:
    ModelKNNtmp();
    ModelKNNtmp(entry::Entries<TrainVector>&&, int k, Norm);

    // Load the trained model from a cache file
    ModelKNNtmp(std::istream&, int k, Norm);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;
    bool shouldCache() const override;

    bool analyze(const entry::Entry<TestVector>&) const override;
};

typedef ModelKNNtmp<SparseVector, SparseVector> ModelKNN;

template <typename TrainVector, typename TestVector>
class ModelKNNInvtmp : public Model<TestVector> {
   private:
    InvertedIndexKNN<TrainVector, TestVector> invKnn;
    int k;  // Number of neighbours to use with kNN
    Norm norm;

   public:
    ModelKNNInvtmp();
    ModelKNNInvtmp(entry::Entries<TrainVector>&&, int k, Norm);

    // Load the trained model from a cache file
    ModelKNNInvtmp(std::istream&, int k, Norm);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;
    bool shouldCache() const override;

    bool analyze(const entry::Entry<TestVector>&) const override;
};

typedef ModelKNNInvtmp<SparseVector, SparseVector> ModelKNNInv;

// TODO Hacer que PCA sea parametrico en todo, no se si vale la pena
template <typename T>  // The kNN model to use
class ModelPCA : public Model<SparseVector> {
   private:
    PCA PCTrans;
    T analyzer;

   public:
    ModelPCA(entry::SpEntries&&, int k, int alpha, Norm, int nthreads = -1);

    // Load the trained model from a cache file
    ModelPCA(std::istream&, int k, Norm);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;
    bool shouldCache() const override;

    bool analyze(const entry::SpEntry&) const override;
};
