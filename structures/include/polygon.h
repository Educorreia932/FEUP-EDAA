#pragma once

#include <vector>

#include "coord.h"

struct polygon_t {
    enum type : char {
        WATER = 'w'
    };

    long long id;
    std::vector<coord_t> coords;
    type t;

    static std::vector<polygon_t> loadPolygons(const std::string &filePath);
};
