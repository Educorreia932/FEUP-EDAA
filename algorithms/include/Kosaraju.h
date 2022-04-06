#ifndef KOSARAJU_H_INCLUDED
#define KOSARAJU_H_INCLUDED

#include "SCC.h"
#include <bits/stdc++.h> 

/**
 * @brief Kosaraju algorithm
 * 
 */
class Kosaraju : public SCC{
private:
    const DUGraph *G;
    std::unordered_set<DUGraph::node_t> S;
    std::stack<DUGraph::node_t> L;
    std::unordered_map<DUGraph::node_t, DUGraph::node_t> SCCs;
public:
    /**
     * @brief Construct without arguments
     * 
     */
    Kosaraju();

    /**
     * @brief Initializes data members that will be used in the algorithm's execution
     * 
     * @param G     Directed Unweighted Graph
     */
    void initialize(const DUGraph *G);

    /**
     * @brief Depth-first Search for Kosaraju
     * 
     * @param u Starting Node
     */
    void DFS_K(DUGraph::node_t u);

    void assign(DUGraph::node_t u, DUGraph::node_t root);

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

#endif //KOSARAJU_H_INCLUDED
