#pragma once

#include "MapMatching.h"

#include "ClosestPointsInRadius.h"
#include "ShortestPath.h"

class HiddenMarkovModel: public MapMatching {
private:
    ClosestPointsInRadius &closestPointsInRadius;
    ShortestPath &shortestPath;
    const double d;
    const double sigma_z;
    const MapGraph *mapGraph;
public:
    /**
     * @brief Construct a Hidden Markov Model.
     * 
     * @param closestPointsInRadius_ Calculates nearest points
     * @param d_                     Radius to search candidate states for (in degrees)
     * @param sigma_z_               Variance of the observations (in metres)
     */
    HiddenMarkovModel(ClosestPointsInRadius &closestPointsInRadius_, ShortestPath &shortestPath_, double d_, double sigma_z_);
    virtual void initialize(const MapGraph *mapGraph_);
    virtual void run();
    virtual std::vector<DWGraph::node_t> getMatches(const std::vector<Coord> &trip) const;
};
