#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <unordered_map>

#include "DeepVStripes.h"
#include "Dijkstra.h"
#include "EdgeType.h"
#include "Kosaraju.h"
#include "MapGraph.h"
#include "K2DTreeClosestPoint.h"
#include "Trip.h"
#include "VStripesRadius.h"

#include "utils.h"

const double METERS_TO_MILLIMS = 1000.0;
const double MILLIMS_TO_METERS = 1.0/1000.0;

void eval2DTree_BuildTime(const MapGraph &M){
    std::ofstream os("eval/2d-tree-buildtime.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::vector<Coord> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(0));

    const size_t REPEAT = 5;

    std::vector<size_t> szs = {
             1,
          1000,
          2000,
          3000,
          4000,
          5000,
          6000,
          7000,
          8000,
          9000,
         10000, 12500, 15000, 17500,
         20000, 22500, 25000, 27500,
         30000, 32500, 35000, 37500,
         40000, 42500, 45000, 47500,
         50000, 52500, 55000, 57500,
         60000, 62500, 65000, 67500,
         70000, 72500, 75000, 77500,
         80000, 82500, 85000, 87500,
         90000, 92500, 95000, 97500,
        100000,102500,105000,107500,
        110000,112500,115000,117500,
        120000,122500,125000,127500,
        130000,132500,135000,137500,
        140000,142500,145000,147500,
        150000,152500,155000,157500,
        160000,162500,165000,167500,
        170000,172500,175000,177500,
        180000,182500,185000,187500,
        190000,192500,195000,197500,
        200000,202500,205000,207500,
        210000,212500,215000,217500,
        220000,222500,225000,227500,
        230000,232500,235000,237500,
        240000,242500,245000,247500,
        250000,252500,255000,257500,
        260000,262500,265000,267500,
        270000,272500,275000,277500,
        280000,282500,285000,287500,
        290000,292500,295000,297500,
        300000,
                            (1<< 1)-1,(1<< 1),(1<< 1)+1,(1<< 1)+2,(1<< 1)+3,(1<< 1)+4,
        (1<< 2)-3,(1<< 2)-2,(1<< 2)-1,(1<< 2),(1<< 2)+1,(1<< 2)+2,(1<< 2)+3,(1<< 2)+4,
        (1<< 3)-3,(1<< 3)-2,(1<< 3)-1,(1<< 3),(1<< 3)+1,(1<< 3)+2,(1<< 3)+3,(1<< 3)+4,
        (1<< 4)-3,(1<< 4)-2,(1<< 4)-1,(1<< 4),(1<< 4)+1,(1<< 4)+2,(1<< 4)+3,(1<< 4)+4,
        (1<< 5)-3,(1<< 5)-2,(1<< 5)-1,(1<< 5),(1<< 5)+1,(1<< 5)+2,(1<< 5)+3,(1<< 5)+4,
        (1<< 6)-3,(1<< 6)-2,(1<< 6)-1,(1<< 6),(1<< 6)+1,(1<< 6)+2,(1<< 6)+3,(1<< 6)+4,
        (1<< 7)-3,(1<< 7)-2,(1<< 7)-1,(1<< 7),(1<< 7)+1,(1<< 7)+2,(1<< 7)+3,(1<< 7)+4,
        (1<< 8)-3,(1<< 8)-2,(1<< 8)-1,(1<< 8),(1<< 8)+1,(1<< 8)+2,(1<< 8)+3,(1<< 8)+4,
        (1<< 9)-3,(1<< 9)-2,(1<< 9)-1,(1<< 9),(1<< 9)+1,(1<< 9)+2,(1<< 9)+3,(1<< 9)+4,
        (1<<10)-3,(1<<10)-2,(1<<10)-1,(1<<10),(1<<10)+1,(1<<10)+2,(1<<10)+3,(1<<10)+4,
        (1<<11)-3,(1<<11)-2,(1<<11)-1,(1<<11),(1<<11)+1,(1<<11)+2,(1<<11)+3,(1<<11)+4,
        (1<<12)-3,(1<<12)-2,(1<<12)-1,(1<<12),(1<<12)+1,(1<<12)+2,(1<<12)+3,(1<<12)+4,
        (1<<13)-3,(1<<13)-2,(1<<13)-1,(1<<13),(1<<13)+1,(1<<13)+2,(1<<13)+3,(1<<13)+4,
        (1<<14)-3,(1<<14)-2,(1<<14)-1,(1<<14),(1<<14)+1,(1<<14)+2,(1<<14)+3,(1<<14)+4,
        (1<<15)-3,(1<<15)-2,(1<<15)-1,(1<<15),(1<<15)+1,(1<<15)+2,(1<<15)+3,(1<<15)+4,
        (1<<16)-3,(1<<16)-2,(1<<16)-1,(1<<16),(1<<16)+1,(1<<16)+2,(1<<16)+3,(1<<16)+4,
        (1<<17)-3,(1<<17)-2,(1<<17)-1,(1<<17),(1<<17)+1,(1<<17)+2,(1<<17)+3,(1<<17)+4,
        (1<<18)-3,(1<<18)-2,(1<<18)-1,(1<<18),(1<<18)+1,(1<<18)+2,(1<<18)+3,(1<<18)+4,
    };
    std::sort(szs.begin(), szs.end());

    for(const size_t &sz: szs){
        os << sz;
        std::cout << "Size: " << sz << std::endl;

        std::list<Vector2> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        std::chrono::_V2::system_clock::time_point begin, end;

        K2DTreeClosestPoint t;
        
        begin = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i < REPEAT; ++i){
            t.initialize(l);
            t.run();
        }
        end = std::chrono::high_resolution_clock::now();
        
        double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
        os << "," << dt;

        os << "\n";
    }
}

