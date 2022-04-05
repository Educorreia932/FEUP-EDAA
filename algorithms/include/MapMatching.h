#pragma once

#include <list>
#include <vector>

#include "ClosestPoint.h"

class MapMatching {
public:
    virtual void initialize(const std::list<Vector2> &points) = 0;
    virtual void run() = 0;
    virtual std::vector<Vector2> getMatches(const std::vector<Vector2> &trip) const = 0;

    class FromClosestPoint;
};

class MapMatching::FromClosestPoint: public MapMatching {
private:
    ClosestPointFactory &closestPointFactory;
    ClosestPoint *closestPoint = nullptr;
public:
    FromClosestPoint(ClosestPointFactory &closestPointFactory_);

    virtual void initialize(const std::list<Vector2> &points_);

    virtual void run();

    virtual std::vector<Vector2> getMatches(const std::vector<Vector2> &trip_) const;
};

