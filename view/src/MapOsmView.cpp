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
    window(window_), mapView(mapView_), mapGraphOsmView(window_, mapView_, graph_)
{
    for(const polygon_t &polygon: polygons_){
        if(polygon.coords.size() < 2) continue;
        Color color;
        switch(polygon.t){
            case polygon_t::type::WATER   : color = Color(170, 211, 223); break;
            case polygon_t::type::LAND    : color = Color(242, 239, 233); break;
            case polygon_t::type::BUILDING: color = Color(217, 208, 201); break;
            default                    : color = Color(  0,   0,   0); break;
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

            std::vector<sf::Vertex> *v;
            if     (polygon.t == polygon_t::type::WATER) v = &water;
            else if(polygon.t == polygon_t::type::LAND) v = &land;
            else if(polygon.t == polygon_t::type::BUILDING) v = &building;
            v->push_back(Vertex(v0, color));
            v->push_back(Vertex(v1, color));
            v->push_back(Vertex(v2, color));
        }

        if(polygon.t == polygon_t::type::BUILDING){
            for(size_t i = 1; i < p[0].size(); ++i){
                Vector2f u(p[0][i-1][0], p[0][i-1][1]),
                        v(p[0][i-0][0], p[0][i-0][1]);
                buildingOutlines.push_back(Vertex(u, buildingOutlineColor));
                buildingOutlines.push_back(Vertex(v, buildingOutlineColor));
            }
        }
    }
}

void MapOsmView::draw(){
    window.draw(&land[0], land.size(), Triangles);
    window.draw(&water[0], water.size(), Triangles);
    window.draw(&building[0], building.size(), Triangles);
    window.draw(&buildingOutlines[0], buildingOutlines.size(), Lines);
    mapGraphOsmView.draw();
}
