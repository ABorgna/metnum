#include "knn.h"

#include <queue>
#include <utility>

#include "../debug.h"

typedef std::priority_queue<std::pair<double, bool>> NeighQueue;

// L1 distance between two bag of words
double distance1(const entry::Entry& a, const entry::Entry& b) {
    Eigen::SparseVector<double>::InnerIterator it1(a.bag_of_words);
    Eigen::SparseVector<double>::InnerIterator it2(b.bag_of_words);
    double res = 0;

    while (it1 and it2) {
        if (it1.index() > it2.index())
            std::swap(it1, it2);
        if (it1.index() < it2.index()) {
            res += it1.value();
            ++it1;
        } else {
            res += abs(it2.value() - it1.value());
            ++it1;
            ++it2;
        }
    }
    if (not it1)
        std::swap(it1, it2);
    do {
        res += it1.value();
    } while (++it1);
    return res;
}

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

bool dumbKnn(const entry::Entries& entries, const entry::Entry& test, int k) {
    NeighQueue queue;

    // Get the nearest k polarities
    for (const auto& e : entries) {
        double dist = distance1(e, test);
        pushIfBetter(queue, k, dist, e.is_positive);
    }

    return decideFromQueue(queue);
}

/*********** Inverted Index Knn ******************/

InvertedIndexKNN::InvertedIndexKNN(const entry::Entries&& entries)
    : entries(std::move(entries)), vocabSize(entries[0].bag_of_words.size()) {
    precomputeInvIndex();
};

void InvertedIndexKNN::precomputeInvIndex() {
    invertedIndex.resize(vocabSize);

    for (size_t i = 0; i < entries.size(); i++) {
        const auto& e = entries[i];

        // Note: The vector invertedIndex[i] must remain sorted.
        for (Eigen::SparseVector<double>::InnerIterator it(e.bag_of_words); it;
             ++it) {
            if (it.value() > 0)
                invertedIndex[it.index()].push_back(i);
        }
    }
}

// TODO: Document this better if we actually use it
bool InvertedIndexKNN::knn(const entry::Entry& testEntry, int k) const {
    // Queue with (num entry, word number, entry position for number in the
    // inverted array)
    typedef std::tuple<int, int, size_t> QueueItem;
    std::priority_queue<QueueItem, std::vector<QueueItem>,
                        std::greater<QueueItem>>
        entriesQueue;

    // Get the nearest k polarities
    for (Eigen::SparseVector<double>::InnerIterator it(testEntry.bag_of_words); it;
         ++it) {
        const auto& wordVec = invertedIndex[it.index()];
        if (wordVec.size() > 0) {
            entriesQueue.emplace(wordVec[0], it.index(), 0);
        }
    }

    NeighQueue neighQueue;

    while (not entriesQueue.empty()) {
        int entryId, word;
        size_t posInInvArray;
        std::tie(entryId, word, posInInvArray) = entriesQueue.top();

        const auto& entry = entries[entryId];
        double dist = distance1(entry, testEntry);
        pushIfBetter(neighQueue, k, dist, testEntry.is_positive);

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
