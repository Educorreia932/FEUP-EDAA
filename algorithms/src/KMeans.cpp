#include "KMeans.h"


Cluster::Cluster(int id, Coord *centroid) {
    this->id = id;
    this->centroid = centroid;
}

Cluster::Cluster() {}

void Cluster::setCentroid(Coord *point) {
    centroid = point;
}

Coord* Cluster::getCentroid() {
    return centroid;
}

void Cluster::addPoint(Coord *point) {
    points.push_back(point);
}

void Cluster::removePoints() {
    points.clear();
}



KMeans::KMeans(const std::vector<Coord*> points, int iters, int k) {
    this->points = points;
    this->iters = iters;
    this->k = k;
}

KMeans::KMeans() {}

void KMeans::initializeClusters() {
    
    for (int i = 0; i < this->k; ++i) {
        int index = std::rand() % points.size();
        Cluster c(i, points.at(index));
    }
}