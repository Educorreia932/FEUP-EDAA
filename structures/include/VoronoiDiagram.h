#ifndef VORONOI_DIAGRAM
#define VORONOI_DIAGRAM

#include "Vector2.h"

#include <list>

struct Face;

struct Site {
    Vector2 point;
    Face* face;
};

struct Segment {
    Site start;
    Site end;
    Segment *twin = nullptr;
    Face *incident_face;
};

struct Face {
    Site *site;
    Segment *outer_component;
};

struct Vertex {
    Vector2 point;
};


class VoronoiDiagram {
private:
    std::list<Vertex> vertices;
    std::list<Segment> half_edges;
public:
    VoronoiDiagram();

    Segment *createHalfEdge(Face *face);
    Vertex *createVertex(Vector2 point);
};

#endif
