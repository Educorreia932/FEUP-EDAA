#include "Edge.h"

class Box {
public:
    double width;
    double height;
    Edge bounds[4];

    Box();
    Box(Vector2 bottom_left, Vector2 upper_right);
    Box(double width, double height);

    bool intersect(Edge edge, Vector2& intersection);
    bool contains(Vector2 point);
};