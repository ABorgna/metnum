#pragma once

#include <unordered_map>
#include <vector>

#include "../entry/types.h"

// The dumbest possible implementation for knn
template <typename TrainVector, typename TestVector>
bool dumbKnn(const entry::Entries<TrainVector>& entries,
             const entry::Entry<TestVector>& test, int k);

/* Inverted index KNN.
 *
 * For each word token we keep a vector with the entries which have a non-zero
 * value for it. When analyzing a new entry, we only compare it to other entries
 * that share at least one word with it.
 *
 * (TODO: Prove that this is valid)
 *
 * The idea was based on this youtube video (yay!)
 * https://www.youtube.com/watch?v=Mlp8hlKwETs
 * (TODO: Remove this comment before turning in the TP :) )
 */
template <typename TrainVector, typename TestVector>
class InvertedIndexKNN {
   private:
    const entry::Entries<TrainVector>&& entries;
    const int vocabSize;
    std::vector<std::vector<int>> invertedIndex;

    void precomputeInvIndex();

   public:
    InvertedIndexKNN(const entry::Entries<TrainVector>&&);
    bool knn(const entry::Entry<TestVector>& test, int k) const;
};