void eval2DTree_QueryTime(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/2d-tree-querytime.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::vector<Coord> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(4));
    
    std::vector<Coord> candidates;
    for(const Trip &r: trips)
        for(const Coord &c: r.coords)
            candidates.push_back(c);

    const size_t N = 100000;
    const size_t REPEAT = 10;

    std::vector<Coord> test_coords(N);
    std::set<size_t> indexes;
    while(indexes.size() < N) indexes.insert(rand()%candidates.size());
    size_t j = 0;
    for(const size_t &i: indexes){
        test_coords[j++] = candidates[i];
    }

    std::vector<size_t> szs = {
             1,
          1000,
          2000,
          3000,
          4000,
          5000,
          6000,
          7000,
          8000,
          9000,
         10000, 12500, 15000, 17500,
         20000, 22500, 25000, 27500,
         30000, 32500, 35000, 37500,
         40000, 42500, 45000, 47500,
         50000, 52500, 55000, 57500,
         60000, 62500, 65000, 67500,
         70000, 72500, 75000, 77500,
         80000, 82500, 85000, 87500,
         90000, 92500, 95000, 97500,
        100000,102500,105000,107500,
        110000,112500,115000,117500,
        120000,122500,125000,127500,
        130000,132500,135000,137500,
        140000,142500,145000,147500,
        150000,152500,155000,157500,
        160000,162500,165000,167500,
        170000,172500,175000,177500,
        180000,182500,185000,187500,
        190000,192500,195000,197500,
        200000,202500,205000,207500,
        210000,212500,215000,217500,
        220000,222500,225000,227500,
        230000,232500,235000,237500,
        240000,242500,245000,247500,
        250000,252500,255000,257500,
        260000,262500,265000,267500,
        270000,272500,275000,277500,
        280000,282500,285000,287500,
        290000,292500,295000,297500,
        300000,
                            (1<< 1)-1,(1<< 1),(1<< 1)+1,(1<< 1)+2,(1<< 1)+3,(1<< 1)+4,
        (1<< 2)-3,(1<< 2)-2,(1<< 2)-1,(1<< 2),(1<< 2)+1,(1<< 2)+2,(1<< 2)+3,(1<< 2)+4,
        (1<< 3)-3,(1<< 3)-2,(1<< 3)-1,(1<< 3),(1<< 3)+1,(1<< 3)+2,(1<< 3)+3,(1<< 3)+4,
        (1<< 4)-3,(1<< 4)-2,(1<< 4)-1,(1<< 4),(1<< 4)+1,(1<< 4)+2,(1<< 4)+3,(1<< 4)+4,
        (1<< 5)-3,(1<< 5)-2,(1<< 5)-1,(1<< 5),(1<< 5)+1,(1<< 5)+2,(1<< 5)+3,(1<< 5)+4,
        (1<< 6)-3,(1<< 6)-2,(1<< 6)-1,(1<< 6),(1<< 6)+1,(1<< 6)+2,(1<< 6)+3,(1<< 6)+4,
        (1<< 7)-3,(1<< 7)-2,(1<< 7)-1,(1<< 7),(1<< 7)+1,(1<< 7)+2,(1<< 7)+3,(1<< 7)+4,
        (1<< 8)-3,(1<< 8)-2,(1<< 8)-1,(1<< 8),(1<< 8)+1,(1<< 8)+2,(1<< 8)+3,(1<< 8)+4,
        (1<< 9)-3,(1<< 9)-2,(1<< 9)-1,(1<< 9),(1<< 9)+1,(1<< 9)+2,(1<< 9)+3,(1<< 9)+4,
        (1<<10)-3,(1<<10)-2,(1<<10)-1,(1<<10),(1<<10)+1,(1<<10)+2,(1<<10)+3,(1<<10)+4,
        (1<<11)-3,(1<<11)-2,(1<<11)-1,(1<<11),(1<<11)+1,(1<<11)+2,(1<<11)+3,(1<<11)+4,
        (1<<12)-3,(1<<12)-2,(1<<12)-1,(1<<12),(1<<12)+1,(1<<12)+2,(1<<12)+3,(1<<12)+4,
        (1<<13)-3,(1<<13)-2,(1<<13)-1,(1<<13),(1<<13)+1,(1<<13)+2,(1<<13)+3,(1<<13)+4,
        (1<<14)-3,(1<<14)-2,(1<<14)-1,(1<<14),(1<<14)+1,(1<<14)+2,(1<<14)+3,(1<<14)+4,
        (1<<15)-3,(1<<15)-2,(1<<15)-1,(1<<15),(1<<15)+1,(1<<15)+2,(1<<15)+3,(1<<15)+4,
        (1<<16)-3,(1<<16)-2,(1<<16)-1,(1<<16),(1<<16)+1,(1<<16)+2,(1<<16)+3,(1<<16)+4,
        (1<<17)-3,(1<<17)-2,(1<<17)-1,(1<<17),(1<<17)+1,(1<<17)+2,(1<<17)+3,(1<<17)+4,
        (1<<18)-3,(1<<18)-2,(1<<18)-1,(1<<18),(1<<18)+1,(1<<18)+2,(1<<18)+3,(1<<18)+4,
    };
    sort(szs.begin(), szs.end());

    for(const size_t &sz: szs){
        os << sz;
        std::cout << "Size: " << sz << std::endl;

        std::list<Vector2> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        K2DTreeClosestPoint t;
        t.initialize(l);
        t.run();
        for(const Coord &u: test_coords){
            std::chrono::_V2::system_clock::time_point begin, end;
            begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                t.getClosestPoint(u);
            }
            end = std::chrono::high_resolution_clock::now();
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;
        }
        os << "\n";
    }
}

void eval2DTree_BuildMemory(){
    std::ofstream os("eval/2d-tree-buildmem.csv");
    
    std::vector<size_t> szs = {
             1,
          1000,
          2000,
          3000,
          4000,
          5000,
          6000,
          7000,
          8000,
          9000,
         10000, 12500, 15000, 17500,
         20000, 22500, 25000, 27500,
         30000, 32500, 35000, 37500,
         40000, 42500, 45000, 47500,
         50000, 52500, 55000, 57500,
         60000, 62500, 65000, 67500,
         70000, 72500, 75000, 77500,
         80000, 82500, 85000, 87500,
         90000, 92500, 95000, 97500,
        100000,102500,105000,107500,
        110000,112500,115000,117500,
        120000,122500,125000,127500,
        130000,132500,135000,137500,
        140000,142500,145000,147500,
        150000,152500,155000,157500,
        160000,162500,165000,167500,
        170000,172500,175000,177500,
        180000,182500,185000,187500,
        190000,192500,195000,197500,
        200000,202500,205000,207500,
        210000,212500,215000,217500,
        220000,222500,225000,227500,
        230000,232500,235000,237500,
        240000,242500,245000,247500,
        250000,252500,255000,257500,
        260000,262500,265000,267500,
        270000,272500,275000,277500,
        280000,282500,285000,287500,
        290000,292500,295000,297500,
        300000,
        (1<< 1),(1<< 1)+1,
        (1<< 2),(1<< 2)+1,
        (1<< 3),(1<< 3)+1,
        (1<< 4),(1<< 4)+1,
        (1<< 5),(1<< 5)+1,
        (1<< 6),(1<< 6)+1,
        (1<< 7),(1<< 7)+1,
        (1<< 8),(1<< 8)+1,
        (1<< 9),(1<< 9)+1,
        (1<<10),(1<<10)+1,
        (1<<11),(1<<11)+1,
        (1<<12),(1<<12)+1,
        (1<<13),(1<<13)+1,
        (1<<14),(1<<14)+1,
        (1<<15),(1<<15)+1,
        (1<<16),(1<<16)+1,
        (1<<17),(1<<17)+1,
        (1<<18),(1<<18)+1,
    };
    sort(szs.begin(), szs.end());
    os << ",mem\n";
    for(const size_t &sz: szs){
        os << sz;
        std::cout << "Size: " << sz << std::endl;

        size_t mem = 0;

        size_t N = utils::nextPow2(sz);
        mem += N * sizeof(Vector2); // Leaf nodes
        mem += N * sizeof(double); // Non-leaf nodes

        os << "," << mem << std::endl;
    }
}

