#include "FortuneAlgorithm.h"

#include <catch2/catch_all.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

VoronoiDiagram voronoi(std::vector<Site> sites) {
    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    return diagram;
}

void voronoi_display(const VoronoiDiagram diagram) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi diagram", sf::Style::Default, settings);

    std::vector<sf::Vertex> lines;
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

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);
        window.draw(&lines[0], lines.size(), sf::Lines);
        window.display();
    }
}

TEST_CASE("Fortune's Algorithm", "[fortune]") {
    std::vector<Site> sites = {
        Site{Vector2(6, 6)},
        Site{Vector2(2, 4)},
        Site{Vector2(4, 2)},
        Site{Vector2(2, 1)}
    };

    VoronoiDiagram diagram = voronoi(sites);

    voronoi_display(diagram);
}
