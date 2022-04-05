#pragma once

#include "ShortestPathOneMany.h"

#include <unordered_map>

/**
 * @brief Dijkstra's algorithm
 * 
 */
class Dijkstra : public ShortestPathOneMany {
private:
    const DWGraph::DWGraph *G;
    DWGraph::node_t s;
    std::unordered_map<DWGraph::node_t, DWGraph::weight_t> dist;
    std::unordered_map<DWGraph::node_t, DWGraph::node_t> prev;
    DWGraph::node_t getStart() const;
public:
    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     */
    void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s);

    /**
     * @brief Execute the algorithm
     * 
     */
    void run();

    /**
     * @brief Retrieves the node chosen prior to getting to node d
     * 
     * @param d                 Destination Node
     * @return DWGraph::node_t  Last Node before getting to the destination Node
     */
    DWGraph::node_t getPrev(DWGraph::node_t d) const;
    DWGraph::weight_t getPathWeight(DWGraph::node_t d) const;

    /**
     * @brief Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    bool hasVisited(DWGraph::node_t u) const;
};
