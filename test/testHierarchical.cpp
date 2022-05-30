#include "HierarchicalClustering.h"

#include <catch2/catch_all.hpp>
#include <vector>

TEST_CASE("Hierarchical Clustering", "[hierarchical]") {
    std::vector<std::vector<double>> distance_matrix = {
        {0, 0, 0, 0, 0},
        {3, 0, 0, 0, 0},
        {4, 6, 0, 0, 0},
        {4, 6, 7, 0, 0},
        {2, 5, 7, 9, 0}
    };

   UPGMA clustering(distance_matrix);

   clustering.calculate();
}