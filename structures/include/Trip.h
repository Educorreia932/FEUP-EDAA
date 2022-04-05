#pragma once

#include <vector>
#include "Coord.h"

class Trip {
public:
    long long id;
    long long timestamp;
    std::vector<Coord> coords;

    static std::vector<Trip> loadTrips(const std::string &filepath);
};
