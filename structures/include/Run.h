#pragma once

#include "coord.h"

class Run {
public:
    long long id;
    long long timestamp;
    std::list<coord_t> coords;
};
