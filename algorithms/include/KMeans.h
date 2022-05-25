#pragma once

#include "Coord.h"

#include <vector>

class Cluster {
public:
    Cluster(int id, Coord *centroid);
    Cluster();

    void setCentroid(Coord *point);
    Coord* getCentroid();
    void addPoint(Coord *point);
    void removePoints();

private:
    int id;
    Coord *centroid;
    std::vector<Coord*> points;
};

class KMeans {
public:
    /**
     * @brief Construct from list of points, and number of iterations and clusters
     * 
     * @param points    Data Points to which apply clustering
     * @param iters     Number of iterations over which we will run k-means
     * @param k         Number of clusters to split data points into
     */
    KMeans(const std::vector<Coord*> points, int iters, int k);

    /**
     * @brief Construct without arguments
     * 
     */
    KMeans();
    
    /**
     * @brief Initializes clusters, assigning an initial random centroid to each
     * 
     */
    void initializeClusters();

private:
    std::vector<Cluster> clusters;
    std::vector<Coord*> points;
    int iters;
    int k;
};