#pragma once

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
    Edge(Vector2 start, Site* site_1, Site* site_2);

    bool intersect(Edge edge, Vector2& intersection);
    Edge merge();
    double evaluateY(double x) const;
    
    bool operator==(const Edge& edge) const;
};

class Box {
public:
    Edge bounds[4];

    Box(Vector2 bottom_left, Vector2 upper_right);

    bool intersect(Edge edge, Vector2 &intersection);
    bool contains(Vector2 point);
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
