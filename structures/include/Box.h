#include "Edge.h"

class Box {
public:
    Edge bounds[4];

    Box(Vector2 bottom_left, Vector2 upper_right);

    bool intersect(Edge edge, Vector2& intersection);
    bool contains(Vector2 point);
};