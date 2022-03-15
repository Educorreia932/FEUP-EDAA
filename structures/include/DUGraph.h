#ifndef DUGRAPH_H_INCLUDED
#define DUGRAPH_H_INCLUDED

#include <cstddef>
#include <unordered_map>
#include <list>

/**
 * @brief Directed Unweighted Graph Interface
 * 
 */
class DUGraph {
public:
    typedef size_t node_t;
private:
    std::list<node_t> nodes;
    std::unordered_map< node_t, std::list<node_t> > adj;
public:
    /**
     * @brief Construct without arguments
     * 
     */
    DUGraph();

    /**
     * @brief Adds a Node to the graph
     * 
     * @param u Node to be added to graph
     */
    void addNode(node_t u);

    /**
     * @brief Adds an Edge to the graph
     * 
     * @param u Node at which the Edge begins
     * @param v Node at which the Edge ends
     */
    void addEdge(node_t u, node_t v);

    /**
     * @brief Retrieves the List of Nodes of the graph
     * 
     * @return const std::list<node_t>& List of Nodes of the graph
     */
    const std::list<node_t>& getNodes() const;

    /**
     * @brief Retrieves the list of nodes that adjacent to u 
     * 
     * @param u Node to consider
     * @return const std::list<node_t>& List of Adjacent Nodes
     */
    const std::list<node_t>& getAdj(node_t u) const;

    /**
     * @brief Retrieves the transposed graph
     * 
     * @return DUGraph Transposed graph
     */
    DUGraph getTranspose() const;
};

#endif //DUGRAPH_H_INCLUDED
