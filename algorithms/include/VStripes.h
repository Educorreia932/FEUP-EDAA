#pragma once

#include "ClosestPoint.h"

#include <queue>

class VStripes: public ClosestPoint {
private:
    std::vector<coord_t> v;

    const coord_t::deg_t d;
    coord_t::deg_t xMin;
    std::vector<std::vector<coord_t>> stripes;

    void checkStripe(const coord_t &p, size_t i, coord_t::deg_t &dBest, coord_t &cBest) const;
public:
    VStripes(coord_t::deg_t width);
    void initialize(const std::list<coord_t> &points);
    void run();
    coord_t getClosestPoint(coord_t p) const;
    std::pair<bool, coord_t> getClosestPoint_success(coord_t p) const;
};
