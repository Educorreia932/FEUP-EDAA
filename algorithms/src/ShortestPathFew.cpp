#include "ShortestPathFew.h"

typedef DWGraph::node_t     node_t;
typedef DWGraph::weight_t   weight_t;

ShortestPathFew::~ShortestPathFew(){}

std::list<node_t> ShortestPathFew::getPath(node_t u) const{
    std::list<node_t> res;
    node_t d = u;
    while(d != getStart()){
        res.push_front(d);
        d = getPrev(d);
    }
    res.push_front(d);
    return res;
}
