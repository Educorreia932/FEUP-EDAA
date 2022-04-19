#include "DijkstraOnRequest.h"

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;

using namespace std;

const double METERS_TO_MILLIMS = 1000.0;

void DijkstraOnRequest::initialize(const DWGraph::DWGraph *G_){
    G = G_;
}

void DijkstraOnRequest::run(){}

node_t DijkstraOnRequest::getPrev(node_t s, node_t d) const{
    throw logic_error("DijkstraOnRequest::getPrev is not implemented");
}

weight_t DijkstraOnRequest::getPathWeight(node_t s, node_t d) const{
    if(dijkstras.count(s) == 0){
        dijkstras.emplace(s, 650*METERS_TO_MILLIMS);
        dijkstras.at(s).initialize(G, s);
        dijkstras.at(s).run();
    }
    return dijkstras.at(s).getPathWeight(d);
}
