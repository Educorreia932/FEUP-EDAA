#include <fstream>
#include <iomanip>
#include <random>

#include "EdgeType.h"
#include "MapGraph.h"
#include "MapGraphView.h"
#include "MapViewer.h"

#include "next_pow2.h"

#include "Run.h"

#include "QuadTreeClosestPoint.h"
#include "DeepVStripes.h"

#include "graphviewer.h"

void eval2DTree_BuildTime(const MapGraph &M){
    std::ofstream os("eval/2d-tree-buildtime.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, coord_t> nodes = M.getNodes();
    std::vector<coord_t> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, coord_t> &p: nodes)
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
    sort(szs.begin(), szs.end());

    for(const size_t &sz: szs){
        os << sz;
        std::cout << "Size: " << sz << std::endl;

        std::list<coord_t> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        std::chrono::_V2::system_clock::time_point begin, end;

        QuadTreeClosestPoint t;
        
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

void eval2DTree_QueryTime(const MapGraph &M, const std::vector<Run> &runs){
    std::ofstream os("eval/2d-tree-querytime.csv");
    os << std::fixed << std::setprecision(3);

    std::unordered_map<DWGraph::node_t, coord_t> nodes = M.getNodes();
    std::vector<coord_t> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, coord_t> &p: nodes)
            coords[i++] = p.second;
    }
    std::shuffle(coords.begin(), coords.end(), std::mt19937(4));
    
    std::vector<coord_t> candidates;
    for(const Run &r: runs)
        for(const coord_t &c: r.coords)
            candidates.push_back(c);

    const size_t N = 100000;
    const size_t REPEAT = 10;

    std::vector<coord_t> test_coords(N);
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

        std::list<coord_t> l;
        for(size_t i = 0; i < sz; ++i)
            l.push_back(coords.at(i));

        QuadTreeClosestPoint t;
        t.initialize(l);
        t.run();
        for(const coord_t &u: test_coords){
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

        size_t N = next_pow2(sz);
        mem += N * sizeof(coord_t) * 2; // Data structure
        mem += N * sizeof(coord_t); // Stable sort

        os << "," << mem << std::endl;
    }
}

