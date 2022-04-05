#include "FortuneAlgorithm.h"

#include <catch2/catch_all.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>

VoronoiDiagram voronoi(std::vector<Site*> sites) {
    VoronoiDiagram diagram = FortuneAlgorithm(sites).construct();

    return diagram;
}

void voronoi_display(const VoronoiDiagram diagram) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi diagram", sf::Style::Default, settings);

    std::vector<sf::Vertex> lines;
    std::vector<sf::CircleShape*> circles;
    
    // Draw edges
    for (Edge edge : diagram.getEdges()) {
        sf::Color color(
            rand() % 255 + 1,
            rand() % 255 + 1,
            rand() % 255 + 1,
            255
        );

        lines.push_back(sf::Vertex(sf::Vector2f(edge.start.x * 100, (-edge.start.y + 8) * 100), color));
        lines.push_back(sf::Vertex(sf::Vector2f(edge.end.x * 100, (-edge.end.y + 8) * 100), color));
    }

    // Draw sites
    for (Site* site : diagram.sites) {
        sf::CircleShape* circle = new sf::CircleShape(10);

        sf::Color color(
            rand() % 255 + 1,
            rand() % 255 + 1,
            rand() % 255 + 1,
            255
        );

        circle->setFillColor(color);

        double x = site->point.x * 100;
        double y = (-site->point.y + 8) * 100;

        circle->setPosition(x, y);
        circle->setRadius(5);

        circles.push_back(circle);
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);
        window.draw(&lines[0], lines.size(), sf::Lines);
    
        for (sf::CircleShape* circle : circles)
            window.draw(*circle);

        window.display();
    }
}

TEST_CASE("Fortune's Algorithm", "[fortune]") {
    std::vector<Site*> sites = {
        new Site{Vector2(6, 6)},
        new Site{Vector2(2, 4)},
        new Site{Vector2(4, 2)},
        new Site{Vector2(2, 1)}
    };

    VoronoiDiagram diagram = voronoi(sites);

    voronoi_display(diagram);
}
