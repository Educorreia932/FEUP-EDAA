#pragma once

#include "ClosestPoint.h"

#include <queue>

class VStripes: public ClosestPoint {
private:
    std::vector<Vector2> v;

    const double d;
    double xMin;
    std::vector<std::vector<Vector2>> stripes;

    void checkStripe(const Vector2 &p, size_t i, double &dBest, Vector2 &cBest) const;
public:
    VStripes(double width);
    void initialize(const std::list<Vector2> &points);
    void run();
    Vector2 getClosestPoint(Vector2 p) const;
    std::pair<bool, Vector2> getClosestPoint_success(Vector2 p) const;
};
