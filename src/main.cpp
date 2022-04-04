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

#include "DraggableZoomableWindow.h"
#include "MapView.h"
#include "MapOsmView.h"

#include "WindowController.h"

#include <X11/Xlib.h>

void view(const MapGraph &M, const std::vector<polygon_t> &polygons){
    DraggableZoomableWindow window(sf::Vector2f(0,0)); window.setBackgroundColor(sf::Color(170, 211, 223));
    MapView mapView(coord_t(41.1594,-8.6199), 20000000);
    MapOsmView mapOsmView(window, mapView, M, polygons);
    mapView.addView(&mapOsmView);
    window.setView(&mapView);

    WindowController windowController(window);
    windowController.run();
}

void voronoi(const MapGraph &M) {
    std::vector<Site> sites;

    for (int i = 0; i < 10; i++)
        sites.push_back(Site{Vector2(i, i)});

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);
}

void voronoi_display(const MapGraph &M) {
}

void view_trips(const std::vector<Trip> &trips){
    std::cout << "Building graph from trips..." << std::endl;
    const size_t N = 100000;
    std::set<size_t> s;
    while(s.size() < N) s.insert(rand()%trips.size());
    std::vector<Trip> tripsSmall;
    for(size_t i : s)
        tripsSmall.push_back(trips[i]);

    DraggableZoomableWindow window(sf::Vector2f(0,0));
    MapView mapView(coord_t(41.1594,-8.6199), 20000000);
    MapTripsView mapTripsView(window, mapView, tripsSmall);
    mapView.addView(&mapTripsView);
    window.setView(&mapView);

    WindowController windowController(window);
    windowController.run();
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];

        std::cout << "Loading map..." << std::endl;
        MapGraph M("res/map/processed/AMP");
        std::cout << "Loaded map" << std::endl;

        std::cout << "Loading polygons..." << std::endl;
        std::vector<polygon_t> polygons = polygon_t::loadPolygons("res/map/processed/AMP.polygons");
        std::cout << "Loaded polygons" << std::endl;

        if(opt == "view"){ view(M, polygons); return 0; }
        if(opt == "voronoi"){ voronoi(M); return 0; }
        if(opt == "voronoi-display"){ voronoi_display(M); return 0; }
        
        std::cout << "Loading trips..." << std::endl;
        std::vector<Trip> trips = Trip::loadTrips("res/data/pkdd15-i/pkdd15-i.trips");
        std::cout << "Loaded trips" << std::endl;

        if(opt == "view-trips"){ view_trips(trips); return 0; }
        
        std::cerr << "Invalid option" << std::endl;
        return -1;
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
        return EXIT_FAILURE;
    }
    return 0;
}
