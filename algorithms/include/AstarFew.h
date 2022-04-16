#pragma once

#include "ShortestPathFew.h"

#include <unordered_map>

#include "Astar.h"
#include "Coord.h"

/**
 * @brief AStar algorithm
 * 
 */
class AstarFew : public ShortestPathFew {
private:
    const std::unordered_map<DWGraph::node_t, Coord> &nodesToCoords;
    const double factor;

    const Astar::heuristic_t *h = nullptr;
    
    const DWGraph::DWGraph *G;
    DWGraph::node_t s;
    std::list<DWGraph::node_t> d;
    const DWGraph::weight_t dMax;
    std::unordered_map<DWGraph::node_t, std::pair<DWGraph::weight_t, DWGraph::node_t>> dist;
public:
    /**
     * @brief Construct without arguments
     * 
     */
    AstarFew(
        const std::unordered_map<DWGraph::node_t, Coord> &nodesToCoords_,
        double factor_,
        DWGraph::weight_t dMax_
    );
    
    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     * @param d Destination Node
     */
    void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, std::list<DWGraph::node_t> d);

    DWGraph::node_t getStart() const;
    std::list<DWGraph::node_t> getDest () const;

    /**
     * @brief Execute the algorithm
     * 
     */
    void run();

    /**
     * @brief Retrieves the node chosen prior to getting to node u
     * 
     * @param u Destination Node
     * @return DWGraph::node_t Last Node before getting to u
     */
    DWGraph::node_t getPrev(DWGraph::node_t u) const;
    DWGraph::weight_t getPathWeight(DWGraph::node_t u) const;

    /**
     * @brief           Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    bool hasVisited(DWGraph::node_t u) const;
};
