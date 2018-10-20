#ifndef VECTOR_BUILDER__H
#define VECTOR_BUILDER__H

#include "../files.h"
#include "reader.h"
#include "types.h"

#include <functional>

namespace entry {

SpEntries vectorize(const Vocabulary& vocab,
                                const TokenizedEntries&);
}

#endif  // VECTOR_BUILDER__H
