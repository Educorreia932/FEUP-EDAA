#include "HierarchicalClustering.h"

#include <catch2/catch_all.hpp>
#include <vector>
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE("Hierarchical Clustering", "[hierarchical]") {
    std::vector<Coord> points = {
        Coord(0.0, 0.0),
        Coord(3.0, 3.0),
        Coord(10.0, 10.0),
    };

    UPGMA upgma(points);

    auto result = upgma.buildTree();

    std::cout << result->distance_to_nn << std::endl;
}