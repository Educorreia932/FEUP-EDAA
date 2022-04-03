#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapGraphView.h"
#include "MapTripsView.h"
#include "MapViewer.h"

#include "Trip.h"

#include "QuadTreeClosestPoint.h"
#include "FortuneAlgorithm.h"

#include "graphviewer.h"

#include "WindowView.h"
#include "MapView.h"
#include "MapGraphOsmView.h"

#include "WindowController.h"

#include <X11/Xlib.h>

void view(const MapGraph &M){
    WindowView windowView(sf::Vector2f(0,0));
    MapView mapView(coord_t(41.1594,-8.6199), 50000);
    MapGraphOsmView mapGraphOsmView(windowView, mapView, M);
    mapView.addView(&mapGraphOsmView);
    windowView.setView(&mapView);

    WindowController windowController(windowView);
    windowController.run();
}

void voronoi(const MapGraph &M) {
    std::vector<Site> sites;

    for (int i = 0; i < 10; i++)
        sites.push_back(Site{Vector2(i, i)});

    VoronoiDiagram diagram = FortuneAlgorithm().construct(sites);
}

void voronoi_display(const MapGraph &M) {
    MapGraphView view(M);
    view.drawVoronoi();
}

void view_trips(const MapGraph &M, const std::vector<Trip> &trips){
    std::cout << "Building graph from trips..." << std::endl;
    const size_t N = 50000;
    std::set<size_t> s;
    while(s.size() < N) s.insert(rand()%trips.size());
    MapGraph R;
    {
        std::map<coord_t, DWGraph::node_t> points;
        for(size_t i : s){
            for(const coord_t &c: trips[i].coords){
                if(!points.count(c)){
                    size_t u = points.size();
                    points[c] = u;
                    R.addNode(u, c);
                }
            }
        }
        std::cout << "Gathered run coords..." << std::endl;
        for(size_t i : s){
            MapGraph::way_t w;
            for(const coord_t &c: trips[i].coords){
                w.nodes.push_back(points.at(c));
            }
            R.addWay(w);
        }
        std::cout << "Built graph from trips" << std::endl;
    }

    MapTripsView view(R);
    view.drawTrips();
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];

        std::cout << "Loading map..." << std::endl;
        MapGraph M("res/map/processed/AMP");
        std::cout << "Loaded map" << std::endl;

        if(opt == "view"){ view(M); return 0; }
        if(opt == "voronoi"){ voronoi(M); return 0; }
        if(opt == "voronoi-display"){ voronoi_display(M); return 0; }
        
        std::cout << "Loading trips..." << std::endl;
        std::vector<Trip> trips = Trip::loadTrips("res/data/pkdd15-i/pkdd15-i.trips");
        std::cout << "Loaded trips" << std::endl;

        if(opt == "view-trips"){ view_trips(M, trips); return 0; }
        
        std::cerr << "Invalid option" << std::endl;
        return -1;
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
        return EXIT_FAILURE;
    }
    return 0;
}
