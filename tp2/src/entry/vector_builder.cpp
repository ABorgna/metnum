#include "vector_builder.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "../debug.h"

namespace entry {

bool vectorizeEntry(const Vocabulary& vocab, const TokenizedEntry& entry,
                    Entry& res) {
    res.id = entry.id;
    res.is_positive = entry.is_positive;
    res.bag_of_words = Eigen::SparseVector<double>(vocab.size());

    // Count the number of non-filtered tokens
    int numWords = 0;

    // Find the index in the bag of words for each token in the entry
    for (int tokenId : entry.tokens) {
        const VocabToken searchToken = {token : tokenId, 0};
        const auto it = lower_bound(vocab.begin(), vocab.end(), searchToken);
        // If the token was not filtered from the vocabulary, get its index
        if (it != vocab.end() && it->token == tokenId) {
            const int index = it - vocab.begin();
            res.bag_of_words.coeffRef(index)++;
            numWords++;
        }
    }

    const bool allFiltered = numWords == 0;
    if (allFiltered)
        return false;

    // Normalize the vector (for norm-1)
    for (Eigen::SparseVector<double>::InnerIterator it(res.bag_of_words); it;
         ++it) {
        it.valueRef() /= numWords;
    }

    return true;
}

Entries vectorize(const Vocabulary& vocab, const TokenizedEntries& entries) {
    DEBUG("Vectorizing the entries.");
    Entries res;
    res.reserve(entries.size());

    std::for_each(entries.begin(), entries.end(),
                  [&res, &vocab](const auto& node) {
                      Entry vec;
                      bool notEmpty = vectorizeEntry(vocab, node, vec);
                      if (notEmpty)
                          res.push_back(std::move(vec));
                  });

    return res;
}

}  // namespace entry
