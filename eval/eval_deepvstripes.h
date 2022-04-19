#pragma once

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
