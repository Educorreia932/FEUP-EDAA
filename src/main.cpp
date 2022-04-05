#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapTripsView.h"
#include "MapTripMatchView.h"

#include "Trip.h"
#include "polygon.h"

#include "K2DTreeClosestPoint.h"
#include "FortuneAlgorithm.h"

#include "DraggableZoomableWindow.h"
#include "MapView.h"
#include "MapOsmView.h"
#include "VoronoiView.h"

#include "WindowController.h"
#include "WindowTripController.h"

#include <X11/Xlib.h>

using hrc = std::chrono::high_resolution_clock;

const double NANOS_TO_SECS = 1.0/1000000000.0;

void view(const MapGraph &M, const std::vector<polygon_t> &polygons){
    DraggableZoomableWindow window(sf::Vector2f(0,0)); window.setBackgroundColor(sf::Color(170, 211, 223));
    MapView mapView(Coord(41.1594,-8.6199), 20000000);
    MapOsmView mapOsmView(window, mapView, M, polygons);
    mapView.addView(&mapOsmView);
    window.setDrawView(&mapView);

    WindowController windowController(window);
    windowController.run();
}

VoronoiDiagram voronoi(const MapGraph& M) {
    std::vector<Site*> sites;

    for (std::pair<const DWGraph::node_t, Coord> node : M.getNodes() ){
        Vector2 point = Vector2(node.second.lat(), node.second.lon());
        sites.push_back(new Site{ point });
    }

    VoronoiDiagram diagram = FortuneAlgorithm(sites).construct();

    for (Edge edge : diagram.getEdges())
        std::cout << "Start: " << edge.start.x << " " << edge.start.y << " End: " << edge.end.x << " " << edge.end.y << std::endl;

    return diagram;
}

void voronoi_display() {
    std::vector<Site*> sites = {
        new Site{Vector2(2, 6)},
        new Site{Vector2(5, 5)},
        new Site{Vector2(5.13, 3.74)},
        new Site{Vector2(1, 1)}
    };

    VoronoiDiagram diagram = FortuneAlgorithm(sites).construct();

    DraggableZoomableWindow window(sf::Vector2f(0,0)); 
    window.setBackgroundColor(sf::Color(255, 255, 255));
    VoronoiView voronoiView(window, diagram);
    window.setDrawView(&voronoiView);

    WindowController windowController(window);
    windowController.run();
}

void view_trips(const std::vector<Trip>& trips) {
    std::cout << "Building graph from trips..." << std::endl;
    const size_t N = 100000;
    std::set<size_t> s;
    while (s.size() < N) s.insert(rand() % trips.size());
    std::vector<Trip> tripsSmall;
    for (size_t i : s)
        tripsSmall.push_back(trips[i]);

    DraggableZoomableWindow window(sf::Vector2f(0,0));
    MapView mapView(Coord(41.1594,-8.6199), 20000000);
    MapTripsView mapTripsView(window, mapView, tripsSmall);
    mapView.addView(&mapTripsView);
    window.setDrawView(&mapView);

    WindowController windowController(window);
    windowController.run();
}

void match_trip(const MapGraph &M, const std::vector<polygon_t> &polygons, const std::vector<Trip> &trips){
    // double minD, maxD, meanD; size_t n;
    
    // minD = 100000;
    // maxD = -100000;
    // meanD = 0;
    // n = 0;
    // const auto &nodesM = M.getNodes();
    // for(const MapGraph::way_t &way: M.getWays()){
    //     if(way.nodes.size() < 2) continue;
    //     for(auto it1 = way.nodes.begin(), it2 = ++way.nodes.begin(); it2 != way.nodes.end(); ++it1, ++it2){
    //         double d = Coord::getDistanceArc(nodesM.at(*it1), nodesM.at(*it2));
    //         minD = std::min(minD, d);
    //         maxD = std::max(maxD, d);
    //         meanD += d;
    //         n++;
    //     }
    // }
    // meanD /= n;
    // std::cout << "[" << minD << ", " << maxD << "], " << meanD << ", nNodes: " << nodesM.size() << std::endl;

    auto begin = hrc::now();
    MapGraph G = M.splitLongEdges(5.0);
    auto end = hrc::now();
    double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*double(NANOS_TO_SECS);
    std::cout << "Took " << dt << "s to split long edges" << std::endl;

    // minD = 100000;
    // maxD = -100000;
    // meanD = 0;
    // n = 0;
    // const auto &nodesG = G.getNodes();
    // for(const MapGraph::way_t &way: G.getWays()){
    //     if(way.nodes.size() < 2) continue;
    //     for(auto it1 = way.nodes.begin(), it2 = ++way.nodes.begin(); it2 != way.nodes.end(); ++it1, ++it2){
    //         double d = Coord::getDistanceArc(nodesG.at(*it1), nodesG.at(*it2));
    //         minD = std::min(minD, d);
    //         maxD = std::max(maxD, d);
    //         meanD += d;
    //         n++;
    //     }
    // }
    // meanD /= n;
    // std::cout << "[" << minD << ", " << maxD << "], " << meanD << ", nNodes: " << nodesG.size() << std::endl;

    std::list<Vector2> points;
    for(const auto &p: G.getNodes()){
        points.push_back(p.second);
    }

    K2DTreeClosestPoint closestPoint;
    closestPoint.initialize(points);
    closestPoint.run();

    std::cout << "Generating graph..." << std::endl;
    DWGraph::DWGraph dwG = G.getFullGraph();
    std::cout << "Generated graph" << std::endl;

    DraggableZoomableWindow window(sf::Vector2f(0,0)); window.setBackgroundColor(sf::Color(170, 211, 223));
    MapView mapView(Coord(41.1594,-8.6199), 20000000);
    MapTerrainOsmView mapTerrainOsmView(window, mapView, polygons);
    MapGraphOsmView mapGraphOsmView(window, mapView, M);
    MapTripMatchView mapTripMatchView(window, mapView);
    mapView.addView(&mapTerrainOsmView);
    mapView.addView(&mapGraphOsmView);
    mapView.addView(&mapTripMatchView);
    window.setDrawView(&mapView);

    WindowTripController windowTripController(window, mapTripMatchView, G, dwG, trips, closestPoint);
    windowTripController.run();
}

int main(int argc, char* argv[]) {
    XInitThreads();

    try {
        if (argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];
        if (opt == "voronoi-display") { voronoi_display(); return 0; }

        std::chrono::_V2::system_clock::time_point begin, end;
        double dt;

        std::cout << "Loading map..." << std::endl;
        begin = hrc::now();
        MapGraph M("res/map/processed/AMP");
        end = hrc::now();
        dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
        std::cout << "Loaded map, took " << dt << "s" << std::endl;

        std::cout << "Loading polygons..." << std::endl;
        begin = hrc::now();
        std::vector<polygon_t> polygons = polygon_t::loadPolygons("res/map/processed/AMP.polygons");
        end = hrc::now();
        dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
        std::cout << "Loaded polygons, took " << dt << "s" << std::endl;

        if (opt == "view") { view(M, polygons); return 0; }
        if (opt == "voronoi") { voronoi(M); return 0; }

        std::cout << "Loading trips..." << std::endl;
        begin = hrc::now();
        std::vector<Trip> trips = Trip::loadTripsBin("res/data/pkdd15-i/pkdd15-i.trips.bin");
        end = hrc::now();
        dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
        std::cout << "Loaded trips, took " << dt << "s" << std::endl;

        if (opt == "view-trips") { view_trips(trips); return 0; }
        if (opt == "match-trip") { match_trip(M, polygons, trips); return 0; }

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
