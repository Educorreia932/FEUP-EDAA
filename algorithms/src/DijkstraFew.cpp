// #pragma GCC target("tune=native")

#include "DijkstraFew.h"

#include <queue>
#include <utility>
#include <chrono>
#include <exception>

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

DijkstraFew::DijkstraFew(weight_t dMax_):dMax(dMax_){}

node_t DijkstraFew::getStart() const{
    return s;
}

list<node_t> DijkstraFew::getDest () const{
    return d;
}

void DijkstraFew::initialize(const DWGraph::DWGraph *G_, DWGraph::node_t s_, list<node_t> d_){
    this->s = s_;
    this->d = d_;
    this->G = G_;
    dist.clear();
}

void DijkstraFew::run(){
    unordered_set<node_t> dS(d.begin(), d.end());
    min_priority_queue Q;
    dist[s] = 0; Q.push(mk(0, s));
    while(!Q.empty()){
        auto p = Q.top(); Q.pop();
        node_t u = p.second;

        auto uit = dS.find(u);
        if(uit != dS.end()) dS.erase(uit);
        if(dS.empty()) break;

        for(const Edge &e: G->getAdj(u)){
            weight_t c_ = dist.at(u) + e.w;
            if(c_ > dMax) continue;
            auto dit = dist.find(e.v);
            if(dit == dist.end() || c_ < dit->second){
                dist[e.v] = c_;
                Q.push(mk(c_, e.v));
            }
        }
    }
}

DWGraph::node_t DijkstraFew::getPrev(DWGraph::node_t d) const{
    throw logic_error("DijkstraFew::getPrev is not implemented");
}

weight_t DijkstraFew::getPathWeight(node_t d) const{
    if(dist.count(d)) return dist.at(d);
    else return iINF;
}

bool DijkstraFew::hasVisited(DWGraph::node_t u) const{
    auto it = dist.find(u);
    return (it != dist.end());
}
