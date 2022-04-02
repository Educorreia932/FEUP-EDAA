#include "FortuneAlgorithm.h"

#include <iostream>

int main() {
    std::vector<Site> sites;

    for (int i = 0; i < 10; i++)
        sites.push_back(Site{Vector2(i, i)});

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    for (Edge edge: diagram.getEdges())
        std::cout << "Start: " << edge.start.x << " " << edge.start.y << " End: " << edge.end.x << " " << edge.end.y << std::endl;

    return 0;
}