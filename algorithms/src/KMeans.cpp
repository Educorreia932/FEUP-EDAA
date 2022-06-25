#include "KMeans.h"

#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>


Point::Point() {}

Point::Point(int id, Coord coords) {
    this->id = id;
    this->coords = coords;
    this->cluster_id = -1; 
}

int Point::getId() { return id; }

int Point::getClusterId() { return cluster_id; }

Coord Point::getCoords() { return coords; }

void Point::setClusterId(int id ) { this->cluster_id = id; }

// ----------------------------

Cluster::Cluster() {}

Cluster::Cluster(int id, Point centroid) {
    this->id = id;
    this->centroid = centroid;
    this->addPoint(centroid);
}

void Cluster::setCentroid(Point point) { centroid = point; }

Point Cluster::getCentroid() { return centroid; }

int Cluster::getCentroidId() { return id; }

int Cluster::getSize() { return pointsInCluster.size(); }

Point Cluster::getPoint(int index) { return pointsInCluster.at(index); }

std::vector<Point> Cluster::getPoints() { return pointsInCluster; }

void Cluster::addPoint(Point point) {
    pointsInCluster.push_back(point);
    point.setClusterId(this->id);
}

void Cluster::removePoints() {
    pointsInCluster.clear();
}

// ----------------------------

KMeans::KMeans(const std::vector<Coord> points, int iters, int k) {
    std::vector<Point> all_points;
    for (int i = 0; i < points.size(); i++) {
        Point p(i, points.at(i));
        all_points.push_back(p);
    }
    this->points = all_points;
    this->iters = iters;
    this->k = k;
}

KMeans::KMeans() {}

void KMeans::initializeClusters() {
    std::vector<std::tuple<int, Point>> points_available;
    for (int i = 0; i < points.size(); i++) {
        points_available.push_back( std::tuple<int, Point> (i, points[i])); // save original position and point itself in tuple
    }
    
    for (int i = 0; i < this->k; ++i) {
        int index = std::rand() % points_available.size(); // get random point index (centroid candidate)

        Cluster c(i, std::get<1>(points_available.at(index)));

        clusters.push_back(c);

        points.at(std::get<0>(points_available.at(index))).setClusterId(i);
        std::vector<std::tuple<int,Point>>::iterator it = points_available.begin() + index;
        points_available.erase(it);
    }
}


int KMeans::findClosestCluster(Point point) {

    int closest = point.getClusterId();
    double dist, minDist;

    // get dist to current cluster centroid
    // std::cout << "ID: #" + std::to_string(point.getId()) + " " + std::to_string(point.getClusterId()) << std::endl;
    if (point.getClusterId() == -1) { // cluster not yet allocated
        minDist = std::numeric_limits<double>::max();
    }
    else { // gets as default dist the dist to the current cluster centroid
        auto a = clusters.at(point.getClusterId()).getCentroid().getCoords();
        minDist = Coord::getDistanceArc(point.getCoords(), a);
        // std::cout << "Min Dist: " << std::to_string(minDist) << std::endl;
    }

    // iterate over other cluster centroids to find new allocation
    for (int i = 0; i < k; i++) {
        dist = Coord::getDistanceArc(point.getCoords(), clusters.at(i).getCentroid().getCoords());

        // if found a closest centroid
        if (dist < minDist) {
            minDist = dist;
            point.setClusterId(i);

            closest = i;
        }
    }
    return closest;
}


int KMeans::run() {

    // Initialize Clusters
    initializeClusters();

    // Iterations Loop
    int curr_iter = 0;

    while (curr_iter < iters) {
        bool reached_stop_criteria = true;
        int curr_cluster, new_cluster;

        // set cluster for each point
        for (int i = 0; i < points.size(); i++) {
            curr_cluster = points.at(i).getClusterId();
            new_cluster = findClosestCluster(points.at(i));

            points.at(i).setClusterId(new_cluster);           

            if (curr_cluster != new_cluster) {
                reached_stop_criteria = false; // stop criteria based on cluster stability
            }
        }
        
        // Clear clusters
        for (Cluster &c : clusters) 
            c.removePoints();

        // reassign points to new cluster
        for (int i = 0; i < points.size(); i++) {
            if (points.at(i).getClusterId() == -1) 
                continue;

            clusters.at(points.at(i).getClusterId()).addPoint(points.at(i));
        }

        for (Cluster c : clusters) {
            c.removePoints();
        }

        // Recalculate centroids
        for (int i = 0; i < k; i++) { // for each centroid
            double dists_lat = 0, dists_long = 0, new_lat, new_long;

            // for each point in cluster
            for (int j = 0; j < clusters.at(i).getSize(); j++) {
                dists_lat += clusters.at(i).getPoint(j).getCoords().lat();
                dists_long += clusters.at(i).getPoint(j).getCoords().lon();
                // std::cout << "curr dists: " << dists_lat << " " << dists_long << std::endl;
            }

            // get mean of distance, converting back to coordinates
            new_lat  = (dists_lat  / clusters.at(i).getSize());
            new_long = (dists_long / clusters.at(i).getSize());

            // update centroid
            Coord c(new_lat, new_long);
            Point updated_centroid(i, c);
            clusters.at(i).setCentroid(updated_centroid);
        }

        if (reached_stop_criteria) break;

        ++curr_iter;
    }
    

    return 0;
}