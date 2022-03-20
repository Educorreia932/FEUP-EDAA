#include "DUGraph.h"

DUGraph::DUGraph(){}

void DUGraph::addNode(node_t u){
    if(adj.find(u) != adj.end())
        throw std::invalid_argument("Node already exists");
    nodes.push_back(u);
    adj[u] = std::list<node_t>();
}

void DUGraph::addEdge(DUGraph::node_t u, DUGraph::node_t v){
    if(adj.find(u) == adj.end() || adj.find(v) == adj.end())
        throw std::invalid_argument("Node does not exist");
    adj[u].push_back(v);
}

const std::list<DUGraph::node_t>& DUGraph::getNodes() const{
    return nodes;
}

const std::list<DUGraph::node_t>& DUGraph::getAdj(DUGraph::node_t u) const{
    return adj.at(u);
}

DUGraph DUGraph::getTranspose() const{
    DUGraph ret;
    for(const node_t &u: getNodes()) ret.addNode(u);
    for(const node_t &u: getNodes())
        for(const node_t &v: getAdj(u))
            ret.addEdge(v, u);
    return ret;
}
