#pragma once

#include "ClosestPointsInRadius.h"

class VStripesRadius: public ClosestPointsInRadius {
private:
    std::vector<Vector2> v;

    double d;
    double xMin;
    std::vector<std::vector<Vector2>> stripes;

    void checkStripe(const Vector2 &p, size_t i, std::vector<Vector2> &sols) const;
public:
    void initialize(const std::list<Vector2> &points, double width);
    void run();
    std::vector<Vector2> getClosestPoints(Vector2 p) const;
};
