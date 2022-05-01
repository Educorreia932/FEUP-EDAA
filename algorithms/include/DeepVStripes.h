#pragma once

#include "ClosestPoint.h"

#include <queue>
#include "VStripes.h"

class DeepVStripes: public ClosestPoint {
private:
    std::vector<VStripes> vstripes_vtr;
public:
    DeepVStripes(double width, size_t nLevels);
    void initialize(const std::list<Vector2> &points);
    void run();
    Vector2 getClosestPoint(Vector2 p) const;
};
