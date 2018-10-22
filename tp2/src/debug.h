#pragma once

#include <iostream>

extern bool debugging_enabled;

#define DEBUG(x)                    \
    do {                            \
        if (debugging_enabled) {    \
            std::cerr << x << std::endl; \
        }                           \
    } while (0)
#define DEBUG_IDENT(x, ident)             \
    do {                            \
        int a = ident;              \
        while (a--){            \
            std::cerr << "    ";    \
        }                           \
        DEBUG(x);                    \
    }while (0)
#define DEBUG_PROG(x)                    \
    do {                            \
        if (debugging_enabled) {    \
            std::cerr << x << '\r' << std::flush; \
        }                           \
    } while (0)
#define DEBUG_IDENT_PROG(x, ident)             \
    do {                            \
        int a = ident;              \
        while (a--){            \
            std::cerr << "    ";    \
        }                           \
        DEBUG_PROG(x);                    \
    }while (0)
#define DEBUG_VAR(x)                                   \
    do {                                               \
        if (debugging_enabled) {                       \
            std::cerr << #x << ": " << (x) << std::endl; \
        }                                              \
    } while (0)
