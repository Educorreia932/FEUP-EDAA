#include "VoronoiDiagram.h"

void VoronoiDiagram::addEdge(Edge segment) {
    edges.push_back(segment);
}


Edge::Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint) : start(start) {
    this->m = -1.0 / ((leftpoint.y - rightpoint.y) / (leftpoint.x - rightpoint.x));
}

Vector2 Edge::intersect(Edge edge) {
    return Vector2();
}
