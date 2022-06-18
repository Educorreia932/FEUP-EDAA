#pragma once

#include <iostream>

void evalHierarchical(const MapGraph& map_graph) {
    auto distance_graph = map_graph.getDistanceGraph();
    std::list<Coord> coordinate_list;

    for (auto node : distance_graph.getNodes()) 
        coordinate_list.push_back(map_graph.nodeToCoord(node));

    for (auto coordinates : coordinate_list) 
        std::cout << coordinates.lat() << std::endl;
}