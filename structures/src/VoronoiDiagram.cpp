#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram() {

}

Segment *VoronoiDiagram::createHalfEdge(Face *face) {
    half_edges.emplace_back();
    half_edges.back().incident_face = face;
//    half_edges.back().it = std::prev(half_edges.end());

    if (face->outer_component == nullptr)
        face->outer_component = &half_edges.back();

    return &half_edges.back();
}

Vertex *VoronoiDiagram::createVertex(Vector2 point) {
    vertices.push_back(Vertex{point});

    return &vertices.back();
}
