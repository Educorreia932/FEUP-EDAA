#include "VoronoiDiagram.h"

#include <algorithm>
#include <limits>

Edge::Edge() {

}

Edge::Edge(Vector2 start, Vector2 end) : start(start), end(end) {
    this->m = (end.y - start.y) / (end.x - start.x);
    this->c = start.y - this->m * start.x;
    this->direction = Vector2(end.x - start.x, end.y - start.y);
}

Edge::Edge(Vector2 start, Vector2 leftpoint, Vector2 rightpoint) : start(start) {
    this->m = -1.0 / ((leftpoint.y - rightpoint.y) / (leftpoint.x - rightpoint.x));
    this->c = start.y - this->m * start.x;
    this->direction = Vector2(-1.0 * (leftpoint.y - rightpoint.y), leftpoint.x - rightpoint.x);
}

Edge::Edge(Vector2 start, Site* site_1, Site* site_2) : start(start) {
    Vector2 leftpoint = site_1->point;
    Vector2 rightpoint = site_2->point;

    this->m = -1.0 / ((leftpoint.y - rightpoint.y) / (leftpoint.x - rightpoint.x));
    this->c = start.y - this->m * start.x;
    this->direction = Vector2(-1.0 * (leftpoint.y - rightpoint.y), leftpoint.x - rightpoint.x);

    if (leftpoint.y > rightpoint.y) {
        site_up = site_1;
        site_down = site_2;
    }

    else {
        site_up = site_2;
        site_down = site_1;
    }
}

bool Edge::intersect(Edge edge, Vector2& intersection) {
    // Edges are parallel
    if (this->m == edge.m)
        return false;

    double x, y;

    // Vertical line
    if (this->m == std::numeric_limits<double>::infinity() || this->m == -std::numeric_limits<double>::infinity()) {
        x = start.x;
        y = edge.m * x + edge.c;
    }

    else {
        x = (edge.c - this->c) / (this->m - edge.m);
        y = this->m * x + this->c;
    }

    // Edges have a direction, intersection is on that side of the edge
    if ((x - this->start.x) / this->direction.x < 0)
        return false;

    if ((y - this->start.y) / this->direction.y < 0)
        return false;

    if ((x - edge.start.x) / edge.direction.x < 0)
        return false;

    if ((y - edge.start.y) / edge.direction.y < 0)
        return false;

    intersection = Vector2(x, y);

    return true;
}

Edge Edge::merge() {
    Edge edge = *this;

    if (edge.finished)
        edge.start = adjacent->end;

    return edge;
}

bool Edge::operator==(const Edge& edge) const {
    return start == edge.start && end == edge.end;
}

double Edge::evaluateY(double x) const {
    double w = (x - start.x) / (end.x - start.x);
    double y = (1 - w) * start.y + (w)*end.y;

    return y;
}

Box::Box(Vector2 bottom_left, Vector2 upper_right) {
    Vector2 bottom_right(upper_right.x, bottom_left.y);
    Vector2 upper_left(bottom_left.x, upper_right.y);

    bounds[0] = Edge(bottom_left, upper_left);
    bounds[1] = Edge(upper_left, upper_right);
    bounds[2] = Edge(upper_right, bottom_right);
    bounds[3] = Edge(bottom_right, bottom_left);
}

bool Box::intersect(Edge edge, Vector2& intersection) {
    for (Edge bound : bounds)
        if (bound.intersect(edge, intersection)) {
            if (!intersection.isOn(bound.start, bound.end))
                continue;

            return true;
        }

    return false;
}

bool Box::contains(Vector2 point) {
    return point.x >= bounds[0].start.x &&
        point.x <= bounds[2].start.x &&
        point.y >= bounds[0].start.y &&
        point.y <= bounds[1].start.y;
}

void VoronoiDiagram::addEdge(Edge segment) {
    edges.push_back(segment);
}

std::vector<Edge> VoronoiDiagram::getEdges() const {
    return edges;
}

VoronoiDiagram::VoronoiDiagram(std::vector<Site*> sites) : sites(sites) {

}
