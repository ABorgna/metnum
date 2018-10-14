#include "vector_builder.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "../debug.h"

namespace entry {

bool vectorize(const FrecuencyVocabularyMap& vocab,
               const TokenizedEntry& entry,
               VectorizedEntry& res) {
    res.is_positive = entry.is_positive;
    res.bag_of_words = std::vector<double>(vocab.size(), 0);

    // Check if the entry was completely filtered from the vocabulary
    bool allFiltered = true;

    // Find the index in the bag of words for each token in the entry
    for(int tokenId : entry.tokens) {
        const VocabToken searchToken = {tokenId, 0.};
        const auto it = lower_bound(vocab.begin(), vocab.end(), searchToken); 
        // If the token was not filtered from the vocabulary, get its index
        if(it != vocab.end()) {
            const int index = it - vocab.begin();
            res.bag_of_words[index]++;
            allFiltered = false;
        }
    }

    return not allFiltered;
}

VectorizedEntriesMap vectorizeMap(const FrecuencyVocabularyMap& vocab,
                                  const TokenizedEntriesMap& entries) {
    DEBUG("Vectorizing the entries.");
    VectorizedEntriesMap res;
    res.reserve(entries.size());

    std::for_each(entries.begin(), entries.end(), [&res, &vocab](auto node) {
        VectorizedEntry vec;
        bool notEmpty = vectorize(vocab, node.second, vec);
        if (notEmpty)
            res.emplace(node.first, std::move(vec));
    });

    return res;
}

}  // namespace entry
