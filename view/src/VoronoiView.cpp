#include "VoronoiView.h"

#include <iostream>

#define RADIUS 5

VoronoiView::VoronoiView(sf::RenderTarget& window_, VoronoiDiagram diagram) : window(window_), diagram(diagram) {
    // Edges
    for (Edge edge : diagram.getEdges()) {
        sf::Color color(
            rand() % 200,
            rand() % 200,
            rand() % 200,
            255
        );

        lines.push_back(sf::Vertex(sf::Vector2f(convertX(edge.start.x), convertY(edge.start.y)), color));
        lines.push_back(sf::Vertex(sf::Vector2f(convertX(edge.end.x), convertY(edge.end.y)), color));
    }

    // Sites
    for (Site* site : diagram.sites) {
        sf::CircleShape* circle = new sf::CircleShape();

        sf::Color color(
            rand() % 128,
            rand() % 128,
            rand() % 128,
            255
        );

        circle->setFillColor(color);

        double x = convertX(site->point.x);
        double y = convertY(site->point.y);

        circle->setPosition(x - RADIUS, y - RADIUS); // Center circle
        circle->setRadius(RADIUS);

        circles.push_back(circle);
    }
}

VoronoiView::~VoronoiView() {

}

double VoronoiView::convertX(double x) {
    return (x - diagram.bounding_box.width) * 100;
}

double VoronoiView::convertY(double y) {
    return (-y + diagram.bounding_box.height) * 100;
}

void VoronoiView::refresh() {

}

void VoronoiView::draw() {
    window.draw(&lines[0], lines.size(), sf::Lines);

    for (sf::CircleShape* circle : circles)
        window.draw(*circle);
}
