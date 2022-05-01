#include "ShortestPathFew.h"

typedef DWGraph::node_t     node_t;
typedef DWGraph::weight_t   weight_t;

ShortestPathFew::~ShortestPathFew(){}

std::list<node_t> ShortestPathFew::getPath(node_t u) const{
    std::list<node_t> res;
    node_t d = u;
    while(d != getStart()){
        res.push_front(d);
        d = getPrev(d);
    }
    res.push_front(d);
    return res;
}

ShortestPathFew::FromAll::FromAll(ShortestPathAll &shortestPathAll_):
shortestPathAll(shortestPathAll_)
{}

void ShortestPathFew::FromAll::initialize(const DWGraph::DWGraph *G, DWGraph::node_t s_, std::list<DWGraph::node_t> d){
    shortestPathAll.initialize(G);
    s = s_;
}

void ShortestPathFew::FromAll::run(){
    shortestPathAll.run();
}

DWGraph::node_t ShortestPathFew::FromAll::getStart() const{
    throw std::runtime_error("ShortestPathFew::FromAll::getStart is not implemented");
}

std::list<DWGraph::node_t> ShortestPathFew::FromAll::getDest () const{
    throw std::runtime_error("ShortestPathFew::FromAll::getDest is not implemented");
}

DWGraph::node_t ShortestPathFew::FromAll::getPrev(DWGraph::node_t d) const{
    return shortestPathAll.getPrev(s, d);
}

DWGraph::weight_t ShortestPathFew::FromAll::getPathWeight(DWGraph::node_t d) const{
    return shortestPathAll.getPathWeight(s, d);
}

bool ShortestPathFew::FromAll::hasVisited(DWGraph::node_t u) const{
    throw std::runtime_error("ShortestPathFew::FromAll::hasVisited is not implemented");
}
