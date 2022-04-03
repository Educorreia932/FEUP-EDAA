#pragma once

#include <vector>
#include "coord.h"

class Trip {
public:
    long long id;
    long long timestamp;
    std::vector<coord_t> coords;

    static std::vector<Trip> loadTrips(const std::string &filepath);
};
