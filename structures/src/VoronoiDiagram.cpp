#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(const std::vector<Vector2> points) {
    for (Vector2 point: points) {
        sites.push_back(Site{point, nullptr});
        faces.push_back(Face{&sites.back(), nullptr});

        sites.back().face = &faces.back();
    }
}

Segment* VoronoiDiagram::createHalfEdge(Face* face) {
    half_edges.emplace_back();
    half_edges.back().incident_face = face;
//    half_edges.back().it = std::prev(half_edges.end());

    if (face->outer_component == nullptr)
        face->outer_component = &half_edges.back();

    return &half_edges.back();
}

Vertex* VoronoiDiagram::createVertex(Vector2 point) {
    vertices.push_back(Vertex{point});

    return &vertices.back();
}

size_t VoronoiDiagram::getNumberSites() {
    return sites.size();
}

Site* VoronoiDiagram::getSite(size_t i) {
    return &sites[i];
}
