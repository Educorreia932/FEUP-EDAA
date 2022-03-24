#ifndef VORONOI_DIAGRAM
#define VORONOI_DIAGRAM

#include "Vector2.h"

#include <list>
#include <vector>

class FortuneAlgorithm;

struct Face;

struct Site {
    Vector2 point;
    Face* face;
};

struct Segment {
    Site* start;
    Site* end;
    Segment* twin = nullptr;
    Face* incident_face;
    Segment* previous;
    Segment* next;
};

struct Face {
    Site* site;
    Segment* outer_component;
};

struct Vertex {
    Vector2 point;
};


class VoronoiDiagram {
private:
    friend FortuneAlgorithm;

    std::vector<Site> sites;
    std::vector<Face> faces;
    std::list<Vertex> vertices;
    std::list<Segment> half_edges;
public:
    VoronoiDiagram(const std::vector<Vector2> points);

    Segment* createHalfEdge(Face* face);
    Vertex* createVertex(Vector2 point);
    size_t getNumberSites();
    Site* getSite(size_t i);
};

#endif
