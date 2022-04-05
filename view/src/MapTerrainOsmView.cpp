#include "MapTerrainOsmView.h"

#include <mapbox/earcut.hpp>

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

using Point = std::array<float, 2>;

static const Color buildingOutlineColor(196, 182, 171);

static const size_t N_CIRCLE = 8;

MapTerrainOsmView::MapTerrainOsmView(RenderTarget &window_, MapView &mapView_, const vector<polygon_t> &polygons_):
    window(window_), mapView(mapView_), polygons(polygons_)
{
    refresh();
}

void MapTerrainOsmView::refresh(){
    for(const polygon_t &polygon: polygons){
        if(polygon.coords.size() < 2) continue;
        if(polygon.t != polygon_t::type::WATER && polygon.t != polygon_t::type::LAND) continue;

        Color color;
        switch(polygon.t){
            case polygon_t::type::WATER   : color = Color(170, 211, 223); break;
            case polygon_t::type::LAND    : color = Color(242, 239, 233); break;
            default                       : color = Color(  0,   0,   0); break;
        }
        vector<vector<Point>> p(1);
        for(const Coord &c: polygon.coords){
            auto xy = mapView.coordToVector2f(c);
            p[0].push_back({xy.x, xy.y});
        }
        const Coord &c = *polygon.coords.begin();
        auto xy = mapView.coordToVector2f(c);
        p[0].push_back(Point({xy.x, xy.y}));

        std::vector<size_t> idx = mapbox::earcut<size_t>(p);
        for(size_t i = 0; i < idx.size(); i += 3){
            size_t idx0 = idx[i+0];
            size_t idx1 = idx[i+1];
            size_t idx2 = idx[i+2];

            Vector2f v0(p[0][idx0][0], p[0][idx0][1]);
            Vector2f v1(p[0][idx1][0], p[0][idx1][1]);
            Vector2f v2(p[0][idx2][0], p[0][idx2][1]);

            std::vector<sf::Vertex> *v = nullptr;
            if     (polygon.t == polygon_t::type::WATER) v = &water;
            else if(polygon.t == polygon_t::type::LAND) v = &land;
            if(v != nullptr){
                v->push_back(Vertex(v0, color));
                v->push_back(Vertex(v1, color));
                v->push_back(Vertex(v2, color));
            }
        }
    }
}

void MapTerrainOsmView::draw(){
    window.draw(&land[0], land.size(), Triangles);
    window.draw(&water[0], water.size(), Triangles);
}
