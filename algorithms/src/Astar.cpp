#include "Astar.h"

#include <queue>
#include <iostream>
#include <chrono>
#include <cassert>
#include "utils.h"

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

Astar::heuristic_t::~heuristic_t(){}

Astar::default_heuristic::default_heuristic(){}

const Astar::default_heuristic Astar::h_default;

DWGraph::weight_t Astar::default_heuristic::operator()(DWGraph::node_t) const{
    return 0;
}

Astar::Astar(const Astar::heuristic_t *h_){
    h = h_;
}

Astar::Astar():Astar(&h_default){}

void Astar::initialize(const DWGraph::DWGraph *G_, node_t s_, node_t d_){
    G = G_;
    s = s_;
    d = d_;
    dist.clear();
}

node_t Astar::getStart() const { return s; }

node_t Astar::getDest () const { return d; }

void Astar::run(){
    min_priority_queue Q;
    dist[s] = mk(0, -1); Q.push(mk((*h)(s), s));
    while(!Q.empty()){
        node_t u = Q.top().second;
        Q.pop();
        if(u == d) break;
        for(const Edge &e: G->getAdj(u)){
            weight_t c_ = dist.at(u).first + e.w;
            auto dit = dist.find(e.v);
            if(dit == dist.end() || c_ < dit->second.first){
                dist[e.v] = mk(c_, u);
                Q.push(mk(c_ + (*h)(e.v), e.v));
            }
        }
    }
}

node_t Astar::getPrev(node_t u) const{
    return dist.at(u).second;
}

weight_t Astar::getPathWeight() const{
    return dist.at(d).first;
    // if(dist.count(d)) return dist.at(d);
    // else return INF;
}

bool Astar::hasVisited(DWGraph::node_t u) const{
    return (dist.at(u).first != iINF);
}
