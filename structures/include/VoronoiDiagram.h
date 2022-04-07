#pragma once

#include "Edge.h"
#include "Vector2.h"

#include <queue>
#include <vector>

struct Site {
    Vector2 point;
};

class VoronoiDiagram {
private:
    std::vector<Edge> edges;
public:
    std::vector<Site*> sites;

    VoronoiDiagram(std::vector<Site*> sites);

    void addEdge(Edge segment);
    std::vector<Edge> getEdges() const;
};
