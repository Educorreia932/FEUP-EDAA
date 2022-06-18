#pragma once

#include "Coord.h"
#include "HierarchicalClustering.h"

#include <list>
#include <iostream>
#include <vector>

void evalHierarchical(const MapGraph& map_graph) {
    auto distance_graph = map_graph.getDistanceGraph();
    std::list<Coord> coordinate_list;

    for (auto node : distance_graph.getNodes())
        coordinate_list.push_back(map_graph.nodeToCoord(node));

    std::vector<std::vector<double> > distance_matrix;

    std::list<Coord> sample;

    int N = 100;

    // Get first 100 coordinates
    for (auto i = coordinate_list.begin(); i != coordinate_list.end() && N > 0; ++i, --N)
        sample.push_back(*i);

    for (auto c1 : sample) {
        distance_matrix.push_back(std::vector<double>());

        for (auto c2 : sample) 
            distance_matrix[distance_matrix.size() - 1].push_back(Coord::getDistanceArc(c1, c2));
    }

    auto upgma = UPGMA(distance_matrix);
    auto tree = upgma.calculate();
}
