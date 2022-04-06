#include "Kosaraju.h"

#include <iostream>

using namespace std;

typedef DUGraph::node_t node_t;

Kosaraju::Kosaraju(){}

void Kosaraju::initialize(const DUGraph *G_){
    G = G_;
    GT = G->getTranspose();
    S.clear();
    SCCs.clear();
    L = stack<DUGraph::node_t>();
}

void Kosaraju::DFS_K(DUGraph::node_t u){
    if (S.find(u) != S.end()) return;
    S.insert(u);
    for (node_t v : G->getAdj(u)) DFS_K(v);
    L.push(u);
}

void Kosaraju::assign(DUGraph::node_t u, DUGraph::node_t root){
    if (SCCs.find(u) != SCCs.end()) return;
    SCCs[u] = root;
    for (node_t v : GT.getAdj(u)) {
        assign(v, root);
    }
}

void Kosaraju::run(){
    for (node_t u : G->getNodes()) DFS_K(u);
    while(!L.empty()){
        node_t u = L.top();
        assign(u,u);
        L.pop();
    }   
}

node_t Kosaraju::get_scc(node_t u) const{
    return SCCs.at(u);
}
