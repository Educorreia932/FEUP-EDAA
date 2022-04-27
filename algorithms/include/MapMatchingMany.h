#pragma once

#include <list>
#include <vector>

#include "MapGraph.h"
#include "Trip.h"

class MapMatchingMany {
public:
    virtual void initialize(const MapGraph *mapGraph, const std::vector<Trip> &trips) = 0;
    virtual void run() = 0;
    virtual std::vector<DWGraph::node_t> getMatches(long long tripId) const = 0;

    class FromClosestPoint;
};
