#ifndef DWGRAPH_H_INCLUDED
#define DWGRAPH_H_INCLUDED

#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <utility>
#include <limits>

#include "DUGraph.h"

/**
 * @brief Directed Weighted Graph Namespace
 * 
 */
namespace DWGraph {
    typedef long long node_t;
    static const node_t INVALID_NODE =std::numeric_limits<node_t>::max();
    typedef long long weight_t;
    static constexpr weight_t INF = 1000000000000;

    struct Edge{
        node_t v;
        weight_t w;

        /**
         * @brief Edge Constructor
         * 
         * @param v Node at which the Edge begins
         * @param w Node at which the Edge ends
         */
        Edge(node_t v, weight_t w);

        /**
         * @brief Equality Operator for Edge
         * 
         * @param e         Edge to check
         * @return true     If the two Edges are the same
         * @return false    Otherwise
         */
        bool operator==(const Edge &e) const{
            return v == e.v;
        }
    };
}

namespace std{
    /**
     * @brief template specialization for custom class DWGraph::Edge
     * 
     * @tparam  
     */
    template<> struct hash<DWGraph::Edge>{
        std::size_t operator()(const DWGraph::Edge &e) const{
            return std::hash<DWGraph::node_t>()(e.v);
        }
    };
}

/**
 * @brief Directed Weighted Graph Namespace
 * 
 */
namespace DWGraph{
    class DWGraph {
    private:
        std::unordered_set<node_t> nodes;
        std::unordered_map< node_t, std::unordered_set< Edge > > adj;
        std::unordered_map< node_t, std::unordered_set< node_t> > pred;

        /**
         * @brief Checks coherence
         * 
         */
        void assert_integrity() const;
    public:
        /**
         * @brief DWGraph Constructor
         * 
         */
        DWGraph();

        /**
         * @brief DWGraph Destructor
         * 
         */
        ~DWGraph();

        /**
         * @brief Adds a Node to the graph
         * 
         * @param u Node to be added to graph
         */
        void addNode(node_t u);

        template<class iterator_t>

        /**
         * @brief Removes nodes 
         * 
         * @param it1   Starting Element to remove
         * @param it2   Next to Last Element to remove
         */
        void removeNodes(iterator_t it1, iterator_t it2);

        /**
         * @brief Removes a Node from the graph
         * 
         * @param u Node to be removed
         */
        void removeNode(node_t u);

        /**
         * @brief Checks existence of a Node
         * 
         * @param u         Node whose existence is to be checked
         * @return true     If the Node exists in the graph
         * @return false    Otherwise
         */
        bool hasNode(node_t u) const;

        /**
         * @brief Adds an Edge
         * 
         * @param u     Starting Node of the Edge
         * @param v     Destination Node of the Edge
         * @param w     Weight of the Edge
         */
        void addEdge(node_t u, node_t v, weight_t w);
        
        /**
         * @brief Adds an Edge and in case it already exists replaces the weight with the lowest of the two values
         * 
         * @param u     Starting Node of the Edge
         * @param v     Destination Node of the Edge
         * @param w     Weight of the Edge
         */
        void addBestEdge(node_t u, node_t v, weight_t w);

        /**
         * @brief Retrieves the List of Nodes of the graph
         * 
         * @return const std::unordered_set<node_t>& List of Nodes of the graph
         */
        const std::unordered_set<node_t>& getNodes() const;

        /**
         * @brief Retrieves the list of nodes that adjacent to u
         * 
         * @param u Node to consider
         * @return const std::unordered_set<Edge>& List of Adjacent Nodes
         */
        const std::unordered_set<Edge>& getAdj(node_t u) const;

        /**
         * @brief Retrieves the number of Edges of the graph
         * 
         * @return size_t Number of Edges
         */
        size_t getNumberEdges() const;

        /**
         * @brief Retrieves the transposed graph
         * 
         * @return DWGraph Transposed graph
         */
        DWGraph getTranspose() const;

        /**
         * @brief Get the Path Weight object
         * 
         * @param path 
         * @return weight_t 
         */
        weight_t getPathWeight(const std::list<node_t> &path) const;

        explicit operator DUGraph() const;
    };
}

template<class iterator_t>

void DWGraph::DWGraph::removeNodes(iterator_t it1, iterator_t it2){
    for(auto it = it1; it != it2; ++it)
        removeNode(*it);
}

#endif //DWGRAPH_H_INCLUDED
