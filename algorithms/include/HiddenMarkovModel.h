#pragma once

#include "MapMatching.h"

#include "ClosestPointsInRadius.h"
#include "ShortestPathFew.h"
#include "ViterbiOptimized.h"

class HiddenMarkovModel: public MapMatching {
private:
    ClosestPointsInRadius &closestPointsInRadius;
    ShortestPathFew &shortestPathFew;
    const double d;
    const double sigma_z;
    const double beta;
    const MapGraph *mapGraph;
    DWGraph::DWGraph distGraph;
public:
    /**
     * @brief Construct a Hidden Markov Model.
     * 
     * @param closestPointsInRadius_ Calculates nearest points
     * @param d_                     Radius to search candidate states for (in degrees)
     * @param sigma_z_               Variance of the observations (in metres)
     */
    HiddenMarkovModel(
        ClosestPointsInRadius &closestPointsInRadius_,
        ShortestPathFew &shortestPathFew_,
        double d_, double sigma_z_, double beta_
    );
    virtual void initialize(const MapGraph *mapGraph_);
    virtual void run();
    virtual std::vector<DWGraph::node_t> getMatches(const std::vector<Coord> &trip) const;

    struct MyPi : public Viterbi::InitialProbabilitiesGenerator {
    private:
        const double sigma_z;
        const std::vector<Coord> &S;
        const Coord zt;
    public:
        MyPi(double sigma_z_, const std::vector<Coord> &S_, Coord firstObs);
        virtual double operator()(long i) const;
    };

    struct MyA : public Viterbi::TransitionMatrixGenerator {
    private:
        const double beta;
        const std::vector<Coord> &Y;
        const std::vector<std::vector<double>> &D;
    public:
        MyA(double beta_, const std::vector<Coord> &Y_, const std::vector<std::vector<double>> &D_);
        virtual double operator()(long i, long j, long t) const;
    };

    struct MyB : public Viterbi::EmissionMatrixGenerator {
    private:
        const double sigma_z;
        const std::vector<Coord> &S;
        const std::vector<Coord> &Y;
    public:
        MyB(double sigma_z_, const std::vector<Coord> &S_, const std::vector<Coord> &Y_);
        virtual double operator()(long i, long t) const;
    };
};
