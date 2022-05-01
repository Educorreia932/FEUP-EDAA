// #pragma GCC target("tune=native")

#include "Dijkstra.h"

#include <queue>
#include <utility>
#include <chrono>

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

Dijkstra::Dijkstra(weight_t dMax_):dMax(dMax_){}

node_t Dijkstra::getStart() const{
    return s;
}

void Dijkstra::initialize(const DWGraph::DWGraph *G_, DWGraph::node_t s_){
    this->s = s_;
    this->G = G_;
    dist.clear();
}

void Dijkstra::run(){
    min_priority_queue Q;
    dist[s] = mk(0, s); Q.push(mk(0, s));
    while(!Q.empty()){
        auto p = Q.top(); Q.pop(); //std::cout << "Processing " << p.first << ", " << p.second << ", dMax=" << dMax << std::endl;
        // if(p.first > dMax) break;
        node_t u = p.second; //std::cout << "Adj: " << G->getAdj(u).size() << std::endl;
        for(const Edge &e: G->getAdj(u)){
            weight_t c_ = dist.at(u).first + e.w;
            if(c_ > dMax) continue;
            auto dit = dist.find(e.v);
            if(dit == dist.end() || c_ < dit->second.first){
                dist[e.v] = mk(c_, u);
                Q.push(mk(c_, e.v));
            }
        }
    }
}

DWGraph::node_t Dijkstra::getPrev(DWGraph::node_t d) const{
    return dist.at(d).second;
}

weight_t Dijkstra::getPathWeight(node_t d) const{
    if(dist.count(d)) return dist.at(d).first;
    else return iINF;
}

bool Dijkstra::hasVisited(DWGraph::node_t u) const{
    return (dist.count(u) && dist.at(u).first != iINF);
}
