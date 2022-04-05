#include "MapTripMatchView.h"

#include <mapbox/earcut.hpp>

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

MapTripMatchView::MapTripMatchView(RenderTarget &window_, MapView &mapView_):
    window(window_), mapView(mapView_){}

MapTripMatchView::MapTripMatchView(RenderTarget &window_, MapView &mapView_, const Trip &trip_, const vector<Coord> &matches_):
    window(window_), mapView(mapView_), trip(trip_), matches(matches_)
{
    refresh();
}

void MapTripMatchView::setTripMatches(const Trip &trip_, const vector<Coord> &matches_){
    trip = trip_;
    matches = matches_;
}

void MapTripMatchView::refresh(){
    zip.clear();

    auto it1 = trip.coords.begin(),
         it2 = ++trip.coords.begin();
    while(it2 != trip.coords.end()){
        sf::Vector2f u = mapView.coordToVector2f(*(it1++)),
                     v = mapView.coordToVector2f(*(it2++));
        FullLineShape e(u, v, 2);
        e.setFillColor(Color::Blue);
        for(size_t i = 0; i < e.getVertexCount(); ++i)
            zip.push_back(e[i]);
    }
    for(size_t i = 0; i < trip.coords.size(); ++i){
        sf::Vector2f u = mapView.coordToVector2f(trip.coords[i]),
                     v = mapView.coordToVector2f(matches[i]);
        DashedLineShape e(u, v, 1);
        e.setFillColor(Color::Blue);
        for(size_t i = 0; i < e.getVertexCount(); ++i)
            zip.push_back(e[i]);
    }
}

void MapTripMatchView::draw(){
    window.draw(&zip[0], zip.size(), Quads);
}
