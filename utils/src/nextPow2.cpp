#include "nextPow2.h"

int utils::nextPow2(unsigned long long x) {
    return x == 1 ? 1 : 1 << (8*sizeof(x) - __builtin_clzll(x-1));
}
