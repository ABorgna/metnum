#pragma once

#include <string.h>
//#include <Eigen/SparseCore>

#include "../entry/types.h"
#include "knn.h"

class Model {
   public:
    virtual ~Model();
    // Analize an entry and decide its polarity, using kNN(k).
    virtual bool analize(const entry::Entry&) const = 0;
};

class ModelKNN : public Model {
   private:
    InvertedIndexKNN invKnn;
    int k;  // Number of neighbours to use with kNN

   public:
    ModelKNN(entry::Entries&&, int k);
    bool analize(const entry::Entry&) const override;
};

class ModelPCA : public Model {
   private:
    entry::Entries trainEntries;
    int k;
    int alpha;

   public:
    ModelPCA(entry::Entries&&, int k, int alpha);
    bool analize(const entry::Entry&) const override;
};