void evalDeepVStripes_BuildTime(const MapGraph &M){
    std::ofstream os("eval/deepvstripes-buildtime.csv");
    os << std::fixed;

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::vector<Coord> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(4));

    const size_t REPEAT = 5;

    std::vector<size_t> Ls = {
        1,
        3,
        6,
        9,
        12,
        15,
        18,
        21,
        24,
    };

    std::vector<size_t> szs = {
             1,
           200,
           400,
           600,
           800,
          1000,
          2000,
          4000,
          6000,
          8000,
         10000,
         12500,
         15000,
         17500,
         20000,
         25000,
         30000,
         40000,
         50000,
         60000,
         70000,
         80000,
         90000,
        100000,
        120000,
        140000,
        160000,
        180000,
        200000,
        220000,
        240000,
        260000,
        280000,
        300000,
    };
    sort(szs.begin(), szs.end());

    const double d = 0.0003;

    os << "L";
    for(const size_t &sz: szs){
        os << "," << sz;
    }
    os << "\n";

    for(const size_t &L: Ls){
        os << std::setprecision(0) << L << std::setprecision(9);
        std::cout << "L: " << L << std::endl;

        DeepVStripes t(d, L);

        std::list<Vector2> l;

        for(const size_t &sz: szs){
            std::cout << "Size: " << sz << std::endl;

            for(size_t i = l.size(); i < sz; ++i)
                l.push_back(coords[i]);
            
            std::chrono::_V2::system_clock::time_point begin, end;

            begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                t.initialize(l);
                t.run();
            }
            end = std::chrono::high_resolution_clock::now();
            
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;

        }
        os << "\n";
    }
}

