#include "Astar.h"

#include <queue>
#include <iostream>
#include <chrono>

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
    this->h = h_;
}

Astar::Astar():Astar(&h_default){}

void Astar::initialize(const DWGraph::DWGraph *G_, node_t s_, node_t d_){
    this->G = G_;
    this->s = s_;
    this->d = d_;
    dist.clear();
    hdist.clear();
    prev.clear();
    for(const node_t &u: G->getNodes()){
        dist[u] = DWGraph::INF;
        hdist[u] = DWGraph::INF;
        prev[u] = DWGraph::INVALID_NODE;
    }
}

node_t Astar::getStart() const { return s; }

node_t Astar::getDest () const { return d; }

void Astar::run(){
    min_priority_queue Q;
    dist[s] = 0; hdist[s] = (*h)(s); Q.push(mk(hdist[s], s));
    while(!Q.empty()){
        node_t u = Q.top().second;
        Q.pop();
        if(u == d) break;
        for(Edge e: G->getAdj(u)){
            weight_t c_ = dist[u] + e.w;
            if(c_ < dist[e.v]){
                dist[e.v] = c_;
                hdist[e.v] = c_ + (*h)(e.v);
                prev[e.v] = u;
                Q.push(mk(hdist[e.v], e.v));
            }
        }
    }
}

node_t Astar::getPrev(node_t u) const{
    return prev.at(u);
}

weight_t Astar::getPathWeight() const{
    return dist.at(d);
}

bool Astar::hasVisited(DWGraph::node_t u) const{
    return (dist.at(u) != DWGraph::INF);
}
