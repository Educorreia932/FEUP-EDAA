#include "MapGraphView.h"

#include "MapViewer.h"

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

const std::unordered_map<edge_type_t, MapGraphView::Display> MapGraphView::display_map = {
    {edge_type_t::MOTORWAY      , Display::MOTORWAY   },
    {edge_type_t::MOTORWAY_LINK , Display::MOTORWAY   },
    {edge_type_t::TRUNK         , Display::TRUNK      },
    {edge_type_t::TRUNK_LINK    , Display::TRUNK      },
    {edge_type_t::PRIMARY       , Display::PRIMARY    },
    {edge_type_t::PRIMARY_LINK  , Display::PRIMARY    },
    {edge_type_t::SECONDARY     , Display::SECONDARY  },
    {edge_type_t::SECONDARY_LINK, Display::SECONDARY  },
    {edge_type_t::TERTIARY      , Display::TERTIARY   },
    {edge_type_t::TERTIARY_LINK , Display::TERTIARY   },
    {edge_type_t::UNCLASSIFIED  , Display::ROAD       },
    {edge_type_t::RESIDENTIAL   , Display::RESIDENTIAL},
    {edge_type_t::LIVING_STREET , Display::SLOW       },
    {edge_type_t::SERVICE       , Display::SLOW       }
};

MapGraphView::MapGraphView(const MapGraph &g):graph(g){}

static MapViewer* createMapViewer(coord_t min_coord, coord_t max_coord, int w = 1800, int h = 900){
    MapViewer *gv = new MapViewer(w, h, min_coord, max_coord);
    return gv;
}

void MapGraphView::drawRoads() const{
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
    static const std::unordered_map<edge_type_t, int> width_map = {
        {edge_type_t::MOTORWAY      , 10},
        {edge_type_t::MOTORWAY_LINK , 10},
        {edge_type_t::TRUNK         , 10},
        {edge_type_t::TRUNK_LINK    , 10},
        {edge_type_t::PRIMARY       ,  7},
        {edge_type_t::PRIMARY_LINK  ,  7},
        {edge_type_t::SECONDARY     ,  7},
        {edge_type_t::SECONDARY_LINK,  7},
        {edge_type_t::TERTIARY      ,  5},
        {edge_type_t::TERTIARY_LINK ,  5},
        {edge_type_t::UNCLASSIFIED  ,  5},
        {edge_type_t::RESIDENTIAL   ,  5},
        {edge_type_t::LIVING_STREET ,  5},
        {edge_type_t::SERVICE       ,  5}
    };
    static const std::unordered_map<edge_type_t, GraphViewer::Color> color_map = {
        {edge_type_t::MOTORWAY      , GraphViewer::RED    },
        {edge_type_t::MOTORWAY_LINK , GraphViewer::RED    },
        {edge_type_t::TRUNK         , GraphViewer::PINK   },
        {edge_type_t::TRUNK_LINK    , GraphViewer::PINK   },
        {edge_type_t::PRIMARY       , GraphViewer::ORANGE },
        {edge_type_t::PRIMARY_LINK  , GraphViewer::ORANGE },
        {edge_type_t::SECONDARY     , GraphViewer::YELLOW },
        {edge_type_t::SECONDARY_LINK, GraphViewer::YELLOW },
        {edge_type_t::TERTIARY      , GraphViewer::GRAY   },
        {edge_type_t::TERTIARY_LINK , GraphViewer::GRAY   },
        {edge_type_t::UNCLASSIFIED  , GraphViewer::GRAY   },
        {edge_type_t::RESIDENTIAL   , GraphViewer::GRAY   },
        {edge_type_t::LIVING_STREET , GraphViewer::GRAY   },
        {edge_type_t::SERVICE       , GraphViewer::GRAY   }
    };

    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    MapViewer *gv = createMapViewer(graph.getMinCoord(), graph.getMaxCoord());
    gv->setBackgroundColor(MapViewer::Color(242, 239, 233));
    
    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color = color_map.at(way.edgeType);
        int width = width_map.at(way.edgeType);
        bool dashed = dashed_map.at(way.edgeType);
        bool draw = display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(drawn_nodes.find(v) == drawn_nodes.end()){
                drawn_nodes.insert(v);
                gv->addNode(v, nodes.at(v));
            }
            if(u != 0){
                gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width, dashed);
            }
            u = v;
            ++i;
        }
        
    }
    gv->createWindow();
    gv->join();
}

void MapGraphView::drawSpeeds() const{
    static const int width = 5;
    static const std::map<MapGraph::speed_t, GraphViewer::Color> color_map = {
        {120, GraphViewer::RED},
        {100, GraphViewer::ORANGE},
        { 80, GraphViewer::YELLOW},
        { 60, GraphViewer::GREEN},
        { 50, GraphViewer::BLACK},
        { 40, GraphViewer::GRAY}
    };

    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    MapViewer *gv = createMapViewer(graph.getMinCoord(), graph.getMaxCoord());

    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color; {
            auto it = color_map.lower_bound(way.getMaxSpeed());
            if(it == color_map.end()) throw std::invalid_argument("");
            color = it->second;
        }
        bool draw = display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(drawn_nodes.find(v) == drawn_nodes.end()){
                drawn_nodes.insert(v);
                gv->addNode(v, nodes.at(v));
            }
            if(u != 0){
                gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width);
            }
            u = v;
            ++i;
        }
        
    }
    gv->createWindow();
    gv->join();
}

void MapGraphView::drawVoronoi() const {
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
    static const std::unordered_map<edge_type_t, int> width_map = {
        {edge_type_t::MOTORWAY      , 10},
        {edge_type_t::MOTORWAY_LINK , 10},
        {edge_type_t::TRUNK         , 10},
        {edge_type_t::TRUNK_LINK    , 10},
        {edge_type_t::PRIMARY       ,  7},
        {edge_type_t::PRIMARY_LINK  ,  7},
        {edge_type_t::SECONDARY     ,  7},
        {edge_type_t::SECONDARY_LINK,  7},
        {edge_type_t::TERTIARY      ,  5},
        {edge_type_t::TERTIARY_LINK ,  5},
        {edge_type_t::UNCLASSIFIED  ,  5},
        {edge_type_t::RESIDENTIAL   ,  5},
        {edge_type_t::LIVING_STREET ,  5},
        {edge_type_t::SERVICE       ,  5}
    };
    static const std::unordered_map<edge_type_t, GraphViewer::Color> color_map = {
        {edge_type_t::MOTORWAY      , GraphViewer::RED    },
        {edge_type_t::MOTORWAY_LINK , GraphViewer::RED    },
        {edge_type_t::TRUNK         , GraphViewer::PINK   },
        {edge_type_t::TRUNK_LINK    , GraphViewer::PINK   },
        {edge_type_t::PRIMARY       , GraphViewer::ORANGE },
        {edge_type_t::PRIMARY_LINK  , GraphViewer::ORANGE },
        {edge_type_t::SECONDARY     , GraphViewer::YELLOW },
        {edge_type_t::SECONDARY_LINK, GraphViewer::YELLOW },
        {edge_type_t::TERTIARY      , GraphViewer::GRAY   },
        {edge_type_t::TERTIARY_LINK , GraphViewer::GRAY   },
        {edge_type_t::UNCLASSIFIED  , GraphViewer::GRAY   },
        {edge_type_t::RESIDENTIAL   , GraphViewer::GRAY   },
        {edge_type_t::LIVING_STREET , GraphViewer::GRAY   },
        {edge_type_t::SERVICE       , GraphViewer::GRAY   }
    };

    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    MapViewer *gv = createMapViewer(graph.getMinCoord(), graph.getMaxCoord());
    gv->setBackgroundColor(MapViewer::Color(242, 239, 233));
    
    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color = color_map.at(way.edgeType);
        int width = width_map.at(way.edgeType);
        bool dashed = dashed_map.at(way.edgeType);
        bool draw = display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(drawn_nodes.find(v) == drawn_nodes.end()){
                drawn_nodes.insert(v);
                gv->addNode(v, nodes.at(v));
            }
            if(u != 0){
                gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width, dashed);
            }
            u = v;
            ++i;
        }
        
    }

    // sf::ConvexShape *polygon = new sf::ConvexShape(4);
    // polygon->setPoint(0, gv->coord2vector({41.18565,-8.60933}));
    // polygon->setPoint(1, gv->coord2vector({41.18565,-8.59249}));
    // polygon->setPoint(2, gv->coord2vector({41.17882,-8.59249}));
    // polygon->setPoint(3, gv->coord2vector({41.17882,-8.60933}));
    // polygon->setFillColor(sf::Color(0,255,0,128));
    // gv->addDrawable("polygon", polygon);

    gv->createWindow();
    gv->join();
}
