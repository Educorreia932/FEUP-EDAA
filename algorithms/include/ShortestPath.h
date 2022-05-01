#pragma once

#include <list>

#include "DWGraph.h"
#include "ShortestPathOneMany.h"

/**
 * @brief Shortest Path Interface
 * 
 */
class ShortestPath {
public:

    virtual ~ShortestPath();

    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     * @param d Destination Node
     */
    virtual void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, DWGraph::node_t d) = 0;

    virtual DWGraph::node_t getStart() const = 0;
    virtual DWGraph::node_t getDest () const = 0;

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
    virtual std::list<DWGraph::node_t> getPath() const final;

    virtual DWGraph::weight_t getPathWeight() const = 0;

    /**
     * @brief Shortest Path Between every of the Nodes provided and any other Node
     * 
     */
    class FromOneMany;

    /**
     * @brief           Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    virtual bool hasVisited(DWGraph::node_t u) const = 0;
};

class ShortestPath::FromOneMany : public ShortestPath{
private:
    ShortestPathOneMany *oneMany = nullptr;
    const DWGraph::DWGraph *G = nullptr;
    DWGraph::node_t s, d;
public:
    /**
     * @brief Constructor
     * 
     * @param oneMany   Method of Shortest Path One Many to Use
     */
    FromOneMany(ShortestPathOneMany *oneMany);

    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G     Directed Weighted Graph
     * @param s     Start Node
     * @param d     Destination Node
     */
    virtual void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, DWGraph::node_t d);

    /**
     * @brief Execute the algorithm
     * 
     */
    virtual void run();

    virtual DWGraph::node_t getStart() const;

    virtual DWGraph::node_t getDest () const;

    /**
     * @brief Retrieves the node chosen prior to getting to node d
     * 
     * @param u                 Destination Node
     * @return DWGraph::node_t  Last Node before getting to the destination Node
     */
    virtual DWGraph::node_t getPrev(DWGraph::node_t u) const;

    virtual DWGraph::weight_t getPathWeight() const;

    /**
     * @brief           Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    virtual bool hasVisited(DWGraph::node_t u) const;
};
