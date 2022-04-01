#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapGraphView.h"
#include "MapViewer.h"

#include "Run.h"

#include "QuadTreeClosestPoint.h"

#include "graphviewer.h"

#include <X11/Xlib.h>

void view(const MapGraph &M){
    MapGraphView view(M);
    view.drawRoads();
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
        
        std::cout << "Loading runs..." << std::endl;
        std::vector<Run> runs = Run::loadRuns("res/data/pkdd15-i/pkdd15-i.runs");
        std::cout << "Loaded runs" << std::endl;
        
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
        return EXIT_FAILURE;
    }
    return 0;
}
