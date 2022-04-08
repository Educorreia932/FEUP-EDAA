#pragma once

#include "ShortestPathFew.h"

#include <unordered_map>

#include "Astar.h"

/**
 * @brief AStar algorithm
 * 
 */
class AstarFew : public ShortestPathFew {
public:
    typedef Astar::heuristic_t heuristic_t;
private:
    /**
     * @brief Default Heuristic
     * 
     */
    class default_heuristic : public heuristic_t {
    public:
        default_heuristic();
        DWGraph::weight_t operator()(DWGraph::node_t u) const;
    };
    static const default_heuristic h_default;
    const heuristic_t *h;
    const DWGraph::weight_t dMax;
    const DWGraph::DWGraph *G;
    DWGraph::node_t s;
    std::list<DWGraph::node_t> d;
    std::unordered_map<DWGraph::node_t, std::pair<DWGraph::weight_t, DWGraph::node_t>> dist;
public:
    /**
     * @brief Construct from a heuristic and max distance
     * 
     * @param h heuristic to use
     */
    AstarFew(const heuristic_t *h_, DWGraph::weight_t dMax_);

    /**
     * @brief Construct from a heuristic
     * 
     * @param h heuristic to use
     */
    AstarFew(const heuristic_t *h_);

    /**
     * @brief Construct without arguments
     * 
     */
    AstarFew();
    
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
