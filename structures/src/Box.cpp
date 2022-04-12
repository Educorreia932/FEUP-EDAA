#include "Box.h"

Box::Box() {

}

Box::Box(Vector2 bottom_left, Vector2 upper_right) {
    Vector2 bottom_right(upper_right.x, bottom_left.y);
    Vector2 upper_left(bottom_left.x, upper_right.y);

    width = upper_right.x - bottom_left.x;
    height = upper_right.y - bottom_left.y;

    bounds.push_back(Edge(bottom_left, upper_left));
    bounds.push_back(Edge(upper_left, upper_right));
    bounds.push_back(Edge(upper_right, bottom_right));
    bounds.push_back(Edge(bottom_right, bottom_left));
}

Box::Box(double width, double height) : Box(Vector2(0, 0), Vector2(width, height)) {

}

bool Box::intersect(Edge edge, Vector2& intersection) {
    for (Edge bound : bounds)
        if (bound.intersect(edge, intersection)) {
            if (
                !intersection.isOn(bound) || 
                intersection == edge.start || 
                intersection == edge.end || 
                (edge.finished && !intersection.isOn(edge))
            )
                continue;

            return true;
        }

    return false;
}

bool Box::contains(Vector2 point) {
    return point.x >= bounds[0].start.x &&
        point.x <= bounds[2].start.x &&
        point.y >= bounds[0].start.y &&
        point.y <= bounds[0].end.y;
}
