#pragma once

#include <list>
#include <vector>

#include "ClosestPoint.h"
#include "Coord.h"
#include "MapGraph.h"

class MapMatching {
public:
    virtual void initialize(const MapGraph *mapGraph) = 0;
    virtual void run() = 0;
    virtual std::vector<DWGraph::node_t> getMatches(const std::vector<Coord> &trip) const = 0;

    class FromClosestPoint;
};

class MapMatching::FromClosestPoint: public MapMatching {
private:
    ClosestPointFactory &closestPointFactory;
    ClosestPoint *closestPoint = nullptr;
    const MapGraph *mapGraph;
public:
    FromClosestPoint(ClosestPointFactory &closestPointFactory_);

    virtual void initialize(const MapGraph *mapGraph_);

    virtual void run();

    virtual std::vector<DWGraph::node_t> getMatches(const std::vector<Coord> &trip_) const;
};

