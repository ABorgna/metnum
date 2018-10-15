#pragma once

#include <string.h>
//#include <Eigen/SparseCore>

#include "entry/types.h"
#include "files.h"

class Model {
   public:
    virtual ~Model();
    // Analize an entry and decide its polarity, using kNN(k).
    virtual bool analize(const entry::VectorizedEntry&) const = 0;
};

class ModelKNN : public Model {
   private:
    entry::VectorizedEntriesMap trainEntries;
    int k;  // Number of neighbours to use with kNN

   public:
    ModelKNN(entry::VectorizedEntriesMap&&, int k);
    bool analize(const entry::VectorizedEntry&) const override;
};

class ModelPCA : public Model {
   private:
    entry::VectorizedEntriesMap trainEntries;
    int k;
    int alpha;

   public:
    ModelPCA(entry::VectorizedEntriesMap&&, int k, int alpha);
    bool analize(const entry::VectorizedEntry&) const override;
};
