#pragma once

#include "ShortestPath.h"
#include <unordered_map>

/**
 * @brief AStar algorithm
 * 
 */
class Astar : public ShortestPath {
public:
    /**
     * @brief Heuristic Interface
     * 
     */
    class heuristic_t {
    public:
        virtual ~heuristic_t();

        /**
         * @brief Heuristic Function 
         * 
         * @param u                     Node to analyse
         * @return DWGraph::weight_t    Estimated distance/weight from u to the Destination Node
         */
        virtual DWGraph::weight_t operator()(DWGraph::node_t u) const = 0;
    };
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
    const DWGraph::DWGraph *G;
    DWGraph::node_t s, d;
    std::unordered_map<DWGraph::node_t, DWGraph::weight_t> dist;
    std::unordered_map<DWGraph::node_t, DWGraph::weight_t> hdist;
    std::unordered_map<DWGraph::node_t, DWGraph::node_t  > prev;
public:

    /**
     * @brief Construct from a heuristic
     * 
     * @param h heuristic to use
     */
    Astar(const heuristic_t *h);

    /**
     * @brief Construct without arguments
     * 
     */
    Astar();
    
    /**
     * @brief Initializes the data members that are required for the algorithm's execution
     * 
     * @param G Directed Weighted Graph
     * @param s Starting Node
     * @param d Destination Node
     */
    void initialize(const DWGraph::DWGraph *G, DWGraph::node_t s, DWGraph::node_t d);

    DWGraph::node_t getStart() const;
    DWGraph::node_t getDest () const;

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
    DWGraph::weight_t getPathWeight() const;

    /**
     * @brief           Checks if a specific node was marked as visited
     * 
     * @param u         Node to be checked
     * @return true     If the node has been already visited
     * @return false    Otherwise
     */
    bool hasVisited(DWGraph::node_t u) const;
};
