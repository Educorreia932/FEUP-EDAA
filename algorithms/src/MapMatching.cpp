#include "MapMatching.h"

using namespace std;

typedef DWGraph::node_t node_t;

MapMatching::FromClosestPoint::FromClosestPoint(ClosestPointFactory &closestPointFactory_):
    closestPointFactory(closestPointFactory_)
{}

void MapMatching::FromClosestPoint::initialize(
    const MapGraph *mapGraph_
){
    mapGraph = mapGraph_;
    delete closestPoint; closestPoint = nullptr;

    auto nodes = mapGraph_->getNodes();
    list<Vector2> l;
    for(const auto &p: nodes) l.push_back(p.second);

    closestPoint = closestPointFactory.factoryMethod();
    closestPoint->initialize(l);
}

void MapMatching::FromClosestPoint::run(){
    closestPoint->run();
}

vector<node_t> MapMatching::FromClosestPoint::getMatches(
    const vector<Coord> &trip_
) const {
    vector<node_t> ret(trip_.size());
    for(size_t i = 0; i < trip_.size(); ++i){
        ret[i] = mapGraph->coordToNode(Coord(closestPoint->getClosestPoint(trip_[i])));
    }
    return ret;
}
