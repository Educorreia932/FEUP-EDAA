#pragma once

#include "Coord.h"

#include <string>
#include <vector>

class Trip {
public:
    long long id;
    long long timestamp;
    std::vector<Coord> coords;

    static std::vector<Trip> loadTrips(const std::string &filepath);
};
