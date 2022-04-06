#ifndef SCC_H_INCLUDED
#define SCC_H_INCLUDED

#include "DUGraph.h"

/**
 * @brief Strongly Connected Component Interface
 * 
 */
class SCC{
public:

    /**
     * @brief Initializes data members that will be used in the algorithm's execution
     * 
     * @param G     Directed Unweighted Graph
     */
    virtual void initialize(const DUGraph *G) = 0;
    
    /**
     * @brief Execute the algorithm
     * 
     */
    virtual void run() = 0;

    /**
     * @brief Retrieves the root of the scc that the parameter Node belongs to
     * 
     * @param v                 Node to operate over
     * @return DUGraph::node_t  Node that is the root of the parameter's scc
     */
    virtual DUGraph::node_t get_scc(DUGraph::node_t v) const = 0;
};

#endif //SCC_H_INCLUDED
