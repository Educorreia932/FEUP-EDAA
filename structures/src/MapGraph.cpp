#include "MapGraph.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;

const double SECONDS_TO_MICROS = 1000000;     // Convert seconds to milliseconds
const double KPH_TO_MPS        = (1.0/3.6);   // Convert km/h to m/s
const double METERS_TO_MILLIMS = 1000;

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
        case edge_type_t::CONSTRUCTION   : return  30;
        case edge_type_t::NO             : throw std::invalid_argument("");
        default                          : throw std::invalid_argument("");
    }
}

MapGraph::MapGraph(){}

MapGraph::MapGraph(const std::string &path){
    {
        std::ifstream is; is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        is.open(path + ".nodes");
        size_t numberNodes; is >> numberNodes;
        for(size_t i = 0; i < numberNodes; ++i){
            double lat, lon;
            node_t id; is >> id >> lat >> lon;
            Coord c(lat, lon);
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
        double lat_min = 90, lat_max = -90;
        double lon_min = +180, lon_max = -180;
        for(const auto &u: nodes){
            lat_min = std::min(lat_min, u.second.lat()); lat_max = std::max(lat_max, u.second.lat());
            lon_min = std::min(lon_min, u.second.lon()); lon_max = std::max(lon_max, u.second.lon());
        }
        min_coord = Coord(lat_max, lon_min);
        max_coord = Coord(lat_min, lon_max);
    }

    std::cout << "Nodes: " << nodes.size() << std::endl;
    std::cout << "Edges: " << getNumberOfEdges() << std::endl;
}

MapGraph::~MapGraph(){
    // delete closestPoint;
}

void MapGraph::addNode(DWGraph::node_t u, Coord c){
    nodes[u] = c;
    coord2node[c] = u;
    min_coord.lat() = std::min(min_coord.lat(), c.lat());
    min_coord.lon() = std::min(min_coord.lon(), c.lon());
    max_coord.lat() = std::max(max_coord.lat(), c.lat());
    max_coord.lon() = std::max(max_coord.lon(), c.lon());
}

void MapGraph::addWay(MapGraph::way_t w){
    ways.push_back(w);
}

DWGraph::DWGraph MapGraph::getTimeGraph() const{
    DWGraph::DWGraph G;
    for(const auto &p: nodes) G.addNode(p.first);
    for(const way_t &w: ways){
        if(w.nodes.size() < 2) continue;
        auto it1 = w.nodes.begin();
        for(auto it2 = it1++; it1 != w.nodes.end(); ++it1, ++it2){
            auto d = Coord::getDistanceArc(nodes.at(*it1), nodes.at(*it2));
            double factor = double(SECONDS_TO_MICROS)/(w.getMaxSpeed()*KPH_TO_MPS);
            G.addEdge(*it2, *it1, weight_t(d*factor));
        }
    }
    return G;
}

DWGraph::DWGraph MapGraph::getDistanceGraph() const{
    DWGraph::DWGraph G;
    for(const auto &p: nodes) G.addNode(p.first);
    for(const way_t &w: ways){
        if(w.nodes.size() < 2) continue;
        auto it1 = w.nodes.begin();
        for(auto it2 = it1++; it1 != w.nodes.end(); ++it1, ++it2){
            auto d = Coord::getDistanceArc(nodes.at(*it1), nodes.at(*it2));
            G.addEdge(*it2, *it1, weight_t(d*METERS_TO_MILLIMS));
        }
    }
    return G;
}

MapGraph MapGraph::splitLongEdges(double threshold) const {
    MapGraph G;
    G.nodes = nodes;
    G.coord2node = coord2node;
    G.ways = ways;
    G.min_coord = min_coord;
    G.max_coord = max_coord;

    node_t nextNodeId = 0;
    for(const auto &p: G.nodes)
        nextNodeId = max(nextNodeId, p.first);
    ++nextNodeId;

    for(way_t &way: G.ways){
        for(auto it1 = way.nodes.begin(), it2 = ++way.nodes.begin(); it2 != way.nodes.end();){
            const Coord &u = G.nodes.at(*it1);
            const Coord &v = G.nodes.at(*it2);

            double d = Coord::getDistanceArc(u, v);
            if(d > threshold){
                Coord m = u + (v-u)/2;

                node_t id;
                if(G.coord2node.find(m) != G.coord2node.end()) id = G.coord2node.at(m);
                else id = nextNodeId++;
                
                G.nodes[id] = m;
                G.coord2node[m] = id;
                it2 = way.nodes.insert(it2, id);
            } else {
                ++it1;
                ++it2;
            }
        }
    }

    return G;
}

const std::unordered_map<DWGraph::node_t, Coord> &MapGraph::getNodes() const{
    return nodes;
}

size_t MapGraph::getNumberOfEdges() const {
    size_t ret = 0;
    for(const way_t &w: ways){
        ret += w.nodes.size()-1;
    }
    return ret;
}

Coord MapGraph::getMinCoord() const { return min_coord; }
Coord MapGraph::getMaxCoord() const { return max_coord; }
const std::list<MapGraph::way_t> &MapGraph::getWays() const { return ways; }

DWGraph::node_t MapGraph::coordToNode(const Coord &c) const {
    return coord2node.at(c);
}
Coord MapGraph::nodeToCoord(const DWGraph::node_t &u) const {
    return nodes.at(u);
}
