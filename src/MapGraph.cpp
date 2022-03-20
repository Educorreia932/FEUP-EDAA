#include "MapGraph.h"

// #include "Astar.h"
// #include "KosarajuV.h"
// #include "DFS.h"
// #include "Dijkstra.h"
// #include "ShortestPath.h"
// #include "ShortestPathAll.h"
// #include "VStripes.h"

#include "MapViewer.h"

#include <fstream>
#include <iomanip>
#include <unordered_set>
#include <map>
#include <vector>

#define COORDMULT               50000       // Multiply coordinates to get integer positions
#define SECONDS_TO_MICROS       1000000     // Convert seconds to milliseconds
#define KMH_TO_MS               (1.0/3.6)   // Convert km/h to m/s
#define SPEED_REDUCTION_FACTOR  0.75        // Reduce speed to account for intense road traffic, and the fact people not always travel at maximum speed 

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;

std::string color2str(const GraphViewer::Color &color){
    if(color == GraphViewer::BLACK     ) return "BLACK"     ;
    if(color == GraphViewer::WHITE     ) return "WHITE"     ;
    if(color == GraphViewer::RED       ) return "RED"       ;
    if(color == GraphViewer::GREEN     ) return "GREEN"     ;
    if(color == GraphViewer::BLUE      ) return "BLUE"      ;
    if(color == GraphViewer::YELLOW    ) return "YELLOW"    ;
    if(color == GraphViewer::MAGENTA   ) return "MAGENTA"   ;
    if(color == GraphViewer::CYAN      ) return "CYAN"      ;
    if(color == GraphViewer::PINK      ) return "PINK"      ;
    if(color == GraphViewer::ORANGE    ) return "ORANGE"    ;
    if(color == GraphViewer::GRAY      ) return "GRAY"      ;
    if(color == GraphViewer::LIGHT_GRAY) return "LIGHT_GRAY";
    if(color == GraphViewer::DARK_GRAY ) return "DARK_GRAY" ;
    return "UNKNOWN";
}

MapGraph::speed_t MapGraph::way_t::getMaxSpeed() const{
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    if(speed != -1) return speed;
    #pragma GCC diagnostic pop

    switch(edgeType){
        case edge_type_t::MOTORWAY       : return 120;
        case edge_type_t::MOTORWAY_LINK  : return  60;
        case edge_type_t::TRUNK          : return 100;
        case edge_type_t::TRUNK_LINK     : return  50;
        case edge_type_t::PRIMARY        : return  90;
        case edge_type_t::PRIMARY_LINK   : return  40;
        case edge_type_t::SECONDARY      : return  70;
        case edge_type_t::SECONDARY_LINK : return  30;
        case edge_type_t::TERTIARY       : return  50;
        case edge_type_t::TERTIARY_LINK  : return  30;
        case edge_type_t::UNCLASSIFIED   : return  30;
        case edge_type_t::RESIDENTIAL    : return  30;
        case edge_type_t::LIVING_STREET  : return  10;
        case edge_type_t::SERVICE        : return  20;
        case edge_type_t::NO             : throw std::invalid_argument("");
        default                          : throw std::invalid_argument("");
    }
}

MapGraph::speed_t MapGraph::way_t::getRealSpeed() const{

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    if(speed != -1) return speed*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
    #pragma GCC diagnostic pop

    switch(edgeType){
        case edge_type_t::MOTORWAY       : return 120*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::MOTORWAY_LINK  : return  60*KMH_TO_MS;
        case edge_type_t::TRUNK          : return 100*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::TRUNK_LINK     : return  50*KMH_TO_MS;
        case edge_type_t::PRIMARY        : return  90*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::PRIMARY_LINK   : return  40*KMH_TO_MS;
        case edge_type_t::SECONDARY      : return  70*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::SECONDARY_LINK : return  30*KMH_TO_MS;
        case edge_type_t::TERTIARY       : return  50*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::TERTIARY_LINK  : return  30*KMH_TO_MS;
        case edge_type_t::UNCLASSIFIED   : return  30*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::RESIDENTIAL    : return  30*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::LIVING_STREET  : return  10*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::SERVICE        : return  20*KMH_TO_MS*SPEED_REDUCTION_FACTOR;
        case edge_type_t::NO             : throw std::invalid_argument("");
        default                          : throw std::invalid_argument("");
    }
}

MapGraph::MapGraph(const std::string &path){
    {
        std::ifstream is; is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        is.open(path + ".nodes");
        size_t numberNodes; is >> numberNodes;
        for(size_t i = 0; i < numberNodes; ++i){
            coord_t::deg_t lat, lon;
            node_t id; is >> id >> lat >> lon;
            coord_t c(lat, lon);
            nodes[id] = c;
            coord2node[c] = id;
        }
    }
    {
        std::ifstream is; is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        is.open(path + ".edges");
        size_t numberWays; is >> numberWays; 
        for(size_t i = 0; i < numberWays; ++i){
            way_t way; long long id; char c;
            is >> id >> c >> way.speed; way.edgeType = static_cast<edge_type_t>(c);
            size_t numberNodes; is >> numberNodes;
            for(size_t j = 0; j < numberNodes; ++j){
                node_t id; is >> id;
                way.nodes.push_back(id);
            }
            ways.push_back(way);
        }
    }
    {
        coord_t::deg_t lat_min = 90, lat_max = -90;
        coord_t::deg_t lon_min = +180, lon_max = -180;
        for(const auto &u: nodes){
            lat_min = std::min(lat_min, u.second.getLat()); lat_max = std::max(lat_max, u.second.getLat());
            lon_min = std::min(lon_min, u.second.getLon()); lon_max = std::max(lon_max, u.second.getLon());
        }
        min_coord = coord_t(lat_max, lon_min);
        max_coord = coord_t(lat_min, lon_max);
        mean_coord = (min_coord + max_coord)/2;
    }
    fullGraph = getFullGraph();
    /*
    {
        coord_t station_coord; {
            std::ifstream is; is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            is.open(path + ".points");
            size_t numberPoints; is >> numberPoints;
            is >> station_coord;
        }

        DWGraph::DWGraph G = fullGraph;
        std::list<coord_t> nodes_list;
        for(const node_t &u: G.getNodes()) nodes_list.push_back(nodes.at(u));

        ClosestPoint *closestPoint_initial = new VStripes(0.025);
        closestPoint_initial->initialize(nodes_list);
        closestPoint_initial->run();
        coord_t station_closest = closestPoint_initial->getClosestPoint(station_coord);
        delete closestPoint_initial;
        
        station = DWGraph::INVALID_NODE;
        
        for(const auto &p: nodes){
            if(p.second == station_closest) station = p.first;
        }
        if(station == DWGraph::INVALID_NODE) throw std::invalid_argument("No such node");

        std::cout << "Station node: " << station << std::endl;
    }
    connectedGraph = getConnectedGraph();
    {
        DWGraph::DWGraph G = connectedGraph;
        std::list<coord_t> nodes_list;
        for(const node_t &u: G.getNodes()) nodes_list.push_back(nodes.at(u));

        closestPoint = new VStripes(0.025);
        closestPoint->initialize(nodes_list);
        closestPoint->run();
    }
    */
    connectedGraph = fullGraph;

    std::cout << "Nodes: " << connectedGraph.getNodes().size() << std::endl;
    std::cout << "Edges: " << connectedGraph.getNumberEdges()  << std::endl;
}

MapGraph::~MapGraph(){
    // delete closestPoint;
}

DWGraph::node_t MapGraph::getStationNode() const { return station; }

DWGraph::DWGraph MapGraph::getFullGraph() const{
    DWGraph::DWGraph G;
    for(const auto &p: nodes) G.addNode(p.first);
    for(const way_t &w: ways){
        if(w.nodes.size() < 2) continue;
        auto it1 = w.nodes.begin();
        for(auto it2 = it1++; it1 != w.nodes.end(); ++it1, ++it2){
            auto d = coord_t::getDistanceSI(nodes.at(*it1), nodes.at(*it2));
            double factor = double(SECONDS_TO_MICROS)/(w.getRealSpeed());
            G.addEdge(*it2, *it1, weight_t(d*factor));
        }
    }
    return G;
}

/*
DWGraph::DWGraph MapGraph::getConnectedGraph() const{
    DWGraph::DWGraph G = fullGraph;
    DUGraph Gu = (DUGraph)G;
    Reachability *r = new DFS();
    SCCnode *scc = new KosarajuV(r);
    scc->initialize(&Gu, station);
    scc->run();
    std::list<node_t> nodes_to_remove;
    for(const DUGraph::node_t &u: Gu.getNodes()){
        if(!scc->is_scc(u)){
            nodes_to_remove.push_back(u);
        }
    }
    delete scc;
    delete r;
    G.removeNodes(nodes_to_remove.begin(), nodes_to_remove.end());
    return G;
}
*/

DWGraph::DWGraph MapGraph::getReducedGraph() const{
    DWGraph::DWGraph G = connectedGraph;
    std::cout << "Nodes: " << G.getNodes().size() << "\n"
              << "Edges: " << G.getNumberEdges() << "\n";
    
    size_t prev_nodes;
    do{
        prev_nodes = G.getNodes().size();

        DWGraph::DWGraph GT = G.getTranspose();
        auto V = G.getNodes();
        for(const node_t &u: V){
            const auto &desc = G.getAdj(u);
            const auto &pred = GT.getAdj(u);

            // Remove one-way streets
            if(desc.size() == 1 && pred.size() == 1 && desc.begin()->v != pred.begin()->v){
                //cout << "Deleting one-way street through " << u << endl;
                const node_t a = pred.begin()->v;
                const node_t b = desc.begin()->v;
                const weight_t w = pred.begin()->w + desc.begin()->w;
                G.addBestEdge(a, b, w); GT.addBestEdge(b, a, w);
                G.removeNode(u); GT.removeNode(u);
            } else
            if(desc.size() == 2 && pred.size() == 2){
                auto it = desc.begin();
                const DWGraph::Edge ua_f = *(it++); // Edge from u to a in forward notation
                const DWGraph::Edge ub_f = *(it++); // Edge from u to b in forward notation
                const node_t a = ua_f.v;
                const node_t b = ub_f.v;
                const weight_t w_ua = ua_f.w;
                const weight_t w_ub = ub_f.w;
                auto it1 = pred.find(ua_f);
                auto it2 = pred.find(ub_f);
                if(it1 != pred.end() && it2 != pred.end()){
                    const DWGraph::Edge au_b = *it1; // Edge from a to u in backwards notation
                    const DWGraph::Edge bu_b = *it2; // Edge from b to u in backwards notation
                    const weight_t w_au = au_b.w;
                    const weight_t w_bu = bu_b.w;
                    const weight_t w_ab = w_au+w_ub;
                    const weight_t w_ba = w_bu+w_ua;
                    G.addBestEdge(a, b, w_ab); GT.addBestEdge(b, a, w_ab);
                    G.addBestEdge(b, a, w_ba); GT.addBestEdge(a, b, w_ba);
                    G.removeNode(u); GT.removeNode(u);
                }
            }
        }
        std::cout << "Nodes: " << G.getNodes().size() << "\n"
                  << "Edges: " << G.getNumberEdges() << "\n";
    } while(prev_nodes != G.getNodes().size());

    return G;
}

// DWGraph::node_t MapGraph::getClosestNode(coord_t c) const{
//     coord_t closest = closestPoint->getClosestPoint(c);
//     node_t node = coord2node.at(closest);
//     return node;
// }

std::unordered_map<DWGraph::node_t, coord_t> MapGraph::getNodes() const{
    return nodes;
}


const std::unordered_map<edge_type_t, MapGraph::Display> MapGraph::display_map = {
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

MapViewer* createMapViewer(coord_t min_coord, coord_t max_coord, int w = 1800, int h = 900){
    MapViewer *gv = new MapViewer(w, h, min_coord, max_coord);
    return gv;
}

void MapGraph::drawRoads(int fraction, int display) const{
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
        {edge_type_t::MOTORWAY      , GraphViewer::Color(232, 146, 162) },
        {edge_type_t::MOTORWAY_LINK , GraphViewer::Color(232, 146, 162) },
        {edge_type_t::TRUNK         , GraphViewer::Color(249, 178, 156) },
        {edge_type_t::TRUNK_LINK    , GraphViewer::Color(249, 178, 156) },
        {edge_type_t::PRIMARY       , GraphViewer::Color(252, 214, 164) },
        {edge_type_t::PRIMARY_LINK  , GraphViewer::Color(252, 214, 164) },
        {edge_type_t::SECONDARY     , GraphViewer::Color(247, 250, 191) },
        {edge_type_t::SECONDARY_LINK, GraphViewer::Color(247, 250, 191) },
        {edge_type_t::TERTIARY      , GraphViewer::Color(200, 200, 200) },
        {edge_type_t::TERTIARY_LINK , GraphViewer::Color(200, 200, 200) },
        {edge_type_t::UNCLASSIFIED  , GraphViewer::Color(200, 200, 200) },
        {edge_type_t::RESIDENTIAL   , GraphViewer::Color(200, 200, 200) },
        {edge_type_t::LIVING_STREET , GraphViewer::Color(200, 200, 200) },
        {edge_type_t::SERVICE       , GraphViewer::Color(200, 200, 200) }
    };

    MapViewer *gv = createMapViewer(min_coord, max_coord);
    gv->setBackgroundColor(MapViewer::Color(242, 239, 233));
    
    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color = color_map.at(way.edgeType);
        int width = width_map.at(way.edgeType);
        bool dashed = dashed_map.at(way.edgeType);
        bool draw = display & display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(i%fraction == 0 || i == way.nodes.size()-1){
                if(drawn_nodes.find(v) == drawn_nodes.end()){
                    drawn_nodes.insert(v);
                    gv->addNode(v, nodes.at(v));
                }
                if(u != 0){
                    gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width, dashed);
                }
                u = v;
            }
            ++i;
        }
        
    }
    gv->createWindow();
    gv->join();
}

void MapGraph::drawSpeeds(int fraction, int display) const{
    static const int width = 5;
    static const std::map<speed_t, GraphViewer::Color> color_map = {
        {120, GraphViewer::RED},
        {100, GraphViewer::ORANGE},
        { 80, GraphViewer::YELLOW},
        { 60, GraphViewer::GREEN},
        { 50, GraphViewer::BLACK},
        { 40, GraphViewer::GRAY}
    };

    MapViewer *gv = createMapViewer(min_coord, max_coord);

    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){
        GraphViewer::Color color; {
            auto it = color_map.lower_bound(way.getMaxSpeed());
            if(it == color_map.end()) throw std::invalid_argument("");
            color = it->second;
        }
        bool draw = display & display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(i%fraction == 0 || i == way.nodes.size()-1){
                if(drawn_nodes.find(v) == drawn_nodes.end()){
                    drawn_nodes.insert(v);
                    gv->addNode(v, nodes.at(v));
                }
                if(u != 0){
                    gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width);
                }
                u = v;
            }
            ++i;
        }
        
    }
    gv->createWindow();
    gv->join();
}

void MapGraph::drawSCC(int fraction, int display) const{
    static const int width = 5;
    static const std::map<bool, GraphViewer::Color> color_map = {
        {true , GraphViewer::RED},
        {false, GraphViewer::GRAY}
    };

    DWGraph::DWGraph G  = fullGraph;
    std::unordered_set<node_t> connected_nodes; {
        DWGraph::DWGraph Gu = connectedGraph;
        const auto &l = Gu.getNodes();
        connected_nodes = std::unordered_set<node_t>(l.begin(), l.end());
    }

    MapViewer *gv = createMapViewer(min_coord, max_coord);

    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){

        bool draw = display & display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(i%fraction == 0 || i == way.nodes.size()-1){
                if(drawn_nodes.find(v) == drawn_nodes.end()){
                    drawn_nodes.insert(v);
                    gv->addNode(v, nodes.at(v));
                }
                if(u != 0){
                    GraphViewer::Color color = color_map.at(connected_nodes.count(u) && connected_nodes.count(v));
                    gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width);
                }
                u = v;
            }
            ++i;
        }
        
    }
    gv->createWindow();
    gv->join();
}

/*
class MapGraph::DistanceHeuristic : public Astar::heuristic_t{
private:
    const std::unordered_map<node_t, coord_t> &nodes;
    coord_t dst_pos;
    double factor;
public:
    DistanceHeuristic(const std::unordered_map<node_t, coord_t> &nodes_,
                      coord_t dst_pos_,
                      double factor_): nodes(nodes_), dst_pos(dst_pos_), factor(factor_){}
    weight_t operator()(node_t u) const{
        auto d = coord_t::getDistanceSI(dst_pos, nodes.at(u));
        return weight_t(d*factor);
    }
};
*/

/*
void MapGraph::drawPath(int fraction, int display, node_t src, node_t dst, bool visited) const{
    DWGraph::DWGraph G = connectedGraph;

    std::vector<std::string> name({
        "Dijkstra's algorithm with early stop",
        "A* algorithm, 90km/h",
        "A* algorithm, 60km/h",
        "A* algorithm, 30km/h",
        "A* algorithm, 10km/h"
    });

    std::vector<Astar::heuristic_t*> heuristics({
        new DistanceHeuristic(nodes, nodes.at(dst), double(SECONDS_TO_MICROS)/(90.0*KMH_TO_MS)),
        new DistanceHeuristic(nodes, nodes.at(dst), double(SECONDS_TO_MICROS)/(60.0*KMH_TO_MS)),
        new DistanceHeuristic(nodes, nodes.at(dst), double(SECONDS_TO_MICROS)/(30.0*KMH_TO_MS)),
        new DistanceHeuristic(nodes, nodes.at(dst), double(SECONDS_TO_MICROS)/(10.0*KMH_TO_MS))
    });

    std::vector<ShortestPath*> shortestPaths({
        new Astar(),
        new Astar(heuristics[0]),
        new Astar(heuristics[1]),
        new Astar(heuristics[2]),
        new Astar(heuristics[3])
    });

    std::vector<GraphViewer::Color> pathColor({
        GraphViewer::BLACK,
        GraphViewer::RED,
        GraphViewer::MAGENTA,
        GraphViewer::BLUE,
        GraphViewer::CYAN
    });

    std::vector<GraphViewer::Color> visitedColor({
        GraphViewer::PINK,
        GraphViewer::RED,
        GraphViewer::MAGENTA,
        GraphViewer::BLUE,
        GraphViewer::CYAN
    });

    std::vector<std::unordered_set<node_t> > paths(shortestPaths.size());

    std::cout << std::setfill(' ');
    std::cout << "| Algorithm                                | Colour     | Execution time (micros) | Analysed nodes | Analysed edges | Total time (micros) | Increase in total time | Nodes in path |\n"
              << "|------------------------------------------|------------|-------------------------|----------------|----------------|---------------------|------------------------|---------------|\n";

    for(size_t i = 0; i < shortestPaths.size(); ++i){
        shortestPaths[i]->initialize(&G, src, dst);
        shortestPaths[i]->run();
        std::list<node_t> path = shortestPaths[i]->getPath();
        paths[i] = std::unordered_set<node_t>(path.begin(), path.end());
        statistics_t stats = shortestPaths[i]->getStatistics();
        std::cout   <<  "| " << std::setw(40) << name[i]
                    << " | " << std::setw(10) << color2str((!visited ? pathColor[i] : visitedColor[i])) << " | " << std::setw(23) << stats.execution_time
                    << " | " << std::setw(14) << stats.analysed_nodes
                    << " | " << std::setw(14) << stats.analysed_edges
                    << " | " << std::setw(19) << shortestPaths[i]->getPathWeight()
                    << " | " << std::setw(21) << 100.0*((double)shortestPaths[i]->getPathWeight()/(double)shortestPaths[0]->getPathWeight()-1.0) << "%"
                    << " | " << std::setw(13) << paths[i].size() << " |\n";
    }
    
    MapViewer *gv = createMapViewer(min_coord, max_coord);

    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){

        bool draw = display & display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(!G.hasNode(v)) break;
            if(i%fraction == 0 || i == way.nodes.size()-1){
                if(drawn_nodes.find(v) == drawn_nodes.end()){
                    drawn_nodes.insert(v);
                    gv->addNode(v, nodes.at(v));
                }
                if(u != 0){
                    GraphViewer::Color color = GraphViewer::LIGHT_GRAY;
                    int width = 4;

                    if(!visited){
                        for(size_t j = 0; j < shortestPaths.size(); ++j){
                            if (paths[j].count(u) && paths[j].count(v)){
                                color = pathColor[j];
                                width = 12;
                                break;
                            }
                        }
                    } else {
                        if(paths[0].count(u) && paths[0].count(v)){
                            color = pathColor[0];
                            width = 12;
                        } else {
                            for(long j = shortestPaths.size()-1; j >= 0; --j){
                                if (shortestPaths[j]->hasVisited(u) && shortestPaths[j]->hasVisited(v)){
                                    color = visitedColor[j];
                                    break;
                                }
                            }
                        }
                    }

                    gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width);
                }
                u = v;
            }
            ++i;
        }
    }
    gv->createWindow();
    
    for(Astar::heuristic_t *p: heuristics) delete p;
    for(ShortestPath *p: shortestPaths) delete p;
    
    gv->join();
}
*/

/*
void MapGraph::drawPath(int fraction, int display, coord_t src, coord_t dst, bool visited) const{

    node_t src_node = getClosestNode(src);
    node_t dst_node = getClosestNode(dst);

    std::cout << "Source      (" << src << ") transformed into node " << src_node << std::endl;
    std::cout << "Destination (" << dst << ") transformed into node " << dst_node << std::endl;

    drawPath(fraction, display, src_node, dst_node, visited);
}
*/

void MapGraph::drawReduced() const{
    DWGraph::DWGraph G = getReducedGraph();

    MapViewer *mv = createMapViewer(min_coord, max_coord);
    long long edge_id = 0;

    const auto &V = G.getNodes();
    for(const node_t &u: V){
        mv->addNode(u, nodes.at(u));
    }
    for(const node_t &u: V){
        for(const DWGraph::Edge &e: G.getAdj(u)){
            mv->addEdge(edge_id++, u, e.v, GraphViewer::Edge::EdgeType::DIRECTED, GraphViewer::GRAY, 7);
        }
    }

    mv->createWindow();
    mv->join();
}

/*
void MapGraph::drawRide(int fraction, int display, const Ride &r) const{
    DWGraph::DWGraph G = connectedGraph;

    std::unordered_set<node_t> visited_nodes, important_nodes; {
        std::vector<node_t> path;{
            for(const Ride::Event &e: r.getEvents()){
                if(e.getType() == Ride::Event::event_type::BE_THERE){
                    path.push_back(e.getNode());
                    continue;
                }
                coord_t c = e.getClient().getDest();
                path.push_back(coord2node.at(closestPoint->getClosestPoint(c)));
            }
        }
        important_nodes = std::unordered_set<node_t>(path.begin(), path.end());
        {
            for(size_t i = 1; i < path.size(); ++i){
                const node_t &u = path[i-1], &v = path[i];
                Astar::heuristic_t *w = new DistanceHeuristic(nodes, nodes.at(v), double(SECONDS_TO_MICROS)/(90.0*KMH_TO_MS));
                ShortestPath *sp = new Astar(w);
                sp->initialize(&G, u, v);
                sp->run();
                std::list<node_t> small_path = sp->getPath();
                visited_nodes.insert(small_path.begin(), small_path.end());
                delete sp;
                delete w;
            }
        }
    }

    MapViewer *gv = createMapViewer(min_coord, max_coord);

    std::unordered_set<node_t> drawn_nodes;
    size_t edge_id = 0;
    for(const way_t &way: ways){

        bool draw = display & display_map.at(way.edgeType);

        if(!draw) continue;

        node_t u = 0;
        size_t i = 0;
        for(const node_t &v: way.nodes){
            if(!G.hasNode(v)) break;
            if(i%fraction == 0 || i == way.nodes.size()-1){
                if(drawn_nodes.find(v) == drawn_nodes.end()){
                    drawn_nodes.insert(v);
                    if(important_nodes.count(v)) gv->addNode(v, nodes.at(v), GraphViewer::BLACK, 35);
                    else                         gv->addNode(v, nodes.at(v));
                }
                if(u != 0){
                    GraphViewer::Color color = GraphViewer::LIGHT_GRAY;
                    int width = 4;

                    if(visited_nodes.count(u) && visited_nodes.count(v)){
                        color = GraphViewer::RED;
                        width = 8;
                    }

                    gv->addEdge(edge_id++, u, v, GraphViewer::Edge::EdgeType::UNDIRECTED, color, width);
                }
                u = v;
            }
            ++i;
        }
    }
    gv->createWindow();
    gv->join();
}
*/
