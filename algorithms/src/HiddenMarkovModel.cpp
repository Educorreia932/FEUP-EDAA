#include "HiddenMarkovModel.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <map>

#include "AstarFew.h"
#include "DUGraph.h"
#include "Kosaraju.h"
#include "utils.h"
#include "ViterbiOptimized.h"

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

HiddenMarkovModel::HiddenMarkovModel(
    ClosestPointsInRadius &closestPointsInRadius_,
    ShortestPathFew &shortestPathFew_,
    double d_, double sigma_z_, double beta_
):
    closestPointsInRadius(closestPointsInRadius_),
    shortestPathFew(shortestPathFew_),
    d(d_), sigma_z(sigma_z_), beta(beta_)
{}

void HiddenMarkovModel::initialize(const MapGraph *mapGraph_){
    mapGraph = mapGraph_;

    cout << "Calculating SCC..." << endl;
    distGraph = mapGraph->getDistanceGraph();
    DUGraph duDistGraph = (DUGraph)distGraph;
    Kosaraju kosaraju;
    kosaraju.initialize(&duDistGraph);
    kosaraju.run();
    node_t root = kosaraju.get_scc(4523960191);
    for(const node_t &u: duDistGraph.getNodes()){
        if(kosaraju.get_scc(u) != root){
            distGraph.removeNode(u);
        }
    }
    cout << "Calculated SCC" << endl;

    auto nodes = distGraph.getNodes();
    list<Coord> l;
    for(const node_t &u: nodes) l.push_back(mapGraph->nodeToCoord(u));
    closestPointsInRadius.initialize(l, d);
}

void HiddenMarkovModel::run(){
    closestPointsInRadius.run();
    cout << "Idx\tID                \tVStripes (s)\tT\tK\tA* (s)   \tViterbi (s)\t" << endl;
}

HiddenMarkovModel::MyPi::MyPi(double sigma_z_, const vector<Coord> &S_, Coord firstObs):
sigma_z(sigma_z_), S(S_), zt(firstObs){}

double HiddenMarkovModel::MyPi::operator()(long i) const {
    const Coord &xti = S[i];
    double d = Coord::getDistanceArc(zt, xti);
    return exp(-0.5 * pow(d/sigma_z, 2))/(sqrt(2*M_PI) * sigma_z);
}

HiddenMarkovModel::MyA::MyA(double beta_, const vector<Coord> &Y_, const VVF &D_):
beta(beta_), Y(Y_), D(D_){}

double HiddenMarkovModel::MyA::operator()(long i, long j, long t) const {
    const Coord &zt0 = Y.at(t-1);
    const Coord &zt1 = Y.at(t);
    double dArc = Coord::getDistanceArc(zt0, zt1);

    const double &dRoute = D.at(i).at(j);

    double dt = fabs(dArc-dRoute);
    return exp(-dt/beta)/beta;
}

HiddenMarkovModel::MyB::MyB(double sigma_z_, const vector<Coord> &S_, const vector<Coord> &Y_):
sigma_z(sigma_z_), S(S_), Y(Y_){}

double HiddenMarkovModel::MyB::operator()(long i, long t) const {
    const Coord &zt = Y.at(t);
    const Coord &xti = S.at(i);
    double d = Coord::getDistanceArc(zt, xti);
    return exp(-0.5 * pow(d/sigma_z, 2))/(sqrt(2*M_PI) * sigma_z);
}

vector<node_t> HiddenMarkovModel::getMatches(const vector<Coord> &trip) const{
    vector<Coord> Y = trip;
    const size_t &T = Y.size();

    hrc::time_point begin, end; double dt;

    // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
    begin = hrc::now();
    map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
    vector<Coord> S;
    vector<node_t> idxToNode;
    vector<set<long>> candidateStates(T);
    for(size_t t = 0; t < T; ++t){
        vector<Coord> v = closestPointsInRadius.getClosestPoints(Y.at(t));
        if(v.empty()) throw std::runtime_error("Location t=" + to_string(t) + " has no candidates");
        for(const Coord &c: v){
            if(!Sv.count(c)){
                long id = Sv.size();
                Sv[c] = id;
                S.push_back(c);
                idxToNode.push_back(mapGraph->coordToNode(c));
            }
            candidateStates.at(t).insert(Sv.at(c));
        }
    }
    const size_t &K = Sv.size();

    end = hrc::now();
    dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << dt << "\t" << T << "\t" << K << "\t";

    // ======== DISTANCE MATRIX (A*) ========
    begin = hrc::now();
    VVF distMatrix(K, VF(K, fINF));
    for(size_t t = 0; t+1 < T; ++t){
        list<node_t> l;
        for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

        for(size_t i: candidateStates.at(t)){
            shortestPathFew.initialize(&distGraph, idxToNode.at(i), l);
            shortestPathFew.run();
            for(size_t j: candidateStates.at(t+1)){
                DWGraph::weight_t d = shortestPathFew.getPathWeight(idxToNode.at(j));
                double df = double(d)*MILLIMS_TO_METERS;
                distMatrix[i][j] = (d == iINF ? fINF : df);
            }
        }

        for(
            auto it = candidateStates.at(t+1).begin();
            it != candidateStates.at(t+1).end();
        ){
            size_t j = *it;

            double dBest = fINF;
            for(size_t i: candidateStates.at(t)){
                dBest = min(dBest, distMatrix[i][j]);
            }

            if(dBest >= fINF) it = candidateStates.at(t+1).erase(it);
            else ++it;
        }
    }
    end = hrc::now();
    dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << dt << "\t" << flush;
    
    // ======== HIDDEN MARKOV MODEL (VITERBI) ========
    MyPi Pi(sigma_z, S, Y[0]);
    MyA A(beta, Y, distMatrix);
    MyB B(sigma_z, S, Y);

    begin = hrc::now();
    ViterbiOptimized viterbi;
    viterbi.initialize(T, K, &Pi, &A, &B, candidateStates);
    viterbi.run();

    vector<long> v = viterbi.getLikeliestPath();
    end = hrc::now();
    dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << dt << "\t";

    cout << endl;

    // Final processing
    assert(v.size() == Y.size());
    
    vector<node_t> matches(v.size());
    for(size_t i = 0; i < v.size(); ++i){
        matches[i] = idxToNode[v[i]];
    }
    return matches;
}
