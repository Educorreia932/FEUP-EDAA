#include "MapTripsView.h"

#include "LineShape.h"

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

static const int width = 2;
static const Color color = Color(255, 0, 0, 16);

MapTripsView::MapTripsView(RenderTarget &window_, MapView &mapView_, const vector<Trip> &trips_):
    window(window_), mapView(mapView_), trips(trips_)
{
    refresh();
}

void MapTripsView::refresh(){
    zip.clear();

    for(const Trip &trip: trips){
        if(trip.coords.size() < 2) continue;
        auto it1 = trip.coords.begin(),
             it2 = ++trip.coords.begin();
        while(it2 != trip.coords.end()){
            sf::Vector2f u = mapView.coordToVector2f(*(it1++)),
                         v = mapView.coordToVector2f(*(it2++));
            FullLineShape e(u, v, width);
            e.setFillColor(color);
            for(size_t i = 0; i < e.getVertexCount(); ++i)
                zip.push_back(e[i]);
        }
    }
}

void MapTripsView::draw(){
    window.draw(&zip[0], zip.size(), Quads);
}
