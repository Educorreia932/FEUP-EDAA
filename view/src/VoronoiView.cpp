#include "VoronoiView.h"

#include <iostream>

VoronoiView::VoronoiView(sf::RenderTarget &window_, VoronoiDiagram diagram) : window(window_), diagram(diagram) {
    // Edges
    for (Edge edge : diagram.getEdges()) {
        sf::Color color(
            rand() % 128,
            rand() % 128,
            rand() % 128,
            255
        );

        lines.push_back(sf::Vertex(sf::Vector2f(edge.start.x, (-edge.start.y + 8)), color));
        lines.push_back(sf::Vertex(sf::Vector2f(edge.end.x, (-edge.end.y + 8)), color));

        std::cout << edge.start.x << " " << edge.start.y << " | " << edge.end.x << " " << edge.end.y << std::endl;
    }

    // Sites
    for (Site* site : diagram.sites) {
        sf::CircleShape* circle = new sf::CircleShape(10);

        sf::Color color(
            rand() % 128,
            rand() % 128,
            rand() % 128,
            255
        );

        circle->setFillColor(color);

        double x = site->point.x;
        double y = (-site->point.y + 8);

        circle->setPosition(x, y);
        circle->setRadius(0.1);

        circles.push_back(circle);
    }
}

VoronoiView::~VoronoiView() {

}

void VoronoiView::refresh() {
    
}

void VoronoiView::draw() {
    window.draw(&lines[0], lines.size(), sf::Lines);
    
    for (sf::CircleShape* circle : circles)
        window.draw(*circle);
}
