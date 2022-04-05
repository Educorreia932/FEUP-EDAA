#pragma once

#include "DWGraph.h"
#include "EdgeType.h"
#include "Coord.h"
// #include "ClosestPoint.h"
// #include "Ride.h"

#include <map>

class MapGraph {
public:
    typedef double speed_t;
    struct way_t {
        std::list<DWGraph::node_t> nodes;
        speed_t speed;
        edge_type_t edgeType;
        /**
         * @brief Get the maximum speed in km/h
         * 
         * @return speed_t 
         */
        speed_t getMaxSpeed() const;
    };
private:
    DWGraph::DWGraph fullGraph;
    DWGraph::DWGraph connectedGraph;
    // ClosestPoint *closestPoint = nullptr;

    std::unordered_map<DWGraph::node_t, Coord> nodes;
    std::unordered_map<Coord, DWGraph::node_t> coord2node;
    Coord min_coord = Coord(+90.0, +180.0);
    Coord max_coord = Coord(-90.0, -180.0);
    std::list<way_t> ways;
public:
    MapGraph();
    /**
     * @brief Construct from files
     * 
     * @param path pathname
     */
    MapGraph(const std::string &path);
    ~MapGraph();
    void addNode(DWGraph::node_t u, Coord c);
    void addWay(way_t w);
    DWGraph::DWGraph getFullGraph() const;
    // DWGraph::DWGraph getConnectedGraph() const;
    MapGraph splitLongEdges(double threshold) const;
    const std::unordered_map<DWGraph::node_t, Coord>& getNodes() const;
    Coord getMinCoord() const;
    Coord getMaxCoord() const;
    const std::list<way_t> &getWays() const;
};
