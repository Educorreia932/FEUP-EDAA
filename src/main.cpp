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

void view(int argc, const char *argv[], const MapGraph &M){
    if(argc != 4) throw std::invalid_argument("invalid number of arguments");
    int fraction = atoi(argv[2]);
    int display  = atoi(argv[3]);

    MapGraphView view(M);
    view.drawRoads(fraction, display);
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        MapGraph M("res/map/processed/AMP");
        std::vector<Run> runs = Run::loadRuns("res/data/pkdd15-i/pkdd15-i.runs");
        std::string opt = argv[1];
        if(opt == "view"      ) view      (argc, const_cast<const char **>(argv), M);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
                    //  "       ./main path FRACTION FLAGS [-v]\n";
        return EXIT_FAILURE;
    }
    return 0;
}
