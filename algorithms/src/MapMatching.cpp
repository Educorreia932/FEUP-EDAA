#include "MapMatching.h"

using namespace std;

MapMatching::FromClosestPoint::FromClosestPoint(ClosestPointFactory &closestPointFactory_):
    closestPointFactory(closestPointFactory_)
{}

void MapMatching::FromClosestPoint::initialize(
    const list<Vector2> &points_,
    const vector<Vector2> &trip_
){
    points = points_;
    trip = trip_;
    matches.clear();
}

void MapMatching::FromClosestPoint::run(){
    for(size_t i = 0; i < trip.size(); ++i){
        ClosestPoint *closestPoint = closestPointFactory.factoryMethod();
        closestPoint->initialize(points);
        matches.push_back(closestPoint->getClosestPoint(trip[i]));
        delete closestPoint;
    }
}

list<Vector2> MapMatching::FromClosestPoint::getMatches(){
    return matches;
}
