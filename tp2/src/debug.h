#pragma once

#include <iostream>

extern bool debugging_enabled;

#define DEBUG(x)                    \
    do {                            \
        if (debugging_enabled) {    \
            std::cerr << x << std::endl; \
        }                           \
    } while (0)
#define DEBUG_VAR(x)                                   \
    do {                                               \
        if (debugging_enabled) {                       \
            std::cerr << #x << ": " << (x) << std::endl; \
        }                                              \
    } while (0)
