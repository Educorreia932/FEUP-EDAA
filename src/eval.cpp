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

void eval2DTree(const MapGraph &M, const std::vector<Run> &runs){
    eval2DTree_QueryTime(M, runs);
}

void eval2DTree_QueryTime(const MapGraph &M, const std::vector<Run> &runs){
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
    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        MapGraph M("res/map/processed/AMP");
        std::vector<Run> runs = Run::loadRuns("res/data/pkdd15-i/pkdd15-i.runs");
        std::string opt = argv[1];
        if(opt == "2d-tree") eval2DTree(M, runs);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
