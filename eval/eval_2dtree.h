#pragma once

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

        hrc::time_point begin, end;

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
            hrc::time_point begin, end;
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
