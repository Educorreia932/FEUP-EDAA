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
    static void storeTripsBin(const std::vector<Trip> &trips, const std::string &filepath);
    static std::vector<Trip> loadTripsBin(const std::string &filepath);
};
