#include "Tarjan.h"
#include <iostream>

typedef DUGraph::node_t node_t;

Tarjan::Tarjan(){}

void Tarjan::initialize(const DUGraph *G_){
    this->G = G_;
    this->nid = 1;
    this->id.clear();
    this->low.clear();
    this->onStack.clear();
    this->SCCs.clear();
}

void Tarjan::DFS_T(DUGraph::node_t u){
    L.push(u); onStack.insert(u);

    id[u] = nid; 
    low[u] = nid;
    ++nid;

    for (node_t v : G->getAdj(u)){

        if (id.find(v) == id.end()){
            DFS_T(v);
            low[u] = std::min(low.at(u), low.at(v));
        }
        else if (onStack.find(v) != onStack.end()){
            low[u] = std::min(low.at(u), id.at(v));
        }
    }

    if (low.at(u) == id.at(u)){
        while(!L.empty()){
            node_t v = L.top();
            L.pop(); onStack.erase(v);

            SCCs[v] = u;
            if (v == u) break;
        }
    }
}

void Tarjan::run(){
    for (node_t u : G->getNodes())
        if (id.find(u) == id.end()) DFS_T(u);  
}

node_t Tarjan::get_scc(node_t u) const{
    return SCCs.at(u);
}
