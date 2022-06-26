#define SCALE 1000
#define RADIUS 0.1

#include "ClustersView.h"

#include <iostream>
#include <cmath>

void HSVtoRGB(float* r, float* g, float* b, float h, float s, float v) {
    int i;
    float f, p, q, t;
    
    if (s == 0) {
        // achromatic (grey)
        *r = *g = *b = v;
        return;
    }

    h /= 60;			// sector 0 to 5
    i = floor(h);
    f = h - i;			// factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
        *r = v;
        *g = t;
        *b = p;
        break;
    case 1:
        *r = q;
        *g = v;
        *b = p;
        break;
    case 2:
        *r = p;
        *g = v;
        *b = t;
        break;
    case 3:
        *r = p;
        *g = q;
        *b = v;
        break;
    case 4:
        *r = t;
        *g = p;
        *b = v;
        break;
    default:		
        *r = v;
        *g = p;
        *b = q;
        
        break;
    }
}

ClustersView::ClustersView(sf::RenderTarget& window_, KMeans& kmeans) : window(window_), kmeans(kmeans) {
    // Initialize Clusters
    kmeans.initializeClusters();   
}

void ClustersView::updateCircles() {
    int i = 0;

    for (auto cluster : kmeans.clusters) {
        
        for (auto point : cluster.pointsInCluster) {
            sf::CircleShape* circle = new sf::CircleShape();

            float r, g, b;

            // Get a linear scaled color on the rainbow pallette
            HSVtoRGB(&r, &g, &b, (i * 255.0) / kmeans.clusters.size(), 1.0, 1.0);

            // Set the color for this cluster's points
            sf::Color color(
                r * 255,
                g * 255,
                b * 255,
                255
            );

            circle->setFillColor(color);

            auto coords = point.getCoords();

            double x = (coords.lon() - (-8.68)) * SCALE;
            double y = (coords.lat() - 41.14) * SCALE;

            circle->setPosition(x - RADIUS, y - RADIUS); // Center circle
            circle->setRadius(RADIUS);

            circles.push_back(circle);
        }

        i++;
    }
}

void ClustersView::refresh() {
    if (!reached_stop_criteria) {
        reached_stop_criteria = kmeans.step();

        updateCircles();
    }
}

void ClustersView::draw() {
    for (sf::CircleShape* circle : circles)
        window.draw(*circle);
}
