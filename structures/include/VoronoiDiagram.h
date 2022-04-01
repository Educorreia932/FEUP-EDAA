#ifndef VORONOI
#define VORONOI

#include "Vector2.h"

#include <queue>
#include <vector>

struct Site {
    Vector2 point;
};

class Edge {
public:
    Vector2 start;
    Vector2 end;
    double m; // Gradient
    Edge* adjacent = nullptr;

    Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint);

    Vector2 intersect(Edge edge);
};

class VoronoiDiagram {
private:
    std::vector<Edge> edges;
public:
    void addEdge(Edge segment);
};

#endif