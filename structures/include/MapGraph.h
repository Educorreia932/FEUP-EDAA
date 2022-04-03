#ifndef MAPGRAPH_H_INCLUDED
#define MAPGRAPH_H_INCLUDED

#include "DWGraph.h"
#include "EdgeType.h"
#include "coord.h"
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
        /**
         * @brief Get the real speed in m/s
         * 
         * @return speed_t 
         */
        speed_t getRealSpeed() const;
    };
private:
    DWGraph::DWGraph fullGraph;
    DWGraph::DWGraph connectedGraph;
    // ClosestPoint *closestPoint = nullptr;

    std::unordered_map<DWGraph::node_t, coord_t> nodes;
    std::map<coord_t, DWGraph::node_t> coord2node;
    coord_t min_coord = coord_t(+90.0, +180.0);
    coord_t max_coord = coord_t(-90.0, -180.0);
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
    void addNode(DWGraph::node_t u, coord_t c);
    void addWay(way_t w);
    DWGraph::DWGraph getFullGraph() const;
    DWGraph::DWGraph getConnectedGraph() const;
    DWGraph::DWGraph getReducedGraph() const;
    DWGraph::node_t getClosestNode(coord_t c) const;
    const std::unordered_map<DWGraph::node_t, coord_t>& getNodes() const;
    coord_t getMinCoord() const;
    coord_t getMaxCoord() const;
    const std::list<way_t> &getWays() const;
};

#endif //MAPGRAPH_H_INCLUDED
