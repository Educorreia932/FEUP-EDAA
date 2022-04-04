#include "MapGraphOsmView.h"

#include <mapbox/earcut.hpp>

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

using Point = std::array<float, 2>;

static const std::unordered_map<edge_type_t, bool> dashed_map = {
    {edge_type_t::MOTORWAY      , false},
    {edge_type_t::MOTORWAY_LINK , false},
    {edge_type_t::TRUNK         , false},
    {edge_type_t::TRUNK_LINK    , false},
    {edge_type_t::PRIMARY       , false},
    {edge_type_t::PRIMARY_LINK  , false},
    {edge_type_t::SECONDARY     , false},
    {edge_type_t::SECONDARY_LINK, false},
    {edge_type_t::TERTIARY      , false},
    {edge_type_t::TERTIARY_LINK , false},
    {edge_type_t::UNCLASSIFIED  , false},
    {edge_type_t::RESIDENTIAL   , false},
    {edge_type_t::LIVING_STREET , true },
    {edge_type_t::SERVICE       , true }
};
static const std::unordered_map<edge_type_t, float> width_map = {
    {edge_type_t::MOTORWAY      , 10.0},
    {edge_type_t::MOTORWAY_LINK ,  5.0},
    {edge_type_t::TRUNK         , 10.0},
    {edge_type_t::TRUNK_LINK    ,  5.0},
    {edge_type_t::PRIMARY       ,  7.0},
    {edge_type_t::PRIMARY_LINK  ,  5.0},
    {edge_type_t::SECONDARY     ,  7.0},
    {edge_type_t::SECONDARY_LINK,  5.0},
    {edge_type_t::TERTIARY      ,  5.0},
    {edge_type_t::TERTIARY_LINK ,  5.0},
    {edge_type_t::UNCLASSIFIED  ,  5.0},
    {edge_type_t::RESIDENTIAL   ,  5.0},
    {edge_type_t::LIVING_STREET ,  5.0},
    {edge_type_t::SERVICE       ,  5.0}
};
static const std::unordered_map<edge_type_t, Color> color_map = {
    {edge_type_t::MOTORWAY      , Color(232, 146, 162) },
    {edge_type_t::MOTORWAY_LINK , Color(232, 146, 162) },
    {edge_type_t::TRUNK         , Color(249, 178, 156) },
    {edge_type_t::TRUNK_LINK    , Color(249, 178, 156) },
    {edge_type_t::PRIMARY       , Color(252, 214, 164) },
    {edge_type_t::PRIMARY_LINK  , Color(252, 214, 164) },
    {edge_type_t::SECONDARY     , Color(247, 250, 191) },
    {edge_type_t::SECONDARY_LINK, Color(247, 250, 191) },
    {edge_type_t::TERTIARY      , Color(200, 200, 200) },
    {edge_type_t::TERTIARY_LINK , Color(200, 200, 200) },
    {edge_type_t::UNCLASSIFIED  , Color(200, 200, 200) },
    {edge_type_t::RESIDENTIAL   , Color(200, 200, 200) },
    {edge_type_t::LIVING_STREET , Color(200, 200, 200) },
    {edge_type_t::SERVICE       , Color(200, 200, 200) }
};
static const std::list<edge_type_t> order = {
    edge_type_t::MOTORWAY      ,
    edge_type_t::MOTORWAY_LINK ,
    edge_type_t::TRUNK         ,
    edge_type_t::TRUNK_LINK    ,
    edge_type_t::PRIMARY       ,
    edge_type_t::PRIMARY_LINK  ,
    edge_type_t::SECONDARY     ,
    edge_type_t::SECONDARY_LINK,
    edge_type_t::TERTIARY      ,
    edge_type_t::TERTIARY_LINK ,
    edge_type_t::UNCLASSIFIED  ,
    edge_type_t::RESIDENTIAL   ,
    edge_type_t::LIVING_STREET ,
    edge_type_t::SERVICE
};

MapGraphOsmView::MapGraphOsmView(WindowView &windowView_, MapView &mapView_, const MapGraph &graph_, const vector<polygon_t> &polygons_):
    windowView(windowView_), mapView(mapView_), graph(graph_)
{
    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    auto it = order.rbegin();
    while(it != order.rend()){
        const edge_type_t &edgeType = *(it++);
        for(const way_t &way: ways){
            if(way.edgeType != edgeType) continue;
            if(way.nodes.size() < 2) continue;

            Color color = color_map.at(way.edgeType);
            float width = width_map.at(way.edgeType);
            bool dashed = dashed_map.at(way.edgeType);

            auto it1 = way.nodes.begin(),
                it2 = ++way.nodes.begin();
            while(it2 != way.nodes.end()){
                sf::Vector2f u = mapView.coordToVector2f(nodes.at(*(it1++))),
                             v = mapView.coordToVector2f(nodes.at(*(it2++)));
                LineShape *e = nullptr;
                if(!dashed) e = new FullLineShape  (u, v, width);
                else        e = new DashedLineShape(u, v, width);
                e->setFillColor(color);
                const VertexArray &shape = *e;
                for(size_t i = 0; i < shape.getVertexCount(); ++i)
                    zip.push_back(shape[i]);
                delete e; e = nullptr;
            }
        }
    }

    for(const polygon_t &polygon: polygons_){
        if(polygon.coords.size() < 2) continue;
        vector<vector<Point>> p(1);
        for(const coord_t &c: polygon.coords){
            auto xy = mapView.coordToVector2f(c);
            p[0].push_back({xy.x, xy.y});
        }
        const coord_t &c = *polygon.coords.begin();
        auto xy = mapView.coordToVector2f(c);
        p[0].push_back(Point({xy.x, xy.y}));

        std::vector<size_t> idx = mapbox::earcut<size_t>(p);
        for(size_t i = 0; i < idx.size(); i += 3){
            size_t idx0 = idx[i+0];
            size_t idx1 = idx[i+1];
            size_t idx2 = idx[i+2];

            Vector2f v0(p[0][idx0][0], p[0][idx0][1]); cout << v0.x << "," << v0.y << endl;
            Vector2f v1(p[0][idx1][0], p[0][idx1][1]); cout << v1.x << "," << v1.y << endl;
            Vector2f v2(p[0][idx2][0], p[0][idx2][1]); cout << v2.x << "," << v2.y << endl;
            
            
            polygonShapes.push_back(Vertex(v0, Color(170, 211, 223)));
            polygonShapes.push_back(Vertex(v1, Color(170, 211, 223)));
            polygonShapes.push_back(Vertex(v2, Color(170, 211, 223)));
        }
    }
}

void MapGraphOsmView::draw(){
    RenderWindow *window = windowView.getWindow();
    window->draw(&polygonShapes[0], polygonShapes.size(), Triangles);
    window->draw(&zip[0], zip.size(), Quads);
}