void evalDeepVStripes_QueryTime_d(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/deepvstripes-querytime-d.csv");
    os << std::fixed << std::setprecision(9);

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::list<Vector2> coords(nodes.size());
    for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
        coords.push_back(p.second);

    std::vector<Coord> candidates;
    for(const Trip &r: trips)
        for(const Coord &c: r.coords)
            candidates.push_back(c);

    std::cout << "Candidates size: " << candidates.size() << std::endl;

    const size_t N = 100000;
    const size_t REPEAT = 10;

    std::vector<Coord> test_coords(N);
    std::set<size_t> indexes;
    while(indexes.size() < N) indexes.insert(rand()%candidates.size());
    size_t j = 0;
    for(const size_t &i: indexes){
        test_coords[j++] = candidates[i];
    }

    const double X_AMPLITUDE = 0.35;

    std::vector<std::pair<double, size_t>> params = {
        std::make_pair(0.000001, long(log2(X_AMPLITUDE/0.000001))+1),

        std::make_pair(0.00001, long(log2(X_AMPLITUDE/0.00001))+1),
        std::make_pair(0.00002, long(log2(X_AMPLITUDE/0.00002))+1),
        std::make_pair(0.00004, long(log2(X_AMPLITUDE/0.00004))+1),
        std::make_pair(0.00006, long(log2(X_AMPLITUDE/0.00006))+1),
        std::make_pair(0.00008, long(log2(X_AMPLITUDE/0.00008))+1),

        std::make_pair(0.00010, long(log2(X_AMPLITUDE/0.00010))+1),
        std::make_pair(0.00012, long(log2(X_AMPLITUDE/0.00012))+1),
        std::make_pair(0.00014, long(log2(X_AMPLITUDE/0.00014))+1),
        std::make_pair(0.00016, long(log2(X_AMPLITUDE/0.00016))+1),
        std::make_pair(0.00018, long(log2(X_AMPLITUDE/0.00018))+1),


        std::make_pair(0.000200, long(log2(X_AMPLITUDE/0.000200))+1),
        std::make_pair(0.000225, long(log2(X_AMPLITUDE/0.000225))+1),
        std::make_pair(0.000250, long(log2(X_AMPLITUDE/0.000250))+1),
        std::make_pair(0.000275, long(log2(X_AMPLITUDE/0.000275))+1),
        
        std::make_pair(0.00030, long(log2(X_AMPLITUDE/0.00030))+1),std::make_pair(0.00035, long(log2(X_AMPLITUDE/0.00035))+1),
        std::make_pair(0.00040, long(log2(X_AMPLITUDE/0.00040))+1),std::make_pair(0.00045, long(log2(X_AMPLITUDE/0.00045))+1),
        std::make_pair(0.00050, long(log2(X_AMPLITUDE/0.00050))+1),std::make_pair(0.00055, long(log2(X_AMPLITUDE/0.00055))+1),
        std::make_pair(0.00060, long(log2(X_AMPLITUDE/0.00060))+1),std::make_pair(0.00065, long(log2(X_AMPLITUDE/0.00065))+1),
        std::make_pair(0.00070, long(log2(X_AMPLITUDE/0.00070))+1),std::make_pair(0.00075, long(log2(X_AMPLITUDE/0.00075))+1),
        std::make_pair(0.00080, long(log2(X_AMPLITUDE/0.00080))+1),std::make_pair(0.00085, long(log2(X_AMPLITUDE/0.00085))+1),
        std::make_pair(0.00090, long(log2(X_AMPLITUDE/0.00090))+1),std::make_pair(0.00095, long(log2(X_AMPLITUDE/0.00095))+1),
        std::make_pair(0.00100, long(log2(X_AMPLITUDE/0.00100))+1),
        std::make_pair(0.00110, long(log2(X_AMPLITUDE/0.00110))+1),
        std::make_pair(0.00120, long(log2(X_AMPLITUDE/0.00120))+1),
        std::make_pair(0.00130, long(log2(X_AMPLITUDE/0.00130))+1),
        std::make_pair(0.00140, long(log2(X_AMPLITUDE/0.00140))+1),
        std::make_pair(0.00150, long(log2(X_AMPLITUDE/0.00150))+1),
        std::make_pair(0.00160, long(log2(X_AMPLITUDE/0.00160))+1),
        std::make_pair(0.00170, long(log2(X_AMPLITUDE/0.00170))+1),
        std::make_pair(0.00180, long(log2(X_AMPLITUDE/0.00180))+1),
        std::make_pair(0.00190, long(log2(X_AMPLITUDE/0.00190))+1),
        std::make_pair(0.00200, long(log2(X_AMPLITUDE/0.00200))+1),
        std::make_pair(0.00210, long(log2(X_AMPLITUDE/0.00210))+1),
        std::make_pair(0.00220, long(log2(X_AMPLITUDE/0.00220))+1),
        std::make_pair(0.00230, long(log2(X_AMPLITUDE/0.00230))+1),
        std::make_pair(0.00240, long(log2(X_AMPLITUDE/0.00240))+1),
        std::make_pair(0.00250, long(log2(X_AMPLITUDE/0.00250))+1),
        // std::make_pair(0.00260, long(log2(X_AMPLITUDE/0.00100))+1),
        // std::make_pair(0.00270, long(log2(X_AMPLITUDE/0.00100))+1),
        // std::make_pair(0.00280, long(log2(X_AMPLITUDE/0.00100))+1),
        // std::make_pair(0.00290, long(log2(X_AMPLITUDE/0.00100))+1),
        // std::make_pair(0.00300, long(log2(X_AMPLITUDE/0.00300))+1),
    };
    sort(params.begin(), params.end());


    for(const auto &pr: params){
        const double &d = pr.first;
        const size_t &n = pr.second;

        os << d;
        std::cout << "d: " << d << std::endl;

        DeepVStripes t(d, n);
        t.initialize(coords);
        t.run();

        for(const Coord &u: test_coords){
            std::chrono::_V2::system_clock::time_point begin, end;
            begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                t.getClosestPoint(u);
            }
            end = std::chrono::high_resolution_clock::now();
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;
        }
        os << "\n";
    }
}

