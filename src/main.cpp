#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapViewer.h"

#include "QuadTreeClosestPoint.h"

#include "graphviewer.h"

#include <X11/Xlib.h>

void view(int argc, const char *argv[], const MapGraph &M){
    if(argc != 4) throw std::invalid_argument("invalid number of arguments");
    int fraction = atoi(argv[2]);
    int display  = atoi(argv[3]);

    M.drawRoads(fraction, display);
}

void evalQuadTree(const MapGraph &M);

void eval(int argc, const char *argv[], const MapGraph &M){
    srand(1);
    evalQuadTree(M);
}

void evalQuadTree(const MapGraph &M){
    std::ofstream os("eval/quadtree.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, coord_t> nodes = M.getNodes();
    std::vector<coord_t> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, coord_t> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(0));
    
    coord_t::deg_t minLat = +180.0L, maxLat = -180.0L, minLon = +180.0L, maxLon = -180.0L;
    for(const coord_t &c: coords){
        minLat = std::min(minLat, c.getLat());
        maxLat = std::max(maxLat, c.getLat());
        minLon = std::min(minLon, c.getLon());
        maxLon = std::max(maxLon, c.getLon());
    }

    const size_t N = 100;
    const size_t REPEAT = 1000;

    std::list<coord_t> test_coords;
    for(size_t n = 0; n < N; ++n){
        test_coords.push_back(coord_t(
            minLat + (double(rand())/double(RAND_MAX))*(maxLat-minLat),
            minLon + (double(rand())/double(RAND_MAX))*(maxLon-minLon)
        ));
    }

    std::vector<size_t> szs = {
        // 1<<1,
        // 1<<2,
        // 1<<3,
        // 1<<4,
        // 1<<5,
        // 1<<6,
        // 1<<7,
        // 1<<8,
        // 1<<9,
        // 1<<10,
        // 1<<11,
        // 1<<12,
        // 1<<13,
        // 1<<14,
        // 1<<15,
        // 1<<16,
        // 1<<17,
        1<<18
    };

    for(const size_t &sz: szs){
        os << sz;

        std::list<coord_t> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        QuadTreeClosestPoint t;
        t.initialize(l);
        t.run();auto begin1 = std::chrono::high_resolution_clock::now();
        for(const coord_t &u: test_coords){
            auto begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                coord_t v = t.getClosestPoint(u);
            }
            auto end = std::chrono::high_resolution_clock::now();
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;
        }auto end1 = std::chrono::high_resolution_clock::now(); std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end1-begin1).count()) << std::endl;
        os << "\n";
    }
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        MapGraph M("map/processed/AMP");
        std::string opt = argv[1];
        if(opt == "view"      ) view      (argc, const_cast<const char **>(argv), M);
        if(opt == "eval"      ) eval      (argc, const_cast<const char **>(argv), M);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
                    //  "       ./main path FRACTION FLAGS [-v]\n";
        return EXIT_FAILURE;
    }
    return 0;
}
