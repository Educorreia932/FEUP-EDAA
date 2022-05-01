#pragma once

#include "Box.h"
#include "Edge.h"
#include "Vector2.h"

#include <queue>
#include <vector>

class Edge;

class VoronoiDiagram {
private:
    std::vector<Edge> edges;
public:
    std::vector<Site*> sites;
    Box bounding_box;

    VoronoiDiagram(std::vector<Site*> sites);

    void addEdge(Edge segment);
    std::vector<Edge> getEdges() const;
};
