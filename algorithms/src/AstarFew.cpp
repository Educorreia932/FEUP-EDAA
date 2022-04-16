#include "AstarFew.h"

#include <queue>
#include <iostream>
#include <chrono>
#include <cassert>

#include "utils.h"

using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef DWGraph::Edge Edge;
template<class K, class V> using umap = std::unordered_map<K, V>;
typedef umap<node_t, weight_t> dist_t;
typedef umap<node_t, node_t  > prev_t;
typedef std::priority_queue<std::pair<weight_t, node_t>,
                std::vector<std::pair<weight_t, node_t>>,
               std::greater<std::pair<weight_t, node_t>>> min_priority_queue;
typedef std::chrono::high_resolution_clock hrc;
#define mk(a, b) (std::make_pair((a), (b)))

struct AstarFewHeuristic : public Astar::heuristic_t {
private:
    const unordered_map<node_t, Coord> &nodesToCoords;
    Coord c;
    double r;
    double factor;
public:
    AstarFewHeuristic(
        const unordered_map<node_t, Coord> &nodesToCoords_,
        Coord c_,
        double r_,
        double factor_
    ):nodesToCoords(nodesToCoords_), c(c_),r(r_),factor(factor_){}
    virtual weight_t operator()(node_t u) const {
        const Coord &coord = nodesToCoords.at(u);
        double d = max(0.0, Coord::getDistanceArc(c, coord) - r);
        return weight_t(d * factor);
    }
};

AstarFew::AstarFew(
    const unordered_map<node_t, Coord> &nodesToCoords_,
    double factor_,
    weight_t dMax_
):
    nodesToCoords(nodesToCoords_),
    factor(factor_),
    dMax(dMax_)
{}

void AstarFew::initialize(const DWGraph::DWGraph *G_, node_t s_, list<node_t> d_){
    G = G_;
    s = s_;
    d = d_;
    dist.clear();

    double latMin = +fINF, latMax = -fINF, 
           lonMin = +fINF, lonMax = -fINF;
    for(const node_t &u: d){
        const Coord &coord = nodesToCoords.at(u);
        latMin = min(latMin, coord.lat());
        latMax = max(latMax, coord.lat());
        lonMin = min(lonMin, coord.lon());
        lonMax = max(lonMax, coord.lon());
    }
    Coord c = Coord((latMin+latMax)/2.0, (lonMin+lonMax)/2.0);

    double r = 0.0;
    for(const node_t &u: d){
        const Coord &coord = nodesToCoords.at(u);
        r = max(r, Coord::getDistanceArc(coord, c));
    }

    delete h;
    h = new AstarFewHeuristic(nodesToCoords, c, r, factor);
}

node_t AstarFew::getStart() const { return s; }

list<node_t> AstarFew::getDest () const { return d; }

void AstarFew::run(){
    unordered_set<node_t> dS(d.begin(), d.end());
    min_priority_queue Q;
    dist[s] = mk(0, -1); Q.push(mk((*h)(s), s));
    while(!Q.empty()){
        pair<weight_t, node_t> p =  Q.top(); Q.pop();
        const node_t &u = p.second;
        
        auto uit = dS.find(u);
        if(uit != dS.end()) dS.erase(uit);
        if(dS.empty()) break;
        
        for(const Edge &e: G->getAdj(u)){
            weight_t c_ = dist.at(u).first + e.w;
            if(c_ > dMax) continue;
            auto dit = dist.find(e.v);
            if(dit == dist.end() || c_ < dit->second.first){
                dist[e.v] = mk(c_, u);
                Q.push(mk(c_ + (*h)(e.v), e.v));
            }
        }
    }

    for(const node_t &u: dS){
        if(dist.count(u) == 0)
            dist[u] = mk(iINF, -1);
    }
}

node_t AstarFew::getPrev(node_t u) const{
    return dist.at(u).second;
}

weight_t AstarFew::getPathWeight(node_t u) const{
    return dist.at(u).first;
}

bool AstarFew::hasVisited(DWGraph::node_t u) const{
    return (dist.at(u).first != iINF);
}
