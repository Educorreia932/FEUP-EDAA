#include "KMeans.h"

#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>

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

void Cluster::addPoint(Point point) {
    pointsInCluster.push_back(point);
    point.setClusterId(this->id);
}

void Cluster::removePoints() {
    pointsInCluster.clear();
}

// ----------------------------

KMeans::KMeans(const std::vector<Coord> points, int iters, int k) {
    // std::cout << points.size() << std::endl;
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
    std::set<int> indexes = {};

    for (int i = 0; i <= this->k; ++i) {
        // std::cout << "Creating cluster #" + std::to_string(i) << std::endl;
        // std::cout << "Size:" + std::to_string(points.size()) << std::endl;
        int index = std::rand() % points.size(); // get random point index (centroid candidate)
        if (indexes.find(index) != indexes.end()) { i--; continue; } // if point is already a centroid, skip
        Cluster c(i, points.at(index));         // create cluster
        points.at(index).setClusterId(i);   // point is associated with that cluster
        indexes.insert(index);                  // index saved so as not to be repeated
        clusters.push_back(c);                  // cluster added to clusters' vector
        // std::cout << "Centroid is point ID: #" + std::to_string(points.at(index).getId()) << std::endl;
    }

    // for (int i = 0; i < k; i++) {
    //     std::cout << "Cluster Coords (" + std::to_string(clusters[i].getCentroid().getCoords().lat()) + ", "
    //     + std::to_string(clusters[i].getCentroid().getCoords().lon()) + ")" << std::endl;
    // }
}


int KMeans::findClosestCluster(Point point) {

    int closest = point.getClusterId();
    double dist, minDist;

    // get dist to current cluster centroid
    // std::cout << "ID: #" + std::to_string(point.getId()) + " " + std::to_string(point.getClusterId()) << std::endl;
    if (point.getClusterId() == -1) { // cluster not yet allocated
        minDist = std::numeric_limits<double>::max();
    }
    else {
        // std::cout << "Here is the problem?" << std::endl;
        // std::cout << "Cluster ID: " << std::to_string(point.getClusterId()) << std::endl;
        auto a = clusters.at(point.getClusterId()).getCentroid().getCoords();
        minDist = Coord::getDistanceArc(point.getCoords(), a);
        // std::cout << "Min Distt: " << std::to_string(minDist) << std::endl;
    }
    
    // std::cout << "Got minDist " + std::to_string(minDist) << std::endl;

    // iterate over other cluster centroids to find new allocation
    for (int i = 0; i < k; i++) {
        dist = Coord::getDistanceArc(point.getCoords(), clusters.at(i).getCentroid().getCoords());

        // if found a closest centroid
        if (dist < minDist) {
            minDist = dist;
            // point.setClusterId(i);
            // std::cout << "i:" + std::to_string(i) << std::endl;
            closest = i;
        }
    }
    // std::cout << "Closest:" + std::to_string(closest) << std::endl;
    return closest;
}


int KMeans::run() {

    // Initialize Clusters
    initializeClusters();

    // std::cout << "Init happened" << std::endl;

    // std::cout << clusters.size() << std::endl;

    // Iterations Loop
    int curr_iter = 1;

    while (curr_iter <= iters) {
        // std::cout << "Iter Loop #" + std::to_string(curr_iter) << std::endl;
        bool reached_stop_criteria = true;
        int curr_cluster, new_cluster;

        // set cluster for each point
        for (int i = 0; i < points.size(); i++) {
            curr_cluster = points.at(i).getClusterId();
            new_cluster = findClosestCluster(points.at(i));
            // std::cout << "Point #" + std::to_string(i) +
            //     " - curr:" + std::to_string(curr_cluster) +
            //     " - new:" + std::to_string(new_cluster)
            // << std::endl;

            points.at(i).setClusterId(new_cluster);

            if (curr_cluster != new_cluster) {
                reached_stop_criteria = false; // stop criteria based on cluster stability
                // std::cout << "\n--- HERE ---\n" << std::endl;
            }
            // std::cout << "After update stop criteria bruh" << std::endl;
        }
        // std::cout << "After find closest" << std::endl;
        
        
        // clear clusters
        for (Cluster c : clusters) c.removePoints();
        // std::cout << "After clear clusters" << std::endl;

        // reassign points to new cluster
        for (int i = 0; i < points.size(); i++) {
            if (points.at(i).getClusterId() == -1) continue;
            clusters.at(points.at(i).getClusterId()).addPoint(points.at(i));
        }
            
        // std::cout << "After reassign to clusters" << std::endl;

        // recalculate centroids
        for (int i = 0; i < k; i++) { // for each centroid
            // std::cout << "Inner Loop #" + std::to_string(i) << std::endl;

            double dists_lat = 0, dists_long = 0, new_lat, new_long;

            // for each point in cluster
            for (int j = 0; j < clusters.at(i).getSize(); j++) {
                dists_lat += clusters.at(i).getPoint(j).getCoords().LatDegreesToMeters();
                dists_long += clusters.at(i).getPoint(j).getCoords().LonDegreesToMeters();
            }

            // get mean of distance, converting back to coordinates
            new_lat  = Coord::MetersToLatDegrees() * (dists_lat  / clusters.at(i).getSize());
            new_long = Coord::MetersToLonDegrees() * (dists_long / clusters.at(i).getSize());

            // update centroid
            Coord c(new_lat, new_long);
            Point updated_centroid(i, c);
            clusters.at(i).setCentroid(updated_centroid);
        }

        // std::cout << "Writing to File" << std::endl;
        // std::ofstream pointsFile("eval/testing-iter" + std::to_string(curr_iter) + ".txt");
        // pointsFile << std::fixed << std::setprecision(3);

        // for (int i = 0; i < points.size(); i++) {
        //     pointsFile << "Point #" + std::to_string(i) + 
        //         " (" + std::to_string(points[i].getCoords().lat()) + ", " 
        //         + std::to_string(points[i].getCoords().lon()) + ") - cluster " 
        //         + std::to_string(points.at(i).getClusterId()) << std::endl;
        // }

        // pointsFile << "\n";

        // for (int i = 0; i < clusters.size(); i++) {
        //     pointsFile << "Cluster #" + std::to_string(i) +
        //         " (" + std::to_string(clusters[i].getCentroid().getCoords().lat()) + ", "
        //         + std::to_string(clusters[i].getCentroid().getCoords().lon()) + ")"
        //         << std::endl;
        // }


        if (reached_stop_criteria) break;


        ++curr_iter;
        // std::cout << "Updated iters to: " + std::to_string(curr_iter) << std::endl;
    }
    

    return 0;
}