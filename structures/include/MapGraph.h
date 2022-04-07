#pragma once

#include "DWGraph.h"
#include "EdgeType.h"
#include "Coord.h"
#include "Astar.h"
#include "AstarFew.h"

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
    DWGraph::DWGraph getTimeGraph() const;
    DWGraph::DWGraph getDistanceGraph() const;
    MapGraph splitLongEdges(double threshold) const;
    const std::unordered_map<DWGraph::node_t, Coord>& getNodes() const;
    Coord getMinCoord() const;
    Coord getMaxCoord() const;
    const std::list<way_t> &getWays() const;
    DWGraph::node_t coordToNode(const Coord &c) const;
    Coord nodeToCoord(const DWGraph::node_t &u) const;

    class DistanceHeuristic : public Astar::heuristic_t{
    private:
        const std::unordered_map<DWGraph::node_t, Coord> &nodes;
        Coord dst_pos;
        double factor;
    public:
        DistanceHeuristic(const std::unordered_map<DWGraph::node_t, Coord> &nodes_,
                        Coord dst_pos_,
                        double factor_): nodes(nodes_), dst_pos(dst_pos_), factor(factor_){}
        DWGraph::weight_t operator()(DWGraph::node_t u) const{
            auto d = Coord::getDistanceArc(dst_pos, nodes.at(u));
            return DWGraph::weight_t(d*factor);
        }
    };

    class DistanceHeuristicFew : public AstarFew::heuristic_t {
    private:
        const std::unordered_map<DWGraph::node_t, Coord> &nodes;
        Coord center;
        double dist;
        double factor;
    public:
        DistanceHeuristicFew(
            const std::unordered_map<DWGraph::node_t, Coord> &nodes_,
            Coord center_, double dist_, double factor_
        ): nodes(nodes_), center(center_), dist(dist_), factor(factor_){}
        DWGraph::weight_t operator()(DWGraph::node_t u) const{
            auto d = std::max(Coord::getDistanceArc(center, nodes.at(u)) - dist, 0.0);
            return DWGraph::weight_t(d*factor);
        }
    };
};
