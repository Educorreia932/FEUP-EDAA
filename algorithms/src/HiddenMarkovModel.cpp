#include "HiddenMarkovModel.h"

#include <cmath>
#include <iostream>
#include <map>

#include "Viterbi.h"

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;
typedef std::vector<VVF> VVVF;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

typedef DWGraph::node_t node_t;

HiddenMarkovModel::HiddenMarkovModel(
    ClosestPointsInRadius &closestPointsInRadius_,
    ShortestPath &shortestPath_,
    double d_, double sigma_z_, double beta_
):
    closestPointsInRadius(closestPointsInRadius_),
    shortestPath(shortestPath_),
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

    cout << "T=" << T << ", K=" << K << endl;;
    
    VVF B(K, VF(T, 0.0));
    for(size_t t = 0; t < T; ++t){
        for(const long &i: candidateStates[t]){
            const Coord &zt = Y[t];
            const Coord &xti = S[i];
            double d = Coord::getDistanceArc(zt, xti);
            B[i][t] = exp(-0.5 * pow(d/sigma_z, 2))/(sqrt(2*M_PI) * sigma_z);
        }
    }

    VF Pi = B[0];

    VVVF A(K, VVF(K, VF(T-1)));
    for(size_t i = 0; i < K; ++i){
        for(size_t j = 0; j < K; ++j){
            for(size_t t = 0; t+1 < T; ++t){
                const Coord &zt0 = Y[t];
                const Coord &zt1 = Y[t+1];
                double dArc = Coord::getDistanceArc(zt0, zt1);

                shortestPath.initialize(&distGraph, idxToNode[i], idxToNode[j]);
                shortestPath.run();
                double dRoute = shortestPath.getPathWeight();

                double dt = abs(dArc-dRoute);
                A[i][j][t] = exp(-dt/beta)/beta;
            }
        }
    }

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
