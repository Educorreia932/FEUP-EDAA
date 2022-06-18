#include "KMeans.h"

#include <set>

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
    std::vector<Point> all_points;
    for (int i = 0; i < points.size(); i++) {
        Point p(i, points.at(i));
    }
    this->points = all_points;
    this->iters = iters;
    this->k = k;
}

KMeans::KMeans() {}

void KMeans::initializeClusters() {
    std::set<int> indexes = {};

    for (int i = 0; i < this->k; ++i) {
        int index = std::rand() % points.size(); // get random point index (centroid candidate)
        if (indexes.find(index) != indexes.end()) { continue; } // if point is already a centroid, skip

        Cluster c(i, points.at(index));         // create cluster
        points.at(index).setClusterId(index);   // point is associated with that cluster
        indexes.insert(index);                  // index saved so as not to be repeated
        clusters.push_back(c);                  // cluster added to clusters' vector
    }
}

int KMeans::findClosestCluster(Point point) {

    int closest;
    double dist;

    // get dist to current cluster centroid
    double minDist = point.getCoords().getDistance(clusters.at(point.getClusterId()).getCentroid().getCoords());

    // iterate over other cluster centroids to find new allocation
    for (int i = 0; i < k; i++) {
        dist = point.getCoords().getDistance(clusters.at(i).getCentroid().getCoords());

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
    int curr_iter = 1;

    while (curr_iter <= iters) {
        bool reached_stop_criteria = true;
        int curr_cluster, new_cluster;

        // set cluster for each point
        for (int i = 0; i < points.size(); i++) {
            curr_cluster = points.at(i).getClusterId();
            new_cluster = findClosestCluster(points.at(i));
        }
        if (curr_cluster != new_cluster) reached_stop_criteria = false; // stop criteria based on cluster stability
        
        // clear clusters
        for (Cluster c : clusters) c.removePoints();

        // reassign points to new cluster
        for (int i = 0; i < points.size(); i++) clusters.at(points.at(i).getClusterId()).addPoint(points.at(i));

        // recalculate centroids
        for (int i = 0; i < k; i++) { // for each centroid

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

        if (reached_stop_criteria) break;

        ++curr_iter;
    }

    return 0;
}