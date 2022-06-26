#pragma once

#include "Coord.h"

#include <vector>

class Point {
public:
    Point();
    Point(int id, Coord coords);

    int getId();
    int getClusterId();
    Coord getCoords();

    void setClusterId(int id);

private:
    // cluster_id == -1 represent non-assignment
    int id, cluster_id;
    Coord coords;
};

class Cluster {
private:
    int id;
    Point centroid;
public:
    std::vector<Point> pointsInCluster;

    Cluster();
    Cluster(int id, Point centroid);

    void setCentroid(Point point);

    Point getCentroid();
    int getCentroidId();
    int getSize();
    Point getPoint(int index);
    std::vector<Point> getPoints();

    void addPoint(Point point);
    void removePoints();
};

class KMeans {
private:
    int iters;
    int k;

    /**
     * @brief Initializes clusters, assigning an initial random centroid to each
     */
    void initializeClusters();
public:
    std::vector<Point> points;
    std::vector<Cluster> clusters;

    /**
     * @brief Construct from list of points, and number of iterations and clusters
     *
     * @param points    Data Points to which apply clustering
     * @param iters     Number of iterations over which we will run k-means
     * @param k         Number of clusters to split data points into
     */
    KMeans(const std::vector<Coord> points, int iters, int k);

    /**
     * @brief Construct without arguments
     */
    KMeans();

    int findClosestCluster(Point point);

    int run();
};