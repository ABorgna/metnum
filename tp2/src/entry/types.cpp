#include "types.h"

namespace entry {

bool operator<(const VocabToken& x, const VocabToken& y) {
    return x.token < y.token;
}

}  // namespace entry
