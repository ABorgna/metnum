#pragma once

#include "entry/types.h"

// The dumbest possible implementation for knn
bool dumbKnn(const entry::VectorizedEntriesMap& entries,
             const entry::VectorizedEntry& test, int k);
