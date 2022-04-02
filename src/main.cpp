#include "FortuneAlgorithm.h"

#include <iostream>

int main() {
    std::vector<Site> sites;
    std::vector<Vector2> points = {
        Vector2(5, 10), // A
        Vector2(9, 9.9), // B
        Vector2(5, 6),  // C
        Vector2(1, 2),  // D
        Vector2(4.9, 1.9),  // E
    };

    for (Vector2 point: points)
        sites.push_back(Site{point});

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    for (Edge edge: diagram.getEdges())
        std::cout << "Start: " << edge.start.x << " " << edge.start.y << " End: " << edge.end.x << " " << edge.end.y << std::endl;

    return 0;
}