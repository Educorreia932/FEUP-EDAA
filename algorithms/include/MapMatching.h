#pragma once

#include <list>
#include <vector>

#include "ClosestPoint.h"

class MapMatching {
public:
    virtual void initialize(
        const std::list<Vector2> &points,
        const std::vector<Vector2> &trip
    ) = 0;
    virtual void run() = 0;
    virtual std::list<Vector2> getMatches() = 0;

    class FromClosestPoint;
};

class MapMatching::FromClosestPoint: public MapMatching {
private:
    ClosestPointFactory &closestPointFactory;
    std::list<Vector2> points;
    std::vector<Vector2> trip;
    std::list<Vector2> matches;
public:
    FromClosestPoint(ClosestPointFactory &closestPointFactory_);

    virtual void initialize(
        const std::list<Vector2> &points_,
        const std::vector<Vector2> &trip_
    );

    virtual void run();

    virtual std::list<Vector2> getMatches();
};

