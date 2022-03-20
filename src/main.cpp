#include <fstream>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapViewer.h"

#include "graphviewer.h"

#include <X11/Xlib.h>

void view(int argc, const char *argv[], const MapGraph &M){
    if(argc != 4) throw std::invalid_argument("invalid number of arguments");
    int fraction = atoi(argv[2]);
    int display  = atoi(argv[3]);

    M.drawRoads(fraction, display);
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        MapGraph M("map/processed/AMP");
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
