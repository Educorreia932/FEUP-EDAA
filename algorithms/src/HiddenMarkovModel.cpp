#include "HiddenMarkovModel.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <map>

#include "Astar.h"
#include "Viterbi.h"

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;
typedef std::vector<VVF> VVVF;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

typedef DWGraph::node_t node_t;

using hrc = std::chrono::high_resolution_clock;

const double NANOS_TO_SECS = (1.0/1000000000.0);
const double MILLIMS_TO_METERS = (1.0/1000.0);
const double METERS_TO_MILLIMS = 1000.0;

const double INF = 1000000000.0;

HiddenMarkovModel::HiddenMarkovModel(
    ClosestPointsInRadius &closestPointsInRadius_,
    double d_, double sigma_z_, double beta_
):
    closestPointsInRadius(closestPointsInRadius_),
    d(d_), sigma_z(sigma_z_), beta(beta_)
{}

void HiddenMarkovModel::initialize(const MapGraph *mapGraph_){
    mapGraph = mapGraph_;
    auto nodes = mapGraph->getNodes();
    list<Vector2> l;
    for(const auto &p: nodes) l.push_back(p.second);
    closestPointsInRadius.initialize(l, d);

    distGraph = mapGraph->getDistanceGraph();
}

void HiddenMarkovModel::run(){
    closestPointsInRadius.run();
}

struct MyPi : public Viterbi::InitialProbabilitiesGenerator {
private:
    const double sigma_z;
    const vector<Coord> &S;
    const Coord zt;
public:
    MyPi(double sigma_z_, const vector<Coord> &S_, Coord firstObs):
        sigma_z(sigma_z_), S(S_), zt(firstObs){}
    virtual double operator()(long i) const {
        const Coord &xti = S[i];
        double d = Coord::getDistanceArc(zt, xti);
        return exp(-0.5 * pow(d/sigma_z, 2))/(sqrt(2*M_PI) * sigma_z);
    }
};

struct MyA : public Viterbi::TransitionMatrixGenerator {
private:
    const double beta;
    const vector<Coord> &Y;
    const VVF &D;
public:
    MyA(double beta_, const vector<Coord> &Y_, const VVF &D_):
        beta(beta_), Y(Y_), D(D_){}
    virtual double operator()(long i, long j, long t) const {
        const Coord &zt0 = Y[t];
        const Coord &zt1 = Y[t+1];
        double dArc = Coord::getDistanceArc(zt0, zt1);

        const double &dRoute = D[i][j];

        double dt = abs(dArc-dRoute);
        return exp(-dt/beta)/beta;
    }
};

struct MyB : public Viterbi::EmissionMatrixGenerator {
private:
    const double sigma_z;
    const vector<Coord> &S;
    const vector<Coord> &Y;
public:
    MyB(double sigma_z_, const vector<Coord> &S_, const vector<Coord> &Y_):
        sigma_z(sigma_z_), S(S_), Y(Y_){}
    virtual double operator()(long i, long t) const {
        const Coord &zt = Y[t];
        const Coord &xti = S[i];
        double d = Coord::getDistanceArc(zt, xti);
        return exp(-0.5 * pow(d/sigma_z, 2))/(sqrt(2*M_PI) * sigma_z);
    }
};

vector<node_t> HiddenMarkovModel::getMatches(const vector<Coord> &trip) const{
    const vector<Coord> &Y = trip;
    const size_t &T = Y.size();

    map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
    vector<Coord> S;
    vector<node_t> idxToNode;
    vector<list<long>> candidateStates(trip.size());
    for(size_t t = 0; t < T; ++t){
        vector<Vector2> v = closestPointsInRadius.getClosestPoints(trip[t]);
        for(const Vector2 &c: v){
            if(!Sv.count(Coord(c))){
                long id = Sv.size();
                Sv[Coord(c)] = id;
                S.push_back(Coord(c));
                idxToNode.push_back(mapGraph->coordToNode(Coord(c)));
            }
            candidateStates[t].push_back(Sv.at(Coord(c)));
        }
    }

    const size_t &K = Sv.size();
    
    // Now I have Y[t] and S[i],
    // and I need to obtain Pi[i], A[i,j,t] and B[i,t]

    cout << "T=" << T << ", K=" << K << endl;

    cout << "Creating distance matrix..." << endl;
    auto begin = hrc::now();
    const std::unordered_map<DWGraph::node_t, Coord> &nodes = mapGraph->getNodes();
    VVF distMatrix(K, VF(K, INF));
    for(size_t t = 0; t+1 < T; ++t){
        cout << "t=" << t
             << ", candidateStates.at(t).size()=" << candidateStates.at(t).size()
             << endl;
        for(size_t j: candidateStates.at(t+1)){
            cout << "  j=" << j << endl;
            MapGraph::DistanceHeuristic h(nodes, nodes.at(idxToNode.at(j)), METERS_TO_MILLIMS);
            // Astar astar(&h);
            Astar astar;
            for(size_t i: candidateStates.at(t)){
                cout << "    t=" << t << ", "
                     << "j=" << j << "(" << nodes.at(idxToNode.at(j)).y << "/" << nodes.at(idxToNode.at(j)).x << "), "
                     << "i=" << i << "(" << nodes.at(idxToNode.at(i)).y << "/" << nodes.at(idxToNode.at(i)).x << ")"
                     << "/" << idxToNode.size() << endl;
                astar.initialize(&distGraph, idxToNode.at(i), idxToNode.at(j)); cout << "L146" << endl;
                astar.run(); cout << "L147, K=" << K << endl;
                distMatrix[i][j] =
                    double(astar.getPathWeight())
                    *MILLIMS_TO_METERS; cout << "L148" << endl;
            }
        }
    }
    auto end = hrc::now();
    double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << "Created distance matrix, took " << dt << "s" << endl;
    
    MyPi Pi(sigma_z, S, Y[0]);
    MyA A(beta, Y, distMatrix);
    MyB B(sigma_z, S, Y);

    Viterbi viterbi;
    viterbi.initialize(T, K, &Pi, &A, &B);
    viterbi.run();

    vector<long> v = viterbi.getLikeliestPath();
    
    vector<node_t> matches(v.size());
    for(size_t i = 0; i < v.size(); ++i){
        matches[i] = idxToNode[v[i]];
    }
    return matches;

    return vector<node_t>();
}
