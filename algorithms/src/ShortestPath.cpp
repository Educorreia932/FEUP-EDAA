#include "ShortestPath.h"

typedef DWGraph::node_t     node_t;
typedef DWGraph::weight_t   weight_t;

ShortestPath::~ShortestPath(){}

std::list<node_t> ShortestPath::getPath() const{
    std::list<node_t> res;
    node_t d = getDest();
    while(d != getStart()){
        res.push_front(d);
        d = getPrev(d);
    }
    res.push_front(d);
    return res;
}

ShortestPath::FromOneMany::FromOneMany(ShortestPathOneMany *oneMany_){
    this->oneMany = oneMany_;
}

void ShortestPath::FromOneMany::initialize(const DWGraph::DWGraph *G_, node_t s_, node_t d_){
    this->G = G_;
    this->s = s_;
    this->d = d_;
    oneMany->initialize(G, s);
}
void ShortestPath::FromOneMany::run(){
    oneMany->run();
}

node_t ShortestPath::FromOneMany::getStart() const{
    return s;
}

node_t ShortestPath::FromOneMany::getDest() const{
    return d;
}

node_t ShortestPath::FromOneMany::getPrev(node_t u) const {
    return oneMany->getPrev(u);
}

weight_t ShortestPath::FromOneMany::getPathWeight() const {
    return oneMany->getPathWeight(d);
}

bool ShortestPath::FromOneMany::hasVisited(DWGraph::node_t u) const{
    return oneMany->hasVisited(u);
}
