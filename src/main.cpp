#include "MapViewer.h"

#include "FortuneAlgorithm.h"

int main() {
    std::vector<Site> sites;

    for (int i = 0; i < 10; i++)
        sites.push_back(Site{Vector2(i, i)});

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    return 0;
}