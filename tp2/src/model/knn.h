#pragma once

#include <unordered_map>
#include <vector>

#include "../entry/types.h"

// The dumbest possible implementation for knn
template <typename TrainVector, typename TestVector>
bool dumbKnn(const entry::Entries<TrainVector>& entries,
             const entry::Entry<TestVector>& test, int k,
             const Vector& sumVocab);

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
class InvertedIndexKNN;

// Input/output operators
// (This has to be defined before the class because reasons)
template <typename TrainVector, typename TestVector>
std::ostream& operator<<(std::ostream&,
                         const InvertedIndexKNN<TrainVector, TestVector>&);
template <typename TrainVector, typename TestVector>
std::istream& operator>>(std::istream&,
                         InvertedIndexKNN<TrainVector, TestVector>&);

template <typename TrainVector, typename TestVector>
class InvertedIndexKNN {
   private:
    entry::Entries<TrainVector> entries;
    int vocabSize = 0;
    std::vector<std::vector<int>> invertedIndex;
    Vector sumVocab;

    void precomputeInvIndex();

   public:
    InvertedIndexKNN();
    InvertedIndexKNN(const entry::Entries<TrainVector>&&);

    bool knn(const entry::Entry<TestVector>& test, int k) const;

    friend std::ostream& operator<<<>(
        std::ostream&, const InvertedIndexKNN<TrainVector, TestVector>&);
    friend std::istream& operator>>
        <>(std::istream&, InvertedIndexKNN<TrainVector, TestVector>&);
};
