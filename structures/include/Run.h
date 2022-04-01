#pragma once

#include <vector>
#include "coord.h"

class Run {
public:
    long long id;
    long long timestamp;
    std::vector<coord_t> coords;

    static std::vector<Run> loadRuns(const std::string &filepath);
};
