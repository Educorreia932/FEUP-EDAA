#include "VoronoiDiagram.h"

void VoronoiDiagram::addEdge(Edge segment) {
    edges.push_back(segment);
}


Edge::Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint) : start(start) {
    this->m = -1.0 / ((leftpoint.y - rightpoint.y) / (leftpoint.x - rightpoint.x));
    this->c = start.y - this->m * start.x;
    this->direction = Vector2(-1.0 * (leftpoint.y - rightpoint.y), leftpoint.x - rightpoint.x);
}

Vector2 Edge::intersect(Edge edge) {
    // Edges are parallel
    if (this->m == edge.m)
        return NULL;

    double x = (this->c - edge.c) / (this->m - edge.m);
    double y = this->m * x + this->c;

    // Edges have a direction, intersection is on that side of the edge
    if ((x - this->start.x) / this->direction.x < 0)
        return NULL;

    if ((y - this->start.y) / this->direction.y < 0)
        return NULL;

    if ((x - edge.start.x) / edge.direction.x < 0)
        return NULL;

    if ((y - this->start.y) / edge.direction.y < 0)
        return NULL;

    return Vector2(x, y);
}
