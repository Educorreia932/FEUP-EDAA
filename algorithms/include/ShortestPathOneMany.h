#pragma once

#include <list>

#include "DWGraph.h"

/**
 * @brief Shortest Path From One Node to All other Nodes (Shortest Path One Many Interface)
 * 
 */
class ShortestPathOneMany {
private:

    virtual DWGraph::node_t getStart() const = 0;
public:

    virtual ~ShortestPathOneMany();

    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     */
    virtual void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s) = 0;

    /**
     * @brief Execute the algorithm
     * 
     */
    virtual void run() = 0;

    /**
     * @brief Retrieves the node chosen prior to getting to node d
     * 
     * @param d                 Destination Node
     * @return DWGraph::node_t  Last Node before getting to the destination Node
     */
    virtual DWGraph::node_t getPrev(DWGraph::node_t d) const = 0;

    /**
     * @brief Retrieves the sequence of nodes of the path ending at d
     * 
     * @param d Destination Node
     * @return std::list<DWGraph::node_t> Sequence of nodes that describe the path to d
     */
    virtual std::list<DWGraph::node_t> getPath(DWGraph::node_t d) const final;

    virtual DWGraph::weight_t getPathWeight(DWGraph::node_t d) const = 0;

    /**
     * @brief Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    virtual bool hasVisited(DWGraph::node_t u) const = 0;
};
