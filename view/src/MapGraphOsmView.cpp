#include "MapGraphOsmView.h"

using namespace sf;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

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
    {edge_type_t::MOTORWAY_LINK , 10.0},
    {edge_type_t::TRUNK         , 10.0},
    {edge_type_t::TRUNK_LINK    , 10.0},
    {edge_type_t::PRIMARY       ,  7.0},
    {edge_type_t::PRIMARY_LINK  ,  7.0},
    {edge_type_t::SECONDARY     ,  7.0},
    {edge_type_t::SECONDARY_LINK,  7.0},
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

MapGraphOsmView::MapGraphOsmView(WindowView &windowView_, MapView &mapView_, const MapGraph &graph_):
    windowView(windowView_), mapView(mapView_), graph(graph_)
{
    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    for(const way_t &way: ways){
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

void MapGraphOsmView::draw(){
    RenderWindow *window = windowView.getWindow();
    window->draw(&zip[0], zip.size(), Quads);
}
