#pragma once

#include "MapMatchingMany.h"

#include "ClosestPointsInRadius.h"
#include "ShortestPathFew.h"
#include "ViterbiOptimized.h"

class HiddenMarkovModelMany: public MapMatchingMany {
private:
    ClosestPointsInRadius &closestPointsInRadius;
    const double d;
    const double sigma_z;
    const double beta;
    const size_t NUM_THREADS;
    const MapGraph *mapGraph;
    const std::vector<Trip> *trips;
    DWGraph::DWGraph distGraph;

    std::deque<std::list<Coord>*> observations;
    std::vector<std::set<DWGraph::node_t>> candidates;
    std::mutex candidatesMutex;

    static void candidatesWorker(HiddenMarkovModelMany *hmm, size_t idx);

    std::map<long long, std::vector<DWGraph::node_t>> matchesMap;
public:
    /**
     * @brief Construct a Hidden Markov Model.
     * 
     * @param closestPointsInRadius_ Calculates nearest points
     * @param d_                     Radius to search candidate states for (in degrees)
     * @param sigma_z_               Variance of the observations (in metres)
     */
    HiddenMarkovModelMany(
        ClosestPointsInRadius &closestPointsInRadius_,
        double d_, double sigma_z_, double beta_,
        size_t nThreads
    );
    virtual void initialize(const MapGraph *mapGraph_, const std::vector<Trip> &trips_);
    virtual void run();
    virtual std::vector<DWGraph::node_t> getMatches(long long tripId) const;
};
