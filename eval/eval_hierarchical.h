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
             1,
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

        // Calculate distance matrix
        std::vector<Coord> coordinates_sample(coordinate_list.begin(), coordinate_list.begin() + size);
        std::vector<std::vector<double>> distance_matrix;

        for (size_t i = 0; i < size; i++) {
            distance_matrix.push_back(std::vector<double>());

            for (size_t j = 0; j < size; j++) {
                if (j < i) {
                    auto c1 = coordinates_sample[i];
                    auto c2 = coordinates_sample[j];

                    distance_matrix[distance_matrix.size() - 1].push_back(Coord::getDistanceArc(c1, c2));
                }

                else
                    distance_matrix[distance_matrix.size() - 1].push_back(0);
            }
        }

        // Execute algorithm
        for (size_t i = 0; i < REPEAT; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();

            auto upgma = UPGMA(distance_matrix);
            upgma.calculate();

            // Measure time
            auto end = std::chrono::high_resolution_clock::now();
            double total_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) / 1.0e9;

            std::cout << "Time:" << total_time << std::endl;
            os << "," << total_time;
        }

        os << std::endl;
    }

    os.close();
}