void evalDeepVStripes_QueryTime_d(const MapGraph &M, const std::vector<Run> &runs){
    std::ofstream os("eval/deepvstripes-querytime-d.csv");
    os << std::fixed << std::setprecision(9);

    std::unordered_map<DWGraph::node_t, coord_t> nodes = M.getNodes();
    std::list<coord_t> coords(nodes.size());
    {
        size_t i = 0;
        for(const std::pair<DWGraph::node_t, coord_t> &p: nodes)
            coords.push_back(p.second);
    }

    std::vector<coord_t> candidates;
    for(const Run &r: runs)
        for(const coord_t &c: r.coords)
            candidates.push_back(c);

    std::cout << "Candidates size: " << candidates.size() << std::endl;

    const size_t N = 100000;
    // const size_t REPEAT = 10;

    std::vector<coord_t> test_coords(N);
    std::set<size_t> indexes;
    while(indexes.size() < N) indexes.insert(rand()%candidates.size());
    size_t j = 0;
    for(const size_t &i: indexes){
        test_coords[j++] = candidates[i];
    }

    const coord_t::deg_t X_AMPLITUDE = 0.35;

    std::vector<std::pair<coord_t::deg_t, size_t>> params = {
          std::make_pair(0.00001, long(log2(X_AMPLITUDE/0.00001))+1),
          std::make_pair(0.00002, long(log2(X_AMPLITUDE/0.00002))+1),
          std::make_pair(0.00003, long(log2(X_AMPLITUDE/0.00003))+1),
          std::make_pair(0.00004, long(log2(X_AMPLITUDE/0.00004))+1),
          std::make_pair(0.00005, long(log2(X_AMPLITUDE/0.00005))+1),
          std::make_pair(0.00006, long(log2(X_AMPLITUDE/0.00006))+1),
          std::make_pair(0.00007, long(log2(X_AMPLITUDE/0.00007))+1),
          std::make_pair(0.00008, long(log2(X_AMPLITUDE/0.00008))+1),
          std::make_pair(0.00009, long(log2(X_AMPLITUDE/0.00009))+1),

          std::make_pair(0.00010, long(log2(X_AMPLITUDE/0.00010))+1),std::make_pair(0.000125, long(log2(X_AMPLITUDE/0.000125))+1),std::make_pair(0.00015, long(log2(X_AMPLITUDE/0.00015))+1),std::make_pair(0.000175, long(log2(X_AMPLITUDE/0.000175))+1),
          std::make_pair(0.00020, long(log2(X_AMPLITUDE/0.00020))+1),std::make_pair(0.000225, long(log2(X_AMPLITUDE/0.000225))+1),std::make_pair(0.00025, long(log2(X_AMPLITUDE/0.00025))+1),std::make_pair(0.000275, long(log2(X_AMPLITUDE/0.000275))+1),
          std::make_pair(0.00030, long(log2(X_AMPLITUDE/0.00030))+1),std::make_pair(0.000325, long(log2(X_AMPLITUDE/0.000325))+1),std::make_pair(0.00035, long(log2(X_AMPLITUDE/0.00035))+1),std::make_pair(0.000375, long(log2(X_AMPLITUDE/0.000375))+1),
          std::make_pair(0.00040, long(log2(X_AMPLITUDE/0.00040))+1),std::make_pair(0.000425, long(log2(X_AMPLITUDE/0.000425))+1),std::make_pair(0.00045, long(log2(X_AMPLITUDE/0.00045))+1),std::make_pair(0.000475, long(log2(X_AMPLITUDE/0.000475))+1),
          std::make_pair(0.00050, long(log2(X_AMPLITUDE/0.00050))+1),std::make_pair(0.000525, long(log2(X_AMPLITUDE/0.000525))+1),std::make_pair(0.00055, long(log2(X_AMPLITUDE/0.00055))+1),std::make_pair(0.000575, long(log2(X_AMPLITUDE/0.000575))+1),
          std::make_pair(0.00060, long(log2(X_AMPLITUDE/0.00060))+1),std::make_pair(0.000625, long(log2(X_AMPLITUDE/0.000625))+1),std::make_pair(0.00065, long(log2(X_AMPLITUDE/0.00065))+1),std::make_pair(0.000675, long(log2(X_AMPLITUDE/0.000675))+1),
          std::make_pair(0.00070, long(log2(X_AMPLITUDE/0.00070))+1),std::make_pair(0.000725, long(log2(X_AMPLITUDE/0.000725))+1),std::make_pair(0.00075, long(log2(X_AMPLITUDE/0.00075))+1),std::make_pair(0.000775, long(log2(X_AMPLITUDE/0.000775))+1),
          std::make_pair(0.00080, long(log2(X_AMPLITUDE/0.00080))+1),std::make_pair(0.000825, long(log2(X_AMPLITUDE/0.000825))+1),std::make_pair(0.00085, long(log2(X_AMPLITUDE/0.00085))+1),std::make_pair(0.000875, long(log2(X_AMPLITUDE/0.000875))+1),
          std::make_pair(0.00090, long(log2(X_AMPLITUDE/0.00090))+1),std::make_pair(0.000925, long(log2(X_AMPLITUDE/0.000925))+1),std::make_pair(0.00095, long(log2(X_AMPLITUDE/0.00095))+1),std::make_pair(0.000975, long(log2(X_AMPLITUDE/0.000975))+1),
          std::make_pair(0.00100, long(log2(X_AMPLITUDE/0.00100))+1),
          std::make_pair(0.00200, long(log2(X_AMPLITUDE/0.00200))+1),
          std::make_pair(0.00300, long(log2(X_AMPLITUDE/0.00300))+1),
          std::make_pair(0.00400, long(log2(X_AMPLITUDE/0.00400))+1),
          std::make_pair(0.00500, long(log2(X_AMPLITUDE/0.00500))+1),
        //   std::make_pair(0.00600, long(log2(X_AMPLITUDE/0.00600))+1),
        //   std::make_pair(0.00700, long(log2(X_AMPLITUDE/0.00700))+1),
        //   std::make_pair(0.00800, long(log2(X_AMPLITUDE/0.00800))+1),
        //   std::make_pair(0.00900, long(log2(X_AMPLITUDE/0.00900))+1),
        //   std::make_pair(0.01000, long(log2(X_AMPLITUDE/0.01000))+1),
        //   std::make_pair(0.020, long(log2(X_AMPLITUDE/0.020))+1),
        //   std::make_pair(0.030, long(log2(X_AMPLITUDE/0.030))+1),
        //   std::make_pair(0.040, long(log2(X_AMPLITUDE/0.040))+1),
        //   std::make_pair(0.050, long(log2(X_AMPLITUDE/0.050))+1),
        //   std::make_pair(0.060, long(log2(X_AMPLITUDE/0.060))+1),
        //   std::make_pair(0.070, long(log2(X_AMPLITUDE/0.070))+1),
        //   std::make_pair(0.080, long(log2(X_AMPLITUDE/0.080))+1),
        //   std::make_pair(0.090, long(log2(X_AMPLITUDE/0.090))+1),
        //   std::make_pair(0.100, long(log2(X_AMPLITUDE/0.100))+1),
        //   std::make_pair(0.200, long(log2(X_AMPLITUDE/0.200))+1),
        //   std::make_pair(0.300, long(log2(X_AMPLITUDE/0.300))+1),
    };
    sort(params.begin(), params.end());

    os << ",t\n";
    for(const auto &pr: params){
        const coord_t::deg_t &d = pr.first;
        const size_t &n = pr.second;

        os << d;
        std::cout << "d: " << d << std::endl;

        std::chrono::_V2::system_clock::time_point begin, end;

        DeepVStripes t(d, n);
        t.initialize(coords);
        t.run();

        begin = std::chrono::high_resolution_clock::now();
        for(const coord_t &c: test_coords){
            auto ret = t.getClosestPoint(c);
        }
        end = std::chrono::high_resolution_clock::now();
        
        double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/double(test_coords.size());
        os << "," << dt;

        os << "\n";
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
        
        std::cout << "Loading runs..." << std::endl;
        std::vector<Run> runs = Run::loadRuns("res/data/pkdd15-i/pkdd15-i.runs");
        std::cout << "Loaded runs" << std::endl;

        if(opt == "2d-tree-querytime") eval2DTree_QueryTime(M, runs);
        if(opt == "deepvstripes-querytime-d") evalDeepVStripes_QueryTime_d(M, runs);
    } catch(const std::invalid_argument &e){
        std::cout << "Caught exception: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
