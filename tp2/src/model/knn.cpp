#include "knn.h"

#include <queue>
#include <utility>

// Squared L2 distance between two bag of words
long long distance2(const entry::Entry& a, const entry::Entry& b) {
    Eigen::SparseVector<int>::InnerIterator it1(a.bag_of_words);
    Eigen::SparseVector<int>::InnerIterator it2(b.bag_of_words);
    long long res = 0;

    while (it1 and it2) {
        if (it1.index() > it2.index())
            std::swap(it1, it2);
        int x = it1.value();
        int y = it2.value();

        if (it1.index() < it2.index()) {
            res += x * x;
            ++it1;
        } else {
            res += (y - x) * (y - x);
            ++it1;
            ++it2;
        }
    }
    if (not it1)
        std::swap(it1, it2);
    do {
        res += it1.value() * it1.value();
    } while (++it1);
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
