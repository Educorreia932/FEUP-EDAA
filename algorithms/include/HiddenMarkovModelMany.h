#pragma once

#include "MapMatchingMany.h"

#include "ClosestPointsInRadius.h"
#include "DijkstraDist.h"
#include "ShortestPathFew.h"
#include "utils.h"
#include "ViterbiOptimized.h"

class HiddenMarkovModelMany: public MapMatchingMany {
private:
    ClosestPointsInRadius &closestPointsInRadius;
    const double d;
    const double sigma_z;
    const double beta;
    const MapGraph *mapGraph;
    const std::vector<Trip> *trips;
    DWGraph::DWGraph distGraph;

    utils::ThreadPool threadPool;

    std::deque<std::list<Coord>*> observations;
    std::vector<std::set<DWGraph::node_t>> candidates;
    std::mutex candidatesMutex;
    static void candidatesWorker(HiddenMarkovModelMany *hmm, size_t idx);

    std::map<DWGraph::node_t, DijkstraDist> dijkstras;

    class TripTask: public utils::ThreadPool::Task {
    friend HiddenMarkovModelMany;
    private:
        const HiddenMarkovModelMany &hmm;
        const Trip &trip;
        const size_t index;
        bool success = false;
        std::vector<DWGraph::node_t> matches;
    public:
        TripTask(
            const HiddenMarkovModelMany &hmm_,
            const Trip &trip_,
            size_t index_
        );
        virtual ~TripTask();
        const Trip &getTrip() const;
        void run();
        bool succeeded() const;
        const std::vector<DWGraph::node_t> &getMatches() const;
    };

    std::map<long long, TripTask*> tripTasks;
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
    ~HiddenMarkovModelMany();
    virtual void initialize(const MapGraph *mapGraph_, const std::vector<Trip> &trips_);
    virtual void run();
    virtual const std::vector<DWGraph::node_t> &getMatches(long long tripId) const;
};
