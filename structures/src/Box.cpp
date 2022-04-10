#include "Box.h"

Box::Box(Vector2 bottom_left, Vector2 upper_right) {
    Vector2 bottom_right(upper_right.x, bottom_left.y);
    Vector2 upper_left(bottom_left.x, upper_right.y);

    bounds[0] = Edge(bottom_left, upper_left);
    bounds[1] = Edge(upper_left, upper_right);
    bounds[2] = Edge(upper_right, bottom_right);
    bounds[3] = Edge(bottom_right, bottom_left);
}

Box::Box(double width, double height) : Box(Vector2(0, 0), Vector2(width, height)) {
    
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
