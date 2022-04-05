#include "MapOsmView.h"

#include <mapbox/earcut.hpp>

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

using Point = std::array<float, 2>;

static const Color buildingOutlineColor(196, 182, 171);

static const size_t N_CIRCLE = 8;

MapOsmView::MapOsmView(RenderTarget &window_, MapView &mapView_, const MapGraph &graph_, const vector<polygon_t> &polygons_):
    window(window_), mapView(mapView_),
    mapTerrainOsmView(window_, mapView_, polygons_),
    mapBuildingsOsmView(window_, mapView_, polygons_),
    mapGraphOsmView(window_, mapView_, graph_)
{}

void MapOsmView::draw(){
    mapTerrainOsmView.draw();
    mapBuildingsOsmView.draw();
    mapGraphOsmView.draw();
}
