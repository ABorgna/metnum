#include "knn.h"

#include <queue>

// Squared L2 distance between two bag of words
long long distance2(const entry::Entry& a, const entry::Entry& b) {
    Eigen::SparseVector<int>::InnerIterator itA(a.bag_of_words);
    Eigen::SparseVector<int>::InnerIterator itB(b.bag_of_words);

    long long res = 0;
    while (itA and itB) {
        int ai = itA.value();
        int bi = itB.value();
        if (itA.index() < itB.index()) {
            res += ai * ai;
            ++itA;
        } else if (itA.index() == itB.index()) {
            res += (bi - ai) * (bi - ai);
            ++itA;
            ++itB;
        } else {
            res += bi * bi;
            ++itB;
        }
    }
    while (itA) {
        res += itA.value() * itA.value();
        ++itA;
    }
    while (itB) {
        res += itB.value() * itB.value();
        ++itB;
    }
    return res;
}

bool dumbKnn(const entry::Entries& entries, const entry::Entry& test, int k) {
    std::priority_queue<std::pair<double, bool>> queue;

    // Get the nearest k polarities
    for (const auto& e : entries) {
        long long dist = distance2(e, test);

        if (queue.size() < (size_t)k) {
            queue.emplace(dist, e.is_positive);
        } else if (queue.top().first > dist) {
            queue.pop();
            queue.emplace(dist, e.is_positive);
        }
    }

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
