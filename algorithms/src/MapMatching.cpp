#include "MapMatching.h"

using namespace std;

MapMatching::FromClosestPoint::FromClosestPoint(ClosestPointFactory &closestPointFactory_):
    closestPointFactory(closestPointFactory_)
{}

void MapMatching::FromClosestPoint::initialize(
    const list<Coord> &points_
){
    delete closestPoint; closestPoint = nullptr;
    list<Vector2> l; for(const Coord &c: points_) l.push_back(c);
    closestPoint = closestPointFactory.factoryMethod();
    closestPoint->initialize(l);
}

void MapMatching::FromClosestPoint::run(){
    closestPoint->run();
}

vector<Coord> MapMatching::FromClosestPoint::getMatches(
    const vector<Coord> &trip_
) const {
    vector<Coord> ret(trip_.size());
    for(size_t i = 0; i < trip_.size(); ++i){
        ret[i] = Coord(closestPoint->getClosestPoint(trip_[i]));
    }
    return ret;
}
