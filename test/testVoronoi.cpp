#include "FortuneAlgorithm.h"

#include <catch2/catch_all.hpp>

#include <stdlib.h>
#include <iostream>

VoronoiDiagram voronoi(std::vector<Site*> sites) {
    VoronoiDiagram diagram = FortuneAlgorithm(sites).construct();

    return diagram;
}

TEST_CASE("Fortune's Algorithm", "[fortune]") {
    std::vector<Site*> sites = {
        new Site{Vector2(6, 6)},
        new Site{Vector2(2, 4)},
        new Site{Vector2(4, 2)},
        new Site{Vector2(1, 1)}
    };

    VoronoiDiagram diagram = voronoi(sites);
}
