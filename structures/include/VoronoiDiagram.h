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
    bool finished = false;
    double m; // Gradient
    double c; // Y-axis intercept
    Vector2 direction;
    Edge* adjacent = nullptr;
    Site* site_up;
    Site* site_down;

    Edge();
    Edge(Vector2 start, Vector2 end);
    Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint);

    bool intersect(Edge edge, Vector2& intersection);
    Edge merge();
    double evaluateY(double x) const;
    
    bool operator<(const Edge& edge) const;
    bool operator==(const Edge& edge) const;
};

class Box {
public:
    Edge bounds[4];

    Box(Vector2 bottom_left, Vector2 upper_right);

    bool intersect(Edge edge, Vector2 &intersection);
};

class VoronoiDiagram {
private:
    std::vector<Edge> edges;
public:
    void addEdge(Edge segment);
    std::vector<Edge> getEdges() const;
};

#endif