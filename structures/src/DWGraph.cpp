#include "DWGraph.h"

#include <vector>

DWGraph::Edge::Edge(node_t v_, weight_t w_):v(v_),w(w_){}

void DWGraph::DWGraph::assert_integrity() const{
    if(nodes.size() != adj .size()) throw std::logic_error("");
    if(nodes.size() != pred.size()) throw std::logic_error("");
    std::unordered_set<node_t> nodes_s(nodes.begin(), nodes.end());
    if(nodes.size() != nodes_s.size()) throw std::logic_error("");
    for(const auto &Adj: adj){
        if(!nodes_s.count(Adj.first)) throw std::logic_error("");
        if(!pred.count(Adj.first)) throw std::logic_error("");
        for(const auto &e: Adj.second){
            if(!nodes_s.count(e.v)) throw std::logic_error("");
        }
    }
}

DWGraph::DWGraph::DWGraph(){}

DWGraph::DWGraph::~DWGraph(){}

void DWGraph::DWGraph::addNode(node_t u){
    if(adj.find(u) != adj.end())
        throw std::invalid_argument("Node already exists");
    nodes.insert(u);
    adj[u] = std::unordered_set<Edge>();
    pred[u] = std::unordered_set<node_t>();
}

void DWGraph::DWGraph::removeNode(node_t u){
    for(const Edge &e: adj.at(u)){
        pred.at(e.v).erase(u);
    }
    for(const node_t &v: pred.at(u)){
        adj.at(v).erase(Edge(u, 0));
    }
    nodes.erase(u);
    adj.erase(u);
    pred.erase(u);
}

bool DWGraph::DWGraph::hasNode(node_t u) const{
    return (adj.count(u));
}

void DWGraph::DWGraph::addEdge(node_t u, node_t v, weight_t w){
    Edge e(v, w);
    if(adj.at(u).count(e)) throw std::invalid_argument("Edge already exists");
    adj.at(u).insert(e);
    pred.at(v).insert(u);
}

void DWGraph::DWGraph::addBestEdge(node_t u, node_t v, weight_t w){
    Edge e(v, w);
    auto it = adj.at(u).find(e);
    if(it != adj.at(u).end()) e.w = std::min(e.w, it->w);
    adj.at(u).insert(e);
    pred.at(v).insert(u);
}

const std::unordered_set<DWGraph::node_t>& DWGraph::DWGraph::getNodes() const{
    return nodes;
}

const std::unordered_set<DWGraph::Edge>& DWGraph::DWGraph::getAdj(node_t u) const{
    return adj.at(u);
}

size_t DWGraph::DWGraph::getNumberEdges() const{
    size_t ret = 0;
    for(const node_t &u: getNodes()){
        ret += getAdj(u).size();
    }
    return ret;
}

DWGraph::DWGraph DWGraph::DWGraph::getTranspose() const{
    DWGraph ret;
    for(const node_t &u: getNodes()) ret.addNode(u);
    for(const node_t &u: getNodes())
        for(const Edge &e: getAdj(u))
            ret.addEdge(e.v, u, e.w);
    return ret;
}

DWGraph::weight_t DWGraph::DWGraph::getPathWeight(const std::list<node_t> &path) const{
    if(path.empty()) return INF;
    weight_t ret = 0;
    auto it = path.begin();
    auto prev = it++;
    for(; it != path.end(); ++it, ++prev){
        const node_t &u = *prev, &v = *it;
        auto it2 = adj.at(u).find(Edge(v, 0));
        if(it2 == adj.at(u).end()) throw std::invalid_argument("No such edge");
        ret += it2->w;
    }
    return ret;
}

DWGraph::DWGraph::operator DUGraph() const{
    DUGraph G;
    for(const node_t &u: getNodes()){
        G.addNode(u);
    }
    for(const node_t &u: getNodes()){
        for(const Edge &e: getAdj(u)){
            G.addEdge(u, e.v);
        }
    }
    return G;
}
