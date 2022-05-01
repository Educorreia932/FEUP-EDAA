#pragma once

#include <list>

#include "DWGraph.h"
#include "ShortestPathAll.h"

/**
 * @brief Shortest Path Interface
 * 
 */
class ShortestPathFew {
public:

    virtual ~ShortestPathFew();

    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     * @param d Destination Node
     */
    virtual void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, std::list<DWGraph::node_t> d) = 0;

    virtual DWGraph::node_t getStart() const = 0;
    virtual std::list<DWGraph::node_t> getDest () const = 0;

    /**
     * @brief Execute the algorithm
     * 
     */
    virtual void run() = 0;

    /**
     * @brief Retrieves the node chosen prior to getting to node u
     * 
     * @param u Destination Node
     * @return DWGraph::node_t Last Node before getting to u
     */
    virtual DWGraph::node_t getPrev(DWGraph::node_t u) const = 0;

    /**
     * @brief Retrieves the sequence of nodes of the path between two nodes
     * 
     * @return std::list<DWGraph::node_t> Sequence of nodes that describe the path between two nodes
     */
    virtual std::list<DWGraph::node_t> getPath(DWGraph::node_t u) const final;

    virtual DWGraph::weight_t getPathWeight(DWGraph::node_t u) const = 0;

    /**
     * @brief           Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    virtual bool hasVisited(DWGraph::node_t u) const = 0;

    class FromAll;
};

class ShortestPathFew::FromAll : public ShortestPathFew {
private:
    ShortestPathAll &shortestPathAll;
    DWGraph::node_t s;
public:
    FromAll(ShortestPathAll &shortestPathAll_);
    virtual void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, std::list<DWGraph::node_t> d);
    virtual void run();
    virtual DWGraph::node_t getStart() const;
    virtual std::list<DWGraph::node_t> getDest () const;
    virtual DWGraph::node_t getPrev(DWGraph::node_t d) const;
    virtual DWGraph::weight_t getPathWeight(DWGraph::node_t d) const;
    virtual bool hasVisited(DWGraph::node_t u) const;
};
