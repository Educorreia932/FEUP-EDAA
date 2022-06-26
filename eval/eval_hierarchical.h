#pragma once

#include "Coord.h"
#include "HierarchicalClustering.h"

#include <list>
#include <iostream>
#include <vector>

void evalHierarchical(const MapGraph& map_graph) {
    // Open output CSV
    std::ofstream os("eval/hierarchical.csv");

    os << std::fixed;

    // Initialize list of coordinates
    std::vector<Coord> coordinate_list;

    for (std::pair<DWGraph::node_t, Coord> element : map_graph.getNodes())
        coordinate_list.push_back(element.second);

    std::vector<size_t> sizes = {
            10,
            20,
            30,
            40,
            50,
            60,
            70,
            80,
            90,
           100,
           200,
           300,
           400,
           500,
           600,
           700,
           800,
           900,
          1000,
          1250,
          1500,
          1750,
          2000,
    };

    const size_t REPEAT = 5;

    for (const size_t& size : sizes) {
        std::cout << "Size: " << size << std::endl;
        os << size;

        double total_time = 0;

        // Execute algorithm
        for (size_t i = 0; i < REPEAT; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();

            // Measure time
            auto end = std::chrono::high_resolution_clock::now();
            double time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) / 1.0e9;

            total_time += time;

            os << "," << time;
        }

        std::cout << "Time:" << total_time / REPEAT << std::endl;

        os << std::endl;
    }

    os.close();
}
