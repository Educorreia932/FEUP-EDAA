#include "MapTripsView.h"

#include "MapViewer.h"

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

MapTripsView::MapTripsView(const MapGraph &g):graph(g){}

static MapViewer* createMapViewer(coord_t min_coord, coord_t max_coord, int w = 1800, int h = 900){
    MapViewer *gv = new MapViewer(w, h, min_coord, max_coord);
    return gv;
}

void MapTripsView::drawTrips() const{
    static const int defaultWidth = 2;
    static const GraphViewer::Color defaultColor = GraphViewer::Color(255, 0, 0, 16);

    auto nodes = graph.getNodes();
    auto ways = graph.getWays();

    MapViewer *gv = createMapViewer(graph.getMinCoord(), graph.getMaxCoord());
    gv->setBackgroundColor(MapViewer::Color(242, 239, 233));
    
    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color = defaultColor;
        int width = defaultWidth;

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
