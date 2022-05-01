#include "VoronoiDiagram.h"

#include <algorithm>
#include <limits>

void VoronoiDiagram::addEdge(Edge segment) {
    edges.push_back(segment);
}

std::vector<Edge> VoronoiDiagram::getEdges() const {
    return edges;
}

VoronoiDiagram::VoronoiDiagram(std::vector<Site*> sites) : sites(sites) {

}
