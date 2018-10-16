#include "knn.h"

#include <queue>

// Squared L2 distance between two bag of words
double distance2(const entry::Entry& a, const entry::Entry& b) {
    auto itA = a.bag_of_words.begin();
    auto itB = b.bag_of_words.begin();
    auto aEnd = a.bag_of_words.end();
    auto bEnd = b.bag_of_words.end();

    double res = 0;
    for(;itA != aEnd and itB != bEnd; itA++, itB++) {
        res += (*itB - *itA) * (*itB - *itA);
    }
    return res;
}

bool dumbKnn(const entry::Entries& entries,
             const entry::Entry& test, int k) {

    std::priority_queue<std::pair<double, bool>> queue;

    // Get the nearest k polarities
    for(const auto& e : entries) {
        double dist = distance2(e, test);

        if(queue.size() < (size_t)k) {
            queue.emplace(dist, e.is_positive);
        } else if(queue.top().first > dist) {
            queue.pop();
            queue.emplace(dist, e.is_positive);
        }
    }

    // Polarity count, negative is negative
    int count = 0;
    bool last = false;
    while(not queue.empty()) {
        last = queue.top().second;
        count += last ? 1 : -1;
        queue.pop();
    }

    return count > 0 or (count == 0 and last);
}
