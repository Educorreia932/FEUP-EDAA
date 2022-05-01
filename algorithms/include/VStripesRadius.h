#pragma once

#include "ClosestPointsInRadius.h"

class VStripesRadius: public ClosestPointsInRadius {
private:
    std::vector<Coord> v;
    double d;

    double xMin;
    std::vector<std::vector<Coord>> stripes;

    void checkStripe(const Coord &p, size_t i, std::vector<Coord> &sols) const;
public:
    void initialize(const std::list<Coord> &points, double width);
    void run();
    std::vector<Coord> getClosestPoints(Coord p) const;
};
