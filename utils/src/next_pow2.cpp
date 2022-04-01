#include "next_pow2.h"

uint32_t next_pow2(uint32_t x) {
    return x == 1 ? 1 : 1<<(32-__builtin_clz(x-1));
}