void evalDeepVStripes_QueryTime(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/deepvstripes-querytime.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::vector<Coord> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(4));
    
    std::vector<Coord> candidates;
    for(const Trip &r: trips)
        for(const Coord &c: r.coords)
            candidates.push_back(c);

    const size_t N = 100000;
    const size_t REPEAT = 10;

    std::vector<Coord> test_coords(N);
    std::set<size_t> indexes;
    while(indexes.size() < N) indexes.insert(rand()%candidates.size());
    size_t j = 0;
    for(const size_t &i: indexes){
        test_coords[j++] = candidates[i];
    }

    std::vector<size_t> szs = {
             1,
           200,
           400,
           600,
           800,
          1000,
          2000,
          4000,
          6000,
          8000,
         10000, 12500, 15000, 17500,
         20000, 22500, 25000, 27500,
         30000, 32500, 35000, 37500,
         40000, 45000,
         50000, 55000,
         60000, 65000,
         70000, 75000,
         80000, 85000,
         90000, 95000,
        100000,105000,
        110000,115000,
        120000,125000,
        130000,135000,
        140000,145000,
        150000,155000,
        160000,165000,
        170000,175000,
        180000,185000,
        190000,195000,
        200000,205000,
        210000,215000,
        220000,225000,
        230000,235000,
        240000,245000,
        250000,255000,
        260000,265000,
        270000,275000,
        280000,285000,
        290000,295000,
        300000,
    };
    sort(szs.begin(), szs.end());

    const double X_AMPLITUDE = 0.35;
    const double d = 0.0003;

    for(const size_t &sz: szs){
        os << sz;
        std::cout << "Size: " << sz << std::endl;

        std::list<Vector2> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        DeepVStripes t(d, long(log2(X_AMPLITUDE/d))+2);
        t.initialize(l);
        t.run();
        for(const Coord &u: test_coords){
            std::chrono::_V2::system_clock::time_point begin, end;
            begin = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < REPEAT; ++i){
                t.getClosestPoint(u);
            }
            end = std::chrono::high_resolution_clock::now();
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT);
            os << "," << dt;
        }
        os << "\n";
    }
}

void evalDeepVStripes_QueryTime_nd(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/deepvstripes-querytime-nd.csv");
    os << std::fixed;

    std::unordered_map<DWGraph::node_t, Coord> nodes = M.getNodes();
    std::vector<Coord> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, Coord> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(4));
    
    std::vector<Coord> candidates;
    for(const Trip &r: trips)
        for(const Coord &c: r.coords)
            candidates.push_back(c);

    const size_t N = 100000;
    const size_t REPEAT = 10;

    std::vector<Coord> test_coords(N);
    std::set<size_t> indexes;
    while(indexes.size() < N) indexes.insert(rand()%candidates.size());
    size_t j = 0;
    for(const size_t &i: indexes){
        test_coords[j++] = candidates[i];
    }

    std::vector<double> ds = {
        0.0001,
        0.0002,
        0.0003,
        0.0004,
        0.0005,
        0.0006,
        0.0007,
        0.0008,
        0.0009,
        0.0010,
    };

    std::vector<size_t> szs = {
             1,
           200,
           400,
           600,
           800,
          1000,
          2000,
          4000,
          6000,
          8000,
         10000,
         12500,
         15000,
         17500,
         20000,
         25000,
         30000,
         40000,
         50000,
         60000,
         70000,
         80000,
         90000,
        100000,
        120000,
        140000,
        160000,
        180000,
        200000,
        220000,
        240000,
        260000,
        280000,
        300000,
    };
    sort(szs.begin(), szs.end());

    const double X_AMPLITUDE = 0.35;

    os << "d";
    for(const size_t &sz: szs){
        os << "," << sz;
    }
    os << "\n";

    for(const double &d: ds){
        os << std::setprecision(4) << d << std::setprecision(9);
        std::cout << "d: " << d << std::endl;

        DeepVStripes t(d, long(log2(X_AMPLITUDE/d))+2);

        std::list<Vector2> l;

        for(const size_t &sz: szs){
            std::cout << "Size: " << sz << std::endl;

            for(size_t i = l.size(); i < sz; ++i)
                l.push_back(coords[i]);

            t.initialize(l);
            t.run();
            
            std::chrono::_V2::system_clock::time_point begin, end;

            begin = std::chrono::high_resolution_clock::now();
            for(const Coord &u: test_coords){
                for(size_t i = 0; i < REPEAT; ++i){
                    t.getClosestPoint(u);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            
            double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(REPEAT * test_coords.size());
            os << "," << dt;

        }
        os << "\n";
    }
}

DWGraph::DWGraph getSCC(const MapGraph &G){
    std::cout << "Calculating SCC..." << std::endl;
    DWGraph::DWGraph distGraph = G.getDistanceGraph();
    DUGraph duDistGraph = (DUGraph)distGraph;
    Kosaraju kosaraju;
    kosaraju.initialize(&duDistGraph);
    kosaraju.run();
    DWGraph::node_t root = kosaraju.get_scc(4523960191);
    for(const DWGraph::node_t &u: duDistGraph.getNodes()){
        if(kosaraju.get_scc(u) != root){
            distGraph.removeNode(u);
        }
    }
    std::cout << "Calculated SCC" << std::endl;
    return distGraph;
}

void evalHMM_VStripes(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-vstripes.csv");
    os << std::fixed;

    const size_t N = 100000;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    std::chrono::_V2::system_clock::time_point begin, end; double dt;

    os << "i,VStripes\n";

    for(size_t i = 0; i < N; ++i){
        os << i;
        if(i%1000 == 0) std::cout << "i=" << i << "/" << N << std::endl;

        const size_t idx = rand()%trips.size();
        const auto &trip = trips[idx].coords;
        const std::vector<Coord> &Y = trip;
        const size_t &T = Y.size();

        begin = std::chrono::high_resolution_clock::now();
        for(size_t t = 0; t < T; ++t){
            std::vector<Coord> v = closestPointsInRadius.getClosestPoints(Y.at(t));
        }
        end = std::chrono::high_resolution_clock::now();

        dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count());
        os << "," << dt << "\n";
    }
}

void evalHMM_Astar(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-astar.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;

    const size_t N = 1000;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    
    AstarFew shortestPathFew(G.getNodes(), METERS_TO_MILLIMS, 650*METERS_TO_MILLIMS);

    size_t failed = 0;

    os << "i,A*\n";

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            if(n%10 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx << ", tripId=" << trips[idx].id << ", failed=" << failed << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            for(size_t t = 0; t < T; ++t){
                std::vector<Coord> v = closestPointsInRadius.getClosestPoints(Y.at(t));
                if(v.empty()) throw std::runtime_error("Location t=" + std::to_string(t) + " has no candidates");
                for(const Coord &c: v){
                    if(!Sv.count(c)){
                        long id = Sv.size();
                        Sv[c] = id;
                        S.push_back(c);
                        idxToNode.push_back(G.coordToNode(c));
                    }
                    candidateStates.at(t).insert(Sv.at(c));
                }
            }
            const size_t &K = Sv.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    shortestPathFew.initialize(&distGraph, idxToNode.at(i), l);
                    shortestPathFew.run();
                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::weight_t d = shortestPathFew.getPathWeight(idxToNode.at(j));
                        double df = double(d)*MILLIMS_TO_METERS;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
                }

                for(
                    auto it = candidateStates.at(t+1).begin();
                    it != candidateStates.at(t+1).end();
                ){
                    size_t j = *it;

                    double dBest = fINF;
                    for(size_t i: candidateStates.at(t)){
                        dBest = std::min(dBest, distMatrix[i][j]);
                    }

                    if(dBest >= fINF) it = candidateStates.at(t+1).erase(it);
                    else ++it;
                }
            }

            end = std::chrono::high_resolution_clock::now();
            dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count());
            os << n << "," << dt << "\n";
        } catch(const std::exception &e){
            std::cout << "Failed: " << e.what() << std::endl;
            --n;
            ++failed;
            // throw e;
        }
    }
}

