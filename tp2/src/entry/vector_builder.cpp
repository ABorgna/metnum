#include "vector_builder.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../debug.h"

namespace entry {

bool vectorizeEntry(const Vocabulary& vocab, const TokenizedEntry& entry,
                    Entry& res) {
    res.id = entry.id;
    res.is_positive = entry.is_positive;

    std::map<size_t, double> bag_of_words;

    // Count the number of non-filtered tokens
    int numWords = 0;

    // Find the index in the bag of words for each token in the entry
    for (int tokenId : entry.tokens) {
        const VocabToken searchToken = {token : tokenId, 0};
        const auto it = lower_bound(vocab.begin(), vocab.end(), searchToken);
        // If the token was not filtered from the vocabulary, get its index
        if (it != vocab.end() && it->token == tokenId) {
            const int index = it - vocab.begin();
            bag_of_words[index]++;
            numWords++;
        }
    }

    const bool allFiltered = numWords == 0;
    if (allFiltered)
        return false;

    // Normalize the vector (for norm-1)
    for (auto& p : bag_of_words) {
        p.second /= numWords;
    }

    res.bag_of_words = SparseVector(bag_of_words, vocab.size());

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
