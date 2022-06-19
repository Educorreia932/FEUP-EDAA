#pragma once

#include "Coord.h"
#include "KMeans.h"

#include <list>
#include <iostream>
#include <vector>

void evalKMeans(const MapGraph& map_graph) {
    // Open output CSV
    std::ofstream os("eval/kmeans.csv");

    os << std::fixed;

    std::vector<Coord> coords;
    
    for (std::pair<DWGraph::node_t, Coord> element : map_graph.getNodes())
        coords.push_back(element.second);

    std::vector<size_t> sizes = {
            // 10,
            // 20,
            // 30,
            // 40,
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
          2000,
          3000,
          4000,
          5000,
          6000,
          7000,
          8000,
          9000,
         10000,
        100000,
        300000
    };

    std::vector<size_t> n_clusters = {
        10,
        20,
        30,
        40
    };


    for (const size_t& size : sizes) {
        for (const size_t& n_cluster : n_clusters) {
            std::cout << "Size: " << size << " # Clusters: " << n_cluster << std::endl;
            os << size << ", " << n_cluster;

            int N = size;
            auto first = coords.begin();
            auto last  = coords.begin() + N;
            std::vector<Coord> subCoords(first, last);

            auto begin = std::chrono::high_resolution_clock::now();

            auto c = KMeans(subCoords, 20, n_cluster);
            c.run();

            // Measure time
            auto end = std::chrono::high_resolution_clock::now();
            double total_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) / 1.0e9;

            os << "," << total_time;

            os << std::endl;
        }
    }

    os.close();
}