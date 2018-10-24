#include "knn.h"

#include <functional>
#include <queue>
#include <utility>

#include "../debug.h"

typedef std::priority_queue<std::pair<double, bool>> NeighQueue;

void pushIfBetter(NeighQueue& queue, double k, double newDist,
                  bool positivity) {
    if (queue.size() < (size_t)k) {
        queue.emplace(newDist, positivity);
    } else if (queue.top().first > newDist) {
        queue.pop();
        queue.emplace(newDist, positivity);
    }
}

bool decideFromQueue(NeighQueue& queue) {
    // Polarity count, negative is negative
    int count = 0;
    bool last = false;
    while (not queue.empty()) {
        last = queue.top().second;
        count += last ? 1 : -1;
        queue.pop();
    }

    return count > 0 or (count == 0 and last);
}

/*********** DumbKnn ******************/

template <typename V, typename W>
bool dumbKnn(const entry::Entries<V>& entries, const entry::Entry<W>& test,
             int k, Norm norm, const Vector& sumEntries) {
    NeighQueue queue;

    // Get the nearest k polarities
    for (const auto& e : entries) {
        double dist;
        if (norm == NORM_CHI2)
            dist = distanciaChi2(e.bag_of_words, test.bag_of_words, sumEntries);
        else
            dist = distanciaN(e.bag_of_words, test.bag_of_words, norm);
        pushIfBetter(queue, k, dist, e.is_positive);
    }

    return decideFromQueue(queue);
}

/*********** Inverted Index Knn ******************/

// Default constructor
template <typename V, typename W>
InvertedIndexKNN<V, W>::InvertedIndexKNN() : entries(), vocabSize(0){};

template <typename V, typename W>
InvertedIndexKNN<V, W>::InvertedIndexKNN(const entry::Entries<V>&& entries)
    : entries(entries) {
    if (entries.size() > 0) {
        vocabSize = entries[0].bag_of_words.size();
        sumVocab = entry::sumEntries(entries);
    }
    precomputeInvIndex();
};

template <typename V, typename W>
void InvertedIndexKNN<V, W>::precomputeInvIndex() {
    invertedIndex.resize(vocabSize);

    for (size_t i = 0; i < entries.size(); i++) {
        const auto& e = entries[i];

        // Note: The vector invertedIndex[i] must remain sorted.
        traverseVector(e.bag_of_words,
                       [this, i](size_t word, double frequency) {
                           if (frequency > 0)
                               invertedIndex[word].push_back(i);
                       });
    }
}

// TODO: Document this better if we actually use it
template <typename V, typename W>
bool InvertedIndexKNN<V, W>::knn(const entry::Entry<W>& testEntry, int k,
                                 Norm norm) const {
    // Queue with (num entry, word number, entry position for number in the
    // inverted array)
    typedef std::tuple<int, int, size_t> QueueItem;
    std::priority_queue<QueueItem, std::vector<QueueItem>,
                        std::greater<QueueItem>>
        entriesQueue;

    // Get the nearest k polarities
    traverseVector(testEntry.bag_of_words,
                   [this, &entriesQueue](size_t word, double) {
                       const auto& wordVec = invertedIndex[word];
                       if (wordVec.size() > 0) {
                           entriesQueue.emplace(wordVec[0], word, 0);
                       }
                   });

    NeighQueue neighQueue;

    while (not entriesQueue.empty()) {
        int entryId, word;
        size_t posInInvArray;
        std::tie(entryId, word, posInInvArray) = entriesQueue.top();

        const auto& entry = entries[entryId];
        double dist;
        if (norm == NORM_CHI2)
            dist = distanciaChi2(entry.bag_of_words, testEntry.bag_of_words,
                                 sumVocab);
        else
            dist = distanciaN(entry.bag_of_words, testEntry.bag_of_words, norm);
        pushIfBetter(neighQueue, k, dist, entry.is_positive);

        while (not entriesQueue.empty() and
               std::get<0>(entriesQueue.top()) == entryId) {
            entriesQueue.pop();
        }
        if (invertedIndex[word].size() > posInInvArray + 1) {
            entriesQueue.emplace(invertedIndex[word][posInInvArray + 1], word,
                                 posInInvArray + 1);
        }
    }

    return decideFromQueue(neighQueue);
}

// We use the same tag no matter the instantiation of V.
// This is not optimal, but meh.
template <typename TrainVector, typename TestVector>
std::ostream& operator<<(std::ostream& os,
                         const InvertedIndexKNN<TrainVector, TestVector>& knn) {
    std::tuple<entry::Entries<TrainVector>, int, std::vector<std::vector<int>>,
               Vector>
        tup(knn.entries, knn.vocabSize, knn.invertedIndex, knn.sumVocab);
    writeNamedTuple(os, "InvertedIndexKNN", tup);
    return os;
}

template <typename TrainVector, typename TestVector>
std::istream& operator>>(std::istream& is,
                         InvertedIndexKNN<TrainVector, TestVector>& knn) {
    std::tuple<entry::Entries<TrainVector>, int, std::vector<std::vector<int>>,
               Vector>
        tup;
    readNamedTuple(is, "InvertedIndexKNN", tup);
    std::tie(knn.entries, knn.vocabSize, knn.invertedIndex, knn.sumVocab) = tup;

    // Invalid data
    if (knn.vocabSize <= 0) {
        throw std::invalid_argument("Error while parsing InvertedIndexKNN");
    }

    return is;
}

// Explicit instantiations for the exported functions
template bool dumbKnn<SparseVector, SparseVector>(
    const entry::Entries<SparseVector>&, const entry::Entry<SparseVector>&, int,
    Norm, const Vector&);
template bool dumbKnn<SparseVector, Vector>(const entry::Entries<SparseVector>&,
                                            const entry::Entry<Vector>&, int,
                                            Norm, const Vector&);
template bool dumbKnn<Vector, Vector>(const entry::Entries<Vector>&,
                                      const entry::Entry<Vector>&, int, Norm,
                                      const Vector&);

template class InvertedIndexKNN<SparseVector, SparseVector>;
template class InvertedIndexKNN<Vector, SparseVector>;
template class InvertedIndexKNN<Vector, Vector>;

template std::ostream& operator<<(
    std::ostream& os, const InvertedIndexKNN<SparseVector, SparseVector>& knn);
template std::ostream& operator<<(
    std::ostream& os, const InvertedIndexKNN<Vector, SparseVector>& knn);
template std::ostream& operator<<(std::ostream& os,
                                  const InvertedIndexKNN<Vector, Vector>& knn);

template std::istream& operator>>(
    std::istream& is, InvertedIndexKNN<SparseVector, SparseVector>& knn);
template std::istream& operator>>(std::istream& is,
                                  InvertedIndexKNN<Vector, SparseVector>& knn);
template std::istream& operator>>(std::istream& is,
                                  InvertedIndexKNN<Vector, Vector>& knn);
