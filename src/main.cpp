#include <fstream>

#include "MapViewer.h"

#include "graphviewer.h"
#include "FortuneAlgorithm.h"

constexpr unsigned int WINDOW_WIDTH = 600;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr float POINT_RADIUS = 0.005f;
constexpr float OFFSET = 1.0f;

void drawPoint(sf::RenderWindow &window, Vector2 point, sf::Color color) {
    sf::CircleShape shape(POINT_RADIUS);
    shape.setPosition(sf::Vector2f(point.x - POINT_RADIUS, 1 - point.y - POINT_RADIUS));
    shape.setFillColor(color);
    window.draw(shape);
}

void drawEdge(sf::RenderWindow &window, Vector2 origin, Vector2 destination, sf::Color color) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(origin.x, 1.0f - origin.y), color),
            sf::Vertex(sf::Vector2f(destination.x, 1.0f - destination.y), color)
    };
    window.draw(line, 2, sf::Lines);
}

void drawPoints(sf::RenderWindow &window, VoronoiDiagram &diagram) {
    for (std::size_t i = 0; i < diagram.getNumberSites(); ++i)
        drawPoint(window, diagram.getSite(i)->point, sf::Color(100, 250, 50));
}

void drawDiagram(sf::RenderWindow &window, VoronoiDiagram &diagram) {
    for (std::size_t i = 0; i < diagram.getNumberSites(); ++i) {
        const Site* site = diagram.getSite(i);
        Vector2 center = site->point;
        Face* face = site->face;
        Segment* halfEdge = face->outer_component;

        if (halfEdge == nullptr)
            continue;

        while (halfEdge->previous != nullptr) {
            halfEdge = halfEdge->previous;

            if (halfEdge == face->outer_component)
                break;
        }

        Segment* start = halfEdge;
        while (halfEdge != nullptr) {
            if (halfEdge->start != nullptr && halfEdge->end != nullptr) {
                Vector2 origin = (halfEdge->start->point - center) * OFFSET + center;
                Vector2 destination = (halfEdge->end->point - center) * OFFSET + center;
                drawEdge(window, origin, destination, sf::Color::Red);
            }

            halfEdge = halfEdge->next;

            if (halfEdge == start)
                break;
        }
    }
}

int main() {
    std::vector<Vector2> points;

    for (int i = 0; i < 10; i++)
        points.push_back(Vector2(i, i));

    VoronoiDiagram diagram = FortuneAlgorithm(points).construct();

    // Display the diagram
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fortune's algorithm", sf::Style::Default,
                            settings);
    window.setView(sf::View(sf::FloatRect(-0.1f, -0.1f, 1.2f, 1.2f)));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        drawDiagram(window, diagram);
        drawPoints(window, diagram);

        window.display();
    }

    return 0;
}