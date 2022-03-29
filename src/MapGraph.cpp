#include "MapGraph.h"

// #include "Astar.h"
// #include "KosarajuV.h"
// #include "DFS.h"
// #include "Dijkstra.h"
// #include "ShortestPath.h"
// #include "ShortestPathAll.h"
// #include "VStripes.h"

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

const std::unordered_map<DWGraph::node_t, coord_t> &MapGraph::getNodes() const{
    return nodes;
}

coord_t MapGraph::getMinCoord() const { return min_coord; }
coord_t MapGraph::getMaxCoord() const { return max_coord; }
const std::list<MapGraph::way_t> &MapGraph::getWays() const { return ways; }
