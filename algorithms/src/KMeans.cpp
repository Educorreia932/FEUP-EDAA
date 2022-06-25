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
    // std::set<int> indexes = {};

    // for (int i = 0; i <= this->k; ++i) {
    //     // std::cout << "Creating cluster #" + std::to_string(i) << std::endl;
    //     // std::cout << "Size:" + std::to_string(points.size()) << std::endl;
    //     int index = std::rand() % points.size(); // get random point index (centroid candidate)
    //     if (indexes.find(index) != indexes.end()) { i--; continue; } // if point is already a centroid, skip
    //     Cluster c(i, points.at(index));         // create cluster
    //     points.at(index).setClusterId(i);       // point is associated with that cluster
    //     indexes.insert(index);                  // index saved so as not to be repeated
    //     clusters.push_back(c);                  // cluster added to clusters' vector
    //     // std::cout << "Centroid is point ID: #" + std::to_string(points.at(index).getId()) << std::endl;
    // }

    // for (int i = 0; i < k; i++) {
    //     std::cout << "Cluster Coords (" + std::to_string(clusters[i].getCentroid().getCoords().lat()) + ", "
    //     + std::to_string(clusters[i].getCentroid().getCoords().lon()) + ")" << std::endl;
    // }


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
            std::cout << "Found new cluster for Point #" << point.getId() << " : " << i << " (dist: " <<
            minDist << ")" << std::endl;
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
        std::cout << "Iter Loop #" + std::to_string(curr_iter) << std::endl;
        bool reached_stop_criteria = true;
        int curr_cluster, new_cluster;


        for (int i = 0; i < clusters.size(); i++) {
            std::cout <<  std::fixed << std::setprecision(6) << "Cluster #" << i << " (" << clusters[i].getCentroid().getCoords().lat() << ", "
                << clusters[i].getCentroid().getCoords().lon() << ")" << std::endl;
        }

        std::cout << std::endl;
        

        // set cluster for each point
        for (int i = 0; i < points.size(); i++) {
            curr_cluster = points.at(i).getClusterId();
            new_cluster = findClosestCluster(points.at(i));
            std::cout << "Point #" << i << " - curr:" << curr_cluster << " - new:" << new_cluster << std::endl;

            points.at(i).setClusterId(new_cluster);           

            if (curr_cluster != new_cluster) {
                reached_stop_criteria = false; // stop criteria based on cluster stability
            }
        }
        
        
        for (Cluster c : clusters) {
            std::cout << "Centroid #" << c.getCentroidId() << " "
            << c.getCentroid().getCoords().lat() << " " << c.getCentroid().getCoords().lon() << std::endl;
        }
        
        // clear clusters
        for (Cluster &c : clusters) {
            std::cout << "Befor: " << c.getPoints().size() << std::endl;
            c.removePoints();
            std::cout << "After: " << c.getPoints().size() << " Centroid: " << c.getCentroid().getCoords().lat() << std::endl;
        }


        // reassign points to new cluster
        for (int i = 0; i < points.size(); i++) {
            if (points.at(i).getClusterId() == -1) continue;

            if (clusters[points.at(i).getClusterId()].getCentroidId() == points[i].getId()) {
                clusters.at(points.at(i).getClusterId()).addPoint(points.at(i));
                std::cout << "Adding Point #" << points.at(i).getId() << " to Cluster #" << points.at(i).getClusterId()
                << " | size: " << clusters.at(points.at(i).getClusterId()).getSize() << " Point also centroid!" << std::endl;
            }
            else {
                clusters.at(points.at(i).getClusterId()).addPoint(points.at(i));
                std::cout << "Adding Point #" << points.at(i).getId() << " to Cluster #" << points.at(i).getClusterId()
                << " | size: " << clusters.at(points.at(i).getClusterId()).getSize() << std::endl;
                
            }
        }

        for (Cluster c : clusters) {
            std::cout << "Befor: " << c.getPoints().size() << std::endl;
            c.removePoints();
            std::cout << "After: " << c.getPoints().size() << " Centroid: " << c.getCentroid().getCoords().lat() << std::endl;
        }

        // recalculate centroids
        for (int i = 0; i < k; i++) { // for each centroid
            std::cout << "Recalculating Centroid for Cluster #" << i << " | Current coords ("
            << clusters[i].getCentroid().getCoords().lat() << ", "
            << clusters[i].getCentroid().getCoords().lon() << ")"
            << " with size " << clusters[i].getSize() << std::endl;

            double dists_lat = 0, dists_long = 0, new_lat, new_long;

            // for each point in cluster
            for (int j = 0; j < clusters.at(i).getSize(); j++) {
                std::cout << "Point coords (" << clusters.at(i).getPoint(j).getCoords().lat() << ", "
                << clusters.at(i).getPoint(j).getCoords().lon() << ")" << std::endl;
                dists_lat += clusters.at(i).getPoint(j).getCoords().lat();
                dists_long += clusters.at(i).getPoint(j).getCoords().lon();
                // std::cout << "curr dists: " << dists_lat << " " << dists_long << std::endl;
            }

            std::cout << "Average coords: " << (dists_lat  / clusters.at(i).getSize()) << " " << (dists_long / clusters.at(i).getSize()) << std::endl;;

            // get mean of distance, converting back to coordinates
            new_lat  = (dists_lat  / clusters.at(i).getSize());
            new_long = (dists_long / clusters.at(i).getSize());

            // update centroid
            Coord c(new_lat, new_long);
            Point updated_centroid(i, c);
            clusters.at(i).setCentroid(updated_centroid);

            std::cout << "Updated centroid for Cluster #" << i
            << " coords (" << clusters.at(i).getCentroid().getCoords().lat() << ", "
            << clusters.at(i).getCentroid().getCoords().lon() << ")" << std::endl;
        }

        std::cout << "Writing to File" << std::endl;
        std::ofstream pointsFile("eval/testing-iter" + std::to_string(curr_iter) + ".txt");
        // pointsFile << std::fixed << std::setprecision(3);

        for (int i = 0; i < points.size(); i++) {
            pointsFile << "Point #" + std::to_string(i) + 
                " (" + std::to_string(points[i].getCoords().lat()) + ", " 
                + std::to_string(points[i].getCoords().lon()) + ") - cluster " 
                + std::to_string(points.at(i).getClusterId()) << std::endl;
        }

        pointsFile << "\n";

        for (int i = 0; i < clusters.size(); i++) {
            pointsFile << "Cluster #" << i << " (" << clusters[i].getCentroid().getCoords().lat() << ", "
                << clusters[i].getCentroid().getCoords().lon() << ")" << std::endl;
        }


        if (reached_stop_criteria) break;


        ++curr_iter;
    }
    

    return 0;
}