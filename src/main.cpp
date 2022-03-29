#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapViewer.h"

#include "Run.h"

#include "QuadTreeClosestPoint.h"

#include "graphviewer.h"

#include <X11/Xlib.h>

std::list<Run> loadRuns(const std::string &filepath){
    std::list<Run> ret;

    std::ifstream is(filepath);
    size_t N; is >> N;
    for(size_t i = 0; i < N; ++i){
        ret.push_back(Run());
        Run &r = *ret.rbegin();
        size_t M;
        is >> r.id >> r.timestamp >> M;
        coord_t c;
        for(size_t j = 0; j < M; ++j){
            is >> c.lat >> c.lon;
            r.coords.push_back(c);
        }
    }

    return ret;
}

void view(int argc, const char *argv[], const MapGraph &M){
    if(argc != 4) throw std::invalid_argument("invalid number of arguments");
    int fraction = atoi(argv[2]);
    int display  = atoi(argv[3]);

    M.drawRoads(fraction, display);
}

void evalQuadTree(const MapGraph &M, const std::list<Run> &runs);

void eval(int argc, const char *argv[], const MapGraph &M, const std::list<Run> &runs){
    srand(0);
    evalQuadTree(M, runs);
}

void evalQuadTree(const MapGraph &M, const std::list<Run> &runs){
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
        // size_t i = rand()%coords.size(),
        //        j = rand()%coords.size();
        // float x = (double(rand())/double(RAND_MAX));
        // test_coords.push_back(coord_t(
        //     coords[i].getLat() * x + coords[j].getLat() * (1.0-x),
        //     coords[i].getLon() * x + coords[j].getLon() * (1.0-x)
        // ));
        test_coords.push_back(coord_t(
            minLat + (double(rand())/double(RAND_MAX))*(maxLat-minLat),
            minLon + (double(rand())/double(RAND_MAX))*(maxLon-minLon)
        ));
    }

    std::vector<size_t> szs = {
         10000,
         20000,
         30000,
         40000,
         50000,
         60000,
         70000,
         80000,
         90000,
        100000,
        110000,
        120000,
        130000,
        140000,
        150000,
        160000,
        170000,
        180000,
        190000,
        200000,
        210000,
        220000,
        230000,
        240000,
        250000,
        260000,
        270000,
        280000,
        290000,
        300000
    };

    for(const size_t &sz: szs){
        os << sz;

        std::list<coord_t> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        QuadTreeClosestPoint t;
        t.initialize(l);
        t.run();//auto begin1 = std::chrono::high_resolution_clock::now();
        for(const coord_t &u: test_coords){
            auto begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                coord_t v = t.getClosestPoint(u);
            }
            auto end = std::chrono::high_resolution_clock::now();
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;
        }//auto end1 = std::chrono::high_resolution_clock::now(); std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end1-begin1).count()) << std::endl;
        os << "\n";
    }
}

int main(int argc, char *argv[]){
    XInitThreads();

    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        MapGraph M("map/processed/AMP");
        std::list<Run> runs = loadRuns("data/pkdd-i/pkdd-i.runs");
        std::string opt = argv[1];
        if(opt == "view"      ) view      (argc, const_cast<const char **>(argv), M);
        if(opt == "eval"      ) eval      (argc, const_cast<const char **>(argv), M, runs);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        std::cout << "Usage: ./main (view | ...)\n";
                    //  "       ./main path FRACTION FLAGS [-v]\n";
        return EXIT_FAILURE;
    }
    return 0;
}
