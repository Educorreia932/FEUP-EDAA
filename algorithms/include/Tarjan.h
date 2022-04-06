#pragma once

#include "SCC.h"
#include <bits/stdc++.h>

/**
 * @brief Tarjan algorithm
 * 
 */
class Tarjan : public SCC{
private:
    const DUGraph *G;
    int nid; //=1
    std::unordered_map<DUGraph::node_t, DUGraph::node_t> id;
    std::unordered_map<DUGraph::node_t, DUGraph::node_t> low;
    std::unordered_set<DUGraph::node_t> onStack;
    std::stack<DUGraph::node_t> L;
    std::unordered_map<DUGraph::node_t, DUGraph::node_t> SCCs;
public:
    /**
     * @brief Construct without arguments
     * 
     */
    Tarjan();

    /**
     * @brief Initializes data members that will be used in the algorithm's execution
     * 
     * @param G     Directed Unweighted Graph
     */
    void initialize(const DUGraph *G_);

    /**
     * @brief Depth-first Search for Tarjan
     * 
     * @param u Starting Node
     */
    void DFS_T(DUGraph::node_t u);

    /**
     * @brief Execute the algorithm
     * 
     */
    void run();

    /**
     * @brief Retrieves the root of the scc that the parameter Node belongs to
     * 
     * @param v                 Node to operate over
     * @return DUGraph::node_t  Node that is the root of the parameter's scc
     */
    DUGraph::node_t get_scc(DUGraph::node_t u) const;
};
