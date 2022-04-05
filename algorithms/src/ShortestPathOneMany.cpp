#include "ShortestPathOneMany.h"

ShortestPathOneMany::~ShortestPathOneMany(){}

std::list<DWGraph::node_t> ShortestPathOneMany::getPath(DWGraph::node_t d) const{
    std::list<DWGraph::node_t> res;
    while(d != getStart()){
        res.push_front(d);
        d = getPrev(d);
        if(d == DWGraph::INVALID_NODE) return std::list<DWGraph::node_t>();
    }
    res.push_front(d);
    return res;
}
