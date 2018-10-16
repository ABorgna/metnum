#pragma once

#include <unordered_map>
#include <vector>

#include "../entry/types.h"

// The dumbest possible implementation for knn
bool dumbKnn(const entry::Entries& entries,
             const entry::Entry& test, int k);

/*
class InvertedIndexKNN() {
  private:
    entry::Entries&& entries;
    vector<int, vector<int>> invertedIndex;
  public:
    InvertedIndexKNN(const entry::Entries&&);
}
*/
