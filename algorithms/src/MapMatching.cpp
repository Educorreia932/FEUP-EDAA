#include "MapMatching.h"

using namespace std;

MapMatching::FromClosestPoint::FromClosestPoint(ClosestPointFactory &closestPointFactory_):
    closestPointFactory(closestPointFactory_)
{}

void MapMatching::FromClosestPoint::initialize(
    const list<Vector2> &points_
){
    delete closestPoint; closestPoint = nullptr;
    closestPoint = closestPointFactory.factoryMethod();
    closestPoint->initialize(points_);
}

void MapMatching::FromClosestPoint::run(){
    closestPoint->run();
}

vector<Vector2> MapMatching::FromClosestPoint::getMatches(
    const vector<Vector2> &trip_
) const {
    vector<Vector2> ret(trip_.size());
    for(size_t i = 0; i < trip_.size(); ++i){
        ret[i] = closestPoint->getClosestPoint(trip_[i]);
    }
    return ret;
}
