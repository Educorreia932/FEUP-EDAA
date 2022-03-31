#ifndef VORONOI
#define VORONOI

#include "Vector2.h"

#include <queue>
#include <vector>

struct Site {
    Vector2 point;
};

struct Segment {
    Vector2 start;
    Vector2 end;
};

class VoronoiDiagram {
private:
    std::vector<Segment> edges;
public:
    void addEdge(Segment segment);
};

#endif