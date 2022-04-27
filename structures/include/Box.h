#pragma once

#include "Edge.h"
#include "Vector2.h"

class Edge;

class Box {
public:
    double width;
    double height;
    std::vector<Edge> bounds;

    Box();
    Box(Vector2 bottom_left, Vector2 upper_right);
    Box(double width, double height);

    bool intersect(Edge edge, Vector2& intersection);
    bool contains(Vector2 point);
};