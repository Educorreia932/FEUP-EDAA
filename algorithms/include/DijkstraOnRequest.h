#pragma once

#include "ShortestPathAll.h"

#include "DijkstraDist.h"
#include "DWGraph.h"

class DijkstraOnRequest : public ShortestPathAll {
private:
    const DWGraph::DWGraph *G;
    mutable std::unordered_map<DWGraph::node_t, DijkstraDist> dijkstras;
public:
    virtual void initialize(const DWGraph::DWGraph *G);
    virtual void run();
    virtual DWGraph::node_t getPrev(DWGraph::node_t s, DWGraph::node_t d) const;
    virtual DWGraph::weight_t getPathWeight(DWGraph::node_t s, DWGraph::node_t d) const;
};
