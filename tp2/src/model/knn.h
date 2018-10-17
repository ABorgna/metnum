#pragma once

#include <unordered_map>
#include <vector>

#include "../entry/types.h"

// The dumbest possible implementation for knn
bool dumbKnn(const entry::Entries& entries,
             const entry::Entry& test, int k);

/* Inverted index KNN.
 *
 * For each word token we keep a vector with the entries which have a non-zero value for it.
 * When analyzing a new entry, we only compare it to other entries that share at least one word with it.
 *
 * (TODO: Prove that this is valid)
 *
 * The idea was based on this youtube video (yay!)
 * https://www.youtube.com/watch?v=Mlp8hlKwETs
 * (TODO: Remove this comment before turning in the TP :) )
 */
class InvertedIndexKNN {
  private:
    const entry::Entries&& entries;
    const int vocabSize;
    std::vector<std::vector<int>> invertedIndex;

    void precomputeInvIndex();
  public:
    InvertedIndexKNN(const entry::Entries&&);
    bool knn(const entry::Entry& test, int k) const;
};
