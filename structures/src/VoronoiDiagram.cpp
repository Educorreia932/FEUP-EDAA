#include "VoronoiDiagram.h"

void VoronoiDiagram::addEdge(Edge segment) {
    edges.push_back(segment);
}


Edge::Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint) : start(start) {
    this->m = -1.0 / ((leftpoint.y - rightpoint.y) / (leftpoint.x - rightpoint.x));
    this->c = start.y - this->m * start.x;
    this->direction = Vector2(-1.0 * (leftpoint.y - rightpoint.y), leftpoint.x - rightpoint.x);
}

bool Edge::intersect(Edge edge, Vector2 &intersection) {
    // Edges are parallel
    if (this->m == edge.m)
        return false;

    double x = (this->c - edge.c) / (this->m - edge.m);
    double y = this->m * x + this->c;

    // Edges have a direction, intersection is on that side of the edge
    if ((x - this->start.x) / this->direction.x < 0)
        return false;

    if ((y - this->start.y) / this->direction.y < 0)
        return false;

    if ((x - edge.start.x) / edge.direction.x < 0)
        return false;

    if ((y - this->start.y) / edge.direction.y < 0)
        return false;

    intersection = Vector2(x, y);

    return true;
}

double Edge::evaluateY(double x) const {
    double w = (x - start.x)/(end.x - start.x);
    double y = (1-w)*start.y + (w)*end.y;
    return y;
}
