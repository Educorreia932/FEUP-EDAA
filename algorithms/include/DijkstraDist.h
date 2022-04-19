#pragma once

#include "ShortestPathOneMany.h"

#include <unordered_map>

#include "utils.h"

/**
 * @brief Dijkstra's algorithm
 * 
 */
class DijkstraDist : public ShortestPathOneMany {
private:
    const DWGraph::DWGraph *G;
    DWGraph::node_t s;
    const DWGraph::weight_t dMax;
    std::unordered_map<DWGraph::node_t, DWGraph::weight_t> dist;
    DWGraph::node_t getStart() const;
public:
    DijkstraDist(DWGraph::weight_t dMax_ = iINF);

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
