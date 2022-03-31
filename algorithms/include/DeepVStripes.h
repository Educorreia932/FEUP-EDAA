#pragma once

#include "ClosestPoint.h"

#include <queue>
#include "VStripes.h"

class DeepVStripes: public ClosestPoint {
private:
    std::vector<VStripes> vstripes_vtr;
public:
    DeepVStripes(coord_t::deg_t width, size_t nLevels);
    void initialize(const std::list<coord_t> &points);
    void run();
    coord_t getClosestPoint(coord_t p) const;
};
