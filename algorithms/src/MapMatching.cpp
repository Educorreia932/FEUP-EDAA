#include "MapMatching.h"

#include "Kosaraju.h"

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

    cout << "Calculating SCC..." << endl;
    DWGraph::DWGraph distGraph = mapGraph->getDistanceGraph();
    DUGraph duDistGraph = (DUGraph)distGraph;
    Kosaraju kosaraju;
    kosaraju.initialize(&duDistGraph);
    kosaraju.run();
    node_t root = kosaraju.get_scc(4523960191);
    for(const node_t &u: duDistGraph.getNodes()){
        if(kosaraju.get_scc(u) != root){
            distGraph.removeNode(u);
        }
    }
    cout << "Calculated SCC" << endl;

    auto nodes = distGraph.getNodes();
    list<Vector2> l;
    for(const node_t &u: nodes) l.push_back(mapGraph->nodeToCoord(u));

    closestPoint = closestPointFactory.factoryMethod();
    closestPoint->initialize(l);
}

void MapMatching::FromClosestPoint::run(){
    closestPoint->run();
    cout << "Idx\tID                \t" << endl;
}

vector<node_t> MapMatching::FromClosestPoint::getMatches(
    const vector<Coord> &trip_
) const {
    vector<node_t> ret(trip_.size());
    for(size_t i = 0; i < trip_.size(); ++i){
        ret[i] = mapGraph->coordToNode(Coord(closestPoint->getClosestPoint(trip_[i])));
    }
    cout << endl;
    return ret;
}
