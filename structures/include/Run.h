#pragma once

#include "coord.h"

class Run {
public:
    long long id;
    long long timestamp;
    std::vector<coord_t> coords;
};
