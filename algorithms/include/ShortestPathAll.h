#ifndef SHORTESTPATHALL_H_INCLUDED
#define SHORTESTPATHALL_H_INCLUDED

#include <list>
#include <vector>
#include <queue>
#include <thread>

#include "DWGraph.h"
#include "ShortestPathOneMany.h"
#include "SharedQueue.h"

/**
 * @brief Shortest Path Between Each Pair of Vertices Interface
 * 
 */
class ShortestPathAll {
public:
    virtual ~ShortestPathAll();

    /**
     * @brief Initializes the data members that are required for the algorithm's execution such as the G graph
     * 
     * @param G Directed Weighted Graph
     */
    virtual void initialize(const DWGraph::DWGraph *G) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    /**
     * @brief Retrieves the node chosen prior to getting to node d from s
     * 
     * @param s                     Starting Node
     * @param d                     Destination Node
     * @return DWGraph::node_t      Last Node before getting to the destination Node
     */
    virtual DWGraph::node_t getPrev(DWGraph::node_t s, DWGraph::node_t d) const = 0;

    /**
     * @brief Retrieves the sequence of nodes of the path starting at node s and ending at d
     * 
     * @param s                             Starting Node
     * @param d                             Destination Node
     * @return std::list<DWGraph::node_t>   Sequence of nodes that describe the path from Node s to d
     */
    virtual std::list<DWGraph::node_t> getPath(DWGraph::node_t s, DWGraph::node_t d) const final;

    virtual DWGraph::weight_t getPathWeight(DWGraph::node_t s, DWGraph::node_t d) const = 0;

    /**
     * @brief Shortest Path Between every of the Nodes provided and any other Node
     * 
     */
    class FromOneMany;
};

class ShortestPathAll::FromOneMany : public ShortestPathAll {
private:
    size_t nthreads;
    std::list<DWGraph::node_t> nodes;
    utils::SharedQueue<DWGraph::node_t> Q;
    std::vector< std::thread > threads;
    ShortestPathOneManyFactory &oneManyFactory;
    std::unordered_map<DWGraph::node_t, std::unordered_map<DWGraph::node_t, DWGraph::weight_t> > dist;
    const DWGraph::DWGraph *G = nullptr;

    /**
     * @brief Function to be executed by a thread
     * 
     * @param p Shortest Path All Object
     */
    static void thread_func(ShortestPathAll::FromOneMany *p);
public:
    /**
     * @brief Construct from oneMany and nthreads
     * 
     * @param oneMany  Class instance
     * @param nthreads Number of Threads to Use
     */
    FromOneMany(ShortestPathOneManyFactory &oneManyFactory_, size_t nthreads_);

    /**
     * @brief Initializes the graph data member G and the set of nodes to be used as sources
     * 
     * @param G 
     * @param nodes 
     */
    void initialize(const DWGraph::DWGraph *G, const std::unordered_set<DWGraph::node_t> &nodes);
    
    /**
     * @brief Initializes the graph data member G 
     * 
     * @param G Directed Weighted Graph
     */
    void initialize(const DWGraph::DWGraph *G);

    /**
     * @brief Executes the algorithm computing paths for pairs of vertices
     * 
     */
    void run();

    /**
     * @brief Retrieves the node chosen prior to getting to node d from s
     * 
     * @param s                     Starting Node
     * @param d                     Destination Node
     * @return DWGraph::node_t      Last Node before getting to the destination Node
     */
    DWGraph::node_t getPrev(DWGraph::node_t s, DWGraph::node_t d) const;

    DWGraph::weight_t getPathWeight(DWGraph::node_t s, DWGraph::node_t d) const;
};

#endif //SHORTESTPATHALL_H_INCLUDED
