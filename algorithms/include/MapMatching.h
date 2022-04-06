#pragma once

#include <list>
#include <vector>

#include "ClosestPoint.h"
#include "Coord.h"

class MapMatching {
public:
    virtual void initialize(const std::list<Coord> &points) = 0;
    virtual void run() = 0;
    virtual std::vector<Coord> getMatches(const std::vector<Coord> &trip) const = 0;

    class FromClosestPoint;
};

class MapMatching::FromClosestPoint: public MapMatching {
private:
    ClosestPointFactory &closestPointFactory;
    ClosestPoint *closestPoint = nullptr;
public:
    FromClosestPoint(ClosestPointFactory &closestPointFactory_);

    virtual void initialize(const std::list<Coord> &points_);

    virtual void run();

    virtual std::vector<Coord> getMatches(const std::vector<Coord> &trip_) const;
};

