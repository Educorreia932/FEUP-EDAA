#include "AstarFew.h"

#include <queue>
#include <iostream>
#include <chrono>
#include <cassert>

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

const double INF = 1000000000.0;

AstarFew::default_heuristic::default_heuristic(){}

const AstarFew::default_heuristic AstarFew::h_default;

DWGraph::weight_t AstarFew::default_heuristic::operator()(DWGraph::node_t) const{
    return 0;
}

AstarFew::AstarFew(const AstarFew::heuristic_t *h_, weight_t dMax_):h(h_), dMax(dMax_){}

AstarFew::AstarFew(const AstarFew::heuristic_t *h_):AstarFew(h_, INF){}

AstarFew::AstarFew():AstarFew(&h_default){}

void AstarFew::initialize(const DWGraph::DWGraph *G_, node_t s_, list<node_t> d_){
    G = G_;
    s = s_;
    d = d_;
    dist.clear();
}

node_t AstarFew::getStart() const { return s; }

list<node_t> AstarFew::getDest () const { return d; }

void AstarFew::run(){
    unordered_set<node_t> dS(d.begin(), d.end());
    min_priority_queue Q;
    dist[s] = mk(0, -1); Q.push(mk((*h)(s), s));
    while(!Q.empty()){
        pair<weight_t, node_t> p =  Q.top(); Q.pop();
        if(p.first > dMax) break;
        const node_t &u = p.second;
        
        auto uit = dS.find(u);
        if(uit != dS.end()) dS.erase(uit);
        if(dS.empty()) break;
        
        for(const Edge &e: G->getAdj(u)){
            auto uit = dist.find(u);
            weight_t c_ = (uit != dist.end() ? uit->second.first : DWGraph::INF) + e.w;
            auto dit = dist.find(e.v);
            if(dit == dist.end() || c_ < dit->second.first){
                dist[e.v] = mk(c_, u);
                Q.push(mk(c_ + (*h)(e.v), e.v));
            }
        }
    }

    for(const node_t &u: dS){
        if(dist.count(u) == 0)
            dist[u] = mk(INF, -1);
    }
}

node_t AstarFew::getPrev(node_t u) const{
    return dist.at(u).second;
}

weight_t AstarFew::getPathWeight(node_t u) const{
    return dist.at(u).first;
}

bool AstarFew::hasVisited(DWGraph::node_t u) const{
    return (dist.at(u).first != DWGraph::INF);
}