void evalHMM_Dijkstra(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-dijkstra.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;

    const size_t N = 100000; // 1000
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    std::unordered_map<DWGraph::node_t, Dijkstra> dijkstras;

    size_t failed = 0;

    os << "i,Dijkstra\n";

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            if(n%100 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx << ", tripId=" << trips[idx].id << ", failed=" << failed << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            for(size_t t = 0; t < T; ++t){
                std::vector<Coord> v = closestPointsInRadius.getClosestPoints(Y.at(t));
                if(v.empty()) throw std::runtime_error("Location t=" + std::to_string(t) + " has no candidates");
                for(const Coord &c: v){
                    if(!Sv.count(c)){
                        long id = Sv.size();
                        Sv[c] = id;
                        S.push_back(c);
                        idxToNode.push_back(G.coordToNode(c));
                    }
                    candidateStates.at(t).insert(Sv.at(c));
                }
            }
            const size_t &K = Sv.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    auto u = idxToNode.at(i);
                    if(dijkstras.count(u) == 0){
                        dijkstras.emplace(u, 650*METERS_TO_MILLIMS);
                        dijkstras.at(u).initialize(&distGraph, u);
                        dijkstras.at(u).run();
                    }

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::weight_t d = dijkstras.at(idxToNode.at(i)).getPathWeight(idxToNode.at(j));
                        double df = double(d)*MILLIMS_TO_METERS; //std::cout << "i,j=" << i << "," << j << ", df=" << df << std::endl;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
                }

                for(
                    auto it = candidateStates.at(t+1).begin();
                    it != candidateStates.at(t+1).end();
                ){
                    size_t j = *it;

                    double dBest = fINF;
                    for(size_t i: candidateStates.at(t)){
                        dBest = std::min(dBest, distMatrix[i][j]);
                    }

                    if(dBest >= fINF) it = candidateStates.at(t+1).erase(it);
                    else ++it;
                }
            }

            end = std::chrono::high_resolution_clock::now();
            dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count());
            os << n << "," << dt << "\n";
        } catch(const std::exception &e){
            std::cout << "Failed: " << e.what() << std::endl;
            --n;
            ++failed;
            // throw e;
        }
    }
}

int main(int argc, char *argv[]){
    srand(1234);
    try {
        if(argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];
        
        if(opt == "2d-tree-buildmem" ){ eval2DTree_BuildMemory(); return 0; }

        std::cout << "Loading map..." << std::endl;
        MapGraph M("res/map/processed/AMP");
        std::cout << "Loaded map" << std::endl;

        if(opt == "2d-tree-buildtime"){ eval2DTree_BuildTime  (M); return 0; }
        if(opt == "deepvstripes-buildtime"){ evalDeepVStripes_BuildTime(M); return 0; }
        
        std::cout << "Loading trips..." << std::endl;
        std::vector<Trip> trips = Trip::loadTripsBin("res/data/pkdd15-i/pkdd15-i.trips.bin");
        std::cout << "Loaded trips" << std::endl;

        if(opt == "2d-tree-querytime") eval2DTree_QueryTime(M, trips);
        if(opt == "deepvstripes-querytime-d") evalDeepVStripes_QueryTime_d(M, trips);
        if(opt == "deepvstripes-querytime") evalDeepVStripes_QueryTime(M, trips);
        if(opt == "deepvstripes-querytime-nd") evalDeepVStripes_QueryTime_nd(M, trips);
        if(opt == "hmm-vstripes") evalHMM_VStripes(M, trips);
        if(opt == "hmm-astar") evalHMM_Astar(M, trips);
        if(opt == "hmm-dijkstra") evalHMM_Dijkstra(M, trips);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
