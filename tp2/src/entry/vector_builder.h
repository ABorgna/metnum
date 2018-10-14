#ifndef VECTOR_BUILDER__H
#define VECTOR_BUILDER__H

#include "../files.h"
#include "reader.h"
#include "types.h"

#include <functional>

namespace entry {

VectorizedEntriesMap vectorizeMap(const FrecuencyVocabularyMap& vocab,
                                  const TokenizedEntriesMap&);

}

#endif  // VECTOR_BUILDER__H
