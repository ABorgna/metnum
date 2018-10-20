#pragma once

#include <string.h>
//#include <Eigen/SparseCore>

#include "../entry/types.h"
#include "knn.h"

class Model {
   public:
    virtual ~Model();
    // Analize an entry and decide its polarity, using kNN(k).
    virtual bool analize(const entry::SpEntry&) const = 0;
};

class ModelKNN : public Model {
   private:
    InvertedIndexKNN invKnn;
    int k;  // Number of neighbours to use with kNN

   public:
    ModelKNN(entry::SpEntries&&, int k);
    bool analize(const entry::SpEntry&) const override;
};

class ModelPCA : public Model {
   private:
    entry::SpEntries trainEntries;
    int k;
    int alpha;

   public:
    ModelPCA(entry::SpEntries&&, int k, int alpha);
    bool analize(const entry::SpEntry&) const override;
};
