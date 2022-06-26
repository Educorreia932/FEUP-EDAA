#pragma once

#include "Box.h"
#include "Coord.h"
#include "KMeans.h"

#include <list>
#include <iostream>
#include <vector>

void elbowMethod(const MapGraph& map_graph) {
    // Open output CSV
    std::ofstream os("eval/kmeans-elbow.csv");

    os << "Number of clusters,Centroid (Latitude),Centroid (Longitude)\n";

    // Get list of coordinates
    std::vector<Coord> coords;

    Box box = Box(Vector2(-8.68, 41.12), Vector2(-8.62, 41.18));

    for (std::pair<const DWGraph::node_t, Coord> node : map_graph.getNodes()) {
        Vector2 point = Vector2(node.second.lon(), node.second.lat());

        if (box.contains(point)) 
            coords.push_back(node.second);
    }

    std::vector<size_t> n_clusters = {
         5,
        10,
        20,
        30,
        40,
        50,
        60,
        70,
        80,
        90,
       100
    };

    for (auto k : n_clusters) {
        std::cout << "No. clusters: " << k << std::endl;

        auto kmeans = KMeans(coords, 100, k);

        kmeans.run();

        for (auto cluster : kmeans.clusters) {
            auto centroidCoords = cluster.getCentroid().getCoords();

            os << k << ", " <<centroidCoords.lat() << ", " << centroidCoords.lon() << "\n";
        }

        os.flush();
    }

    os.close();
}

void evalKMeans(const MapGraph& map_graph) {
    // Open output CSV
    std::ofstream os("eval/kmeans.csv");

    os << std::fixed;

    std::vector<Coord> coords;

    for (std::pair<DWGraph::node_t, Coord> element : map_graph.getNodes())
        coords.push_back(element.second);

    std::vector<size_t> sizes = {
             1,
          1000,
          2000,
          3000,
          4000,
          5000,
          6000,
          7000,
          8000,
          9000,
         10000, 12500, 15000, 17500,
         20000, 22500, 25000, 27500,
         30000, 32500, 35000, 37500,
         40000, 42500, 45000, 47500,
         50000, 52500, 55000, 57500,
         60000, 62500, 65000, 67500,
         70000, 72500, 75000, 77500,
         80000, 82500, 85000, 87500,
         90000, 92500, 95000, 97500,
        100000,102500,105000,107500,
        110000,112500,115000,117500,
        120000,122500,125000,127500,
        130000,132500,135000,137500,
        140000,142500,145000,147500,
        150000,152500,155000,157500,
        160000,162500,165000,167500,
        170000,172500,175000,177500,
        180000,182500,185000,187500,
        190000,192500,195000,197500,
        200000,202500,205000,207500,
        210000,212500,215000,217500,
        220000,222500,225000,227500,
        230000,232500,235000,237500,
        240000,242500,245000,247500,
        250000,252500,255000,257500,
        260000,262500,265000,267500,
        270000,272500,275000,277500,
        280000,282500,285000,287500,
        290000,292500,295000,297500,
        300000,
    };

    std::vector<size_t> n_clusters = {
        10,
        20,
        30,
        40
    };

    os << "num_clusters,";

    for (const size_t& size : sizes)
        os << size << ",";

    os << std::endl;

    // Iterate over number of clusters
    for (const size_t& n_cluster : n_clusters) {
        os << n_cluster;

        // Iterate over sizes
        for (const size_t& size : sizes) {
            if (size < n_cluster) {
                std::cerr << "Size too small" << std::endl;

                continue;
            }

            std::cout << "Size: " << size << " | " << "No. Clusters: " << n_cluster << std::endl;

            int N = size;
            auto first = coords.begin();
            auto last = coords.begin() + size;

            std::vector<Coord> subCoords(first, last);

            auto begin = std::chrono::high_resolution_clock::now();

            // Execute algorithm
            auto c = KMeans(subCoords, 3, n_cluster);
            c.run();

            // Measure time
            auto end = std::chrono::high_resolution_clock::now();
            double total_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) / 1.0e9;

            os << "," << total_time;
        }

        os << std::endl;
    }

    os.close();
}