#pragma once

#include "Coord.h"

#include <string>
#include <vector>

struct polygon_t {
    enum type : char {
        WATER = 'w',
        LAND  = 'l',
        BUILDING = 'b'
    };

    long long id;
    std::vector<Coord> coords;
    type t;

    static std::vector<polygon_t> loadPolygons(const std::string &filePath);
};
