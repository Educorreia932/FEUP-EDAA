#include "FortuneAlgorithm.h"

#include <iostream>

int main() {
    std::vector<Site> sites;
    std::vector<Vector2> points = {
        Vector2(6, 6), // A
        Vector2(2, 4), // B
        Vector2(4, 2), // C
        Vector2(1, 1), // D
    };

    for (Vector2 point : points)
        sites.push_back(Site{ point });

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    for (Edge edge : diagram.getEdges())
        std::cout << "Start: " << edge.start.x << " " << edge.start.y << " End: " << edge.end.x << " " << edge.end.y << std::endl;

    return 0;
}