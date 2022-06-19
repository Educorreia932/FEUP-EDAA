#include "HierarchicalClustering.h"

#include <catch2/catch_all.hpp>
#include <vector>
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE("Hierarchical Clustering", "[hierarchical]") {
    std::vector<std::vector<double>> distance_matrix = {
        {0, 3, 4, 4, 2},
        {3, 0, 6, 6, 5},
        {4, 6, 0, 7, 7},
        {4, 6, 7, 0, 9},
        {2, 5, 7, 9, 0}
    };

    UPGMA clustering(distance_matrix);

    auto result = clustering.calculate();

    REQUIRE(result.distance == 3.25);
}