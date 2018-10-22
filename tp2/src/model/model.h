#pragma once

#include <string.h>

#include "../entry/types.h"
#include "knn.h"
#include "pca.h"

template <typename TestVector>
class Model {
   public:
    virtual ~Model();

    // Store the trained model in a cache file
    virtual void saveCache(std::ostream&) const = 0;

    // Analize an entry and decide its polarity, using kNN(k).
    virtual bool analize(const entry::Entry<TestVector>&) const = 0;
};

template <typename TrainVector, typename TestVector>
class ModelKNNtmp : public Model<TestVector> {
   private:
    entry::Entries<TrainVector> trainEntries;
    int k;  // Number of neighbours to use with kNN

   public:
    ModelKNNtmp();
    ModelKNNtmp(entry::Entries<TrainVector>&&, int k);

    // Load the trained model from a cache file
    ModelKNNtmp(std::istream&, int k);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;

    bool analize(const entry::Entry<TestVector>&) const override;
};

typedef ModelKNNtmp<SparseVector, SparseVector> ModelKNN;

template <typename TrainVector, typename TestVector>
class ModelKNNInvtmp : public Model<TestVector> {
   private:
    InvertedIndexKNN<TrainVector, TestVector> invKnn;
    int k;  // Number of neighbours to use with kNN

   public:
    ModelKNNInvtmp();
    ModelKNNInvtmp(entry::Entries<TrainVector>&&, int k);

    // Load the trained model from a cache file
    ModelKNNInvtmp(std::istream&, int k);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;

    bool analize(const entry::Entry<TestVector>&) const override;
};

typedef ModelKNNInvtmp<SparseVector, SparseVector> ModelKNNInv;

// TODO Hacer que PCA sea parametrico en todo, no se si vale la pena
template <typename T>  // The kNN model to use
class ModelPCA : public Model<SparseVector> {
   private:
    PCA PCTrans;
    T analyzer;

   public:
    ModelPCA(entry::SpEntries&&, int k, int alpha);

    // Load the trained model from a cache file
    ModelPCA(std::istream&, int k);

    // Store the trained model in a cache file
    void saveCache(std::ostream&) const override;

    bool analize(const entry::SpEntry&) const override;
};
