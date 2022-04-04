#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapTripsView.h"

#include "Trip.h"
#include "polygon.h"

#include "QuadTreeClosestPoint.h"
#include "FortuneAlgorithm.h"

#include "WindowView.h"
#include "MapView.h"
#include "MapGraphOsmView.h"

#include "WindowController.h"

#include <X11/Xlib.h>

void view(const MapGraph& M, const std::vector<polygon_t>& polygons) {
    WindowView windowView(sf::Vector2f(0, 0)); windowView.setBackgroundColor(sf::Color(170, 211, 223));
    MapView mapView(coord_t(41.1594, -8.6199), 20000000);
    MapGraphOsmView mapGraphOsmView(windowView, mapView, M, polygons);
    mapView.addView(&mapGraphOsmView);
    windowView.setView(&mapView);

    WindowController windowController(windowView);
    windowController.run();
}

VoronoiDiagram voronoi(const MapGraph& M) {
    std::vector<Site> sites;
    std::vector<Vector2> points = {
        Vector2(6, 6), // A
        Vector2(2, 4), // B
        Vector2(4, 2), // C
        Vector2(1, 1), // D
    };

    for (Vector2 point : points)
        sites.push_back(Site{ point });

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);

    for (Edge edge : diagram.getEdges())
        std::cout << "Start: " << edge.start.x << " " << edge.start.y << " End: " << edge.end.x << " " << edge.end.y << std::endl;

    return diagram;
}

void voronoi_display(const VoronoiDiagram diagram) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi diagram", sf::Style::Default, settings);

    while (window.isOpen()) {
        sf::Event event;

        window.clear(sf::Color::White);

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        for (Edge edge : diagram.getEdges()) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(edge.start.x * 100, (-edge.start.y + 8) * 100), sf::Color(255, 0, 0, 255)),
                sf::Vertex(sf::Vector2f(edge.end.x * 100, (-edge.end.y + 8) * 100), sf::Color(255, 0, 0, 255))
            };

            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}

void view_trips(const MapGraph& M, const std::vector<Trip>& trips) {
    std::cout << "Building graph from trips..." << std::endl;
    const size_t N = 100000;
    std::set<size_t> s;
    while (s.size() < N) s.insert(rand() % trips.size());
    std::vector<Trip> tripsSmall;
    for (size_t i : s)
        tripsSmall.push_back(trips[i]);

    WindowView windowView(sf::Vector2f(0, 0));
    MapView mapView(coord_t(41.1594, -8.6199), 20000000);
    MapTripsView mapTripsView(windowView, mapView, tripsSmall);
    mapView.addView(&mapTripsView);
    windowView.setView(&mapView);

    WindowController windowController(windowView);
    windowController.run();
}

int main(int argc, char* argv[]) {
    XInitThreads();

    try {
        if (argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];

        std::cout << "Loading map..." << std::endl;
        MapGraph M("res/map/processed/AMP");
        std::cout << "Loaded map" << std::endl;

        std::cout << "Loading polygons..." << std::endl;
        std::vector<polygon_t> polygons = polygon_t::loadPolygons("res/map/processed/AMP.polygons");
        std::cout << "Loaded polygons" << std::endl;

        if (opt == "view") { view(M, polygons); return 0; }
        if (opt == "voronoi") { voronoi(M); return 0; }
        if (opt == "voronoi-display") { voronoi_display(voronoi(M)); return 0; }

        std::cout << "Loading trips..." << std::endl;
        std::vector<Trip> trips = Trip::loadTrips("res/data/pkdd15-i/pkdd15-i.trips");
        std::cout << "Loaded trips" << std::endl;

        if (opt == "view-trips") { view_trips(M, trips); return 0; }

        std::cerr << "Invalid option" << std::endl;
        return -1;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
        return EXIT_FAILURE;
    }
    return 0;
}
