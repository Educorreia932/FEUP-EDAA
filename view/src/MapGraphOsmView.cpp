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
    {edge_type_t::MOTORWAY      ,  7.0},
    {edge_type_t::MOTORWAY_LINK ,  3.0},
    {edge_type_t::TRUNK         ,  7.0},
    {edge_type_t::TRUNK_LINK    ,  3.0},
    {edge_type_t::PRIMARY       ,  5.0},
    {edge_type_t::PRIMARY_LINK  ,  3.0},
    {edge_type_t::SECONDARY     ,  5.0},
    {edge_type_t::SECONDARY_LINK,  3.0},
    {edge_type_t::TERTIARY      ,  3.0},
    {edge_type_t::TERTIARY_LINK ,  3.0},
    {edge_type_t::UNCLASSIFIED  ,  3.0},
    {edge_type_t::RESIDENTIAL   ,  3.0},
    {edge_type_t::LIVING_STREET ,  1.0},
    {edge_type_t::SERVICE       ,  1.0}
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

static const Color buildingOutlineColor(196, 182, 171);

static const size_t N_CIRCLE = 8;

MapGraphOsmView::MapGraphOsmView(WindowView &windowView_, MapView &mapView_, const MapGraph &graph_):
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

            // sf::Vector2f u = mapView.coordToVector2f(nodes.at(*way.nodes.begin()));
            // for(size_t i = 0; i < N_CIRCLE/2; ++i){
            //     float theta1 = 2*M_PI/N_CIRCLE * i;
            //     float theta2 = 2*M_PI/N_CIRCLE * (i+1);
            //     float x1 = cos(theta1)*width/2,
            //           x2 = cos(theta2)*width/2;
            //     float y1 = sin(theta1)*width/2,
            //           y2 = sin(theta2)*width/2;
            //     zip.push_back(Vertex({u.x+x1, u.y+y1}, color));
            //     zip.push_back(Vertex({u.x+x2, u.y+y2}, color));
            //     zip.push_back(Vertex({u.x-x1, u.y-y1}, color));
            //     zip.push_back(Vertex({u.x-x2, u.y-y2}, color));
            // }

            auto it1 = way.nodes.begin(),
                it2 = ++way.nodes.begin();
            bool first = true;
            while(it2 != way.nodes.end()){
                sf::Vector2f u = mapView.coordToVector2f(nodes.at(*(it1++))),
                             v = mapView.coordToVector2f(nodes.at(*(it2++)));
                LineShape *e = nullptr;
                if(!dashed) e = new FullLineShape  (u, v, width);
                else        e = new DashedLineShape(u, v, width);
                e->setFillColor(color);
                const VertexArray &shape = *e;
                const size_t sz = zip.size();
                if(!first && !dashed){
                    zip.push_back(zip[sz-2]);
                    zip.push_back(zip[sz-1]);
                    zip.push_back(shape[0] );
                    zip.push_back(shape[1] );
                }
                for(size_t i = 0; i < shape.getVertexCount(); ++i)
                    zip.push_back(shape[i]);
                delete e; e = nullptr;
                first = false;
            }

            // u = mapView.coordToVector2f(nodes.at(*way.nodes.rbegin()));
            // for(size_t i = 0; i < N_CIRCLE/2; ++i){
            //     float theta1 = 2*M_PI/N_CIRCLE * i;
            //     float theta2 = 2*M_PI/N_CIRCLE * (i+1);
            //     float x1 = cos(theta1)*width/2,
            //           x2 = cos(theta2)*width/2;
            //     float y1 = sin(theta1)*width/2,
            //           y2 = sin(theta2)*width/2;
            //     zip.push_back(Vertex({u.x+x1, u.y+y1}, color));
            //     zip.push_back(Vertex({u.x+x2, u.y+y2}, color));
            //     zip.push_back(Vertex({u.x-x1, u.y-y1}, color));
            //     zip.push_back(Vertex({u.x-x2, u.y-y2}, color));
            // }
        }
    }
}

void MapGraphOsmView::draw(){
    RenderWindow *window = windowView.getWindow();
    window->draw(&zip[0], zip.size(), Quads);
}
