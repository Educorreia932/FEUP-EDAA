#pragma once

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

void getCandidates(
    const MapGraph &G,
    VStripesRadius &closestPointsInRadius,
    const std::vector<Coord> &Y,
    std::vector<Coord> &S,
    std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> &Sv,
    std::vector<DWGraph::node_t> &idxToNode,
    std::vector<std::set<long>> &candidateStates
){
    const size_t &T = Y.size();

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
}

void evalHMM_Dijkstra_earlyStopping(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-dijkstra-s.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 100;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    size_t failed = 0;

    os << "i,Dijkstra-s\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%1 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = Sv.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    DijkstraFew shortestPaths;
                    shortestPaths.initialize(&distGraph, u, l);
                    shortestPaths.run();

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        DWGraph::weight_t d = shortestPaths.getPathWeight(v);
                        double df = (d == iINF ? fINF : double(d)*MILLIMS_TO_METERS);
                        distMatrix[i][j] = df;
                    }
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

void evalHMM_Dijkstra_earlyStopping_dMax(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-dijkstra-sd.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 10000;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    size_t failed = 0;

    os << "i,Dijkstra-sd\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%1 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = Sv.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    DijkstraFew shortestPaths(650 * METERS_TO_MILLIMS);
                    shortestPaths.initialize(&distGraph, u, l);
                    shortestPaths.run();

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        DWGraph::weight_t d = shortestPaths.getPathWeight(v);
                        double df = (d == iINF ? fINF : double(d)*MILLIMS_TO_METERS);
                        distMatrix[i][j] = df;
                    }
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

void evalHMM_Astar(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-astar.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 50;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    size_t failed = 0;

    const std::unordered_map<DWGraph::node_t, Coord> &nodesToCoords = G.getNodes();

    os << "i,A*\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%1 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        Astar::heuristic_t *h = new MapGraph::DistanceHeuristic(
                            nodesToCoords,
                            nodesToCoords.at(v),
                            650*METERS_TO_MILLIMS
                        );
                        Astar shortestPath(h);
                        shortestPath.initialize(&distGraph, u, v);
                        shortestPath.run();
                        DWGraph::weight_t d = shortestPath.getPathWeight();
                        double df = double(d)*MILLIMS_TO_METERS;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
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

void evalHMM_Astar_dMax(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-astar-d.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

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

    size_t failed = 0;

    const std::unordered_map<DWGraph::node_t, Coord> &nodesToCoords = G.getNodes();

    os << "i,A*-d\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%1 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        Astar::heuristic_t *h = new MapGraph::DistanceHeuristic(
                            nodesToCoords,
                            nodesToCoords.at(v),
                            METERS_TO_MILLIMS
                        );
                        Astar shortestPath(h, 650*METERS_TO_MILLIMS);
                        shortestPath.initialize(&distGraph, u, v);
                        shortestPath.run();
                        DWGraph::weight_t d = shortestPath.getPathWeight();
                        double df = double(d)*MILLIMS_TO_METERS;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
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

void evalHMM_AstarFew(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-astarfew.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 100;
    const double d = 50;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));

    VStripesRadius closestPointsInRadius;
    closestPointsInRadius.initialize(l, d);
    closestPointsInRadius.run();

    
    AstarFew shortestPathFew(G.getNodes(), METERS_TO_MILLIMS);

    size_t failed = 0;

    os << "i,A*few\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%1 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    shortestPathFew.initialize(&distGraph, u, l);
                    shortestPathFew.run();
                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        DWGraph::weight_t d = shortestPathFew.getPathWeight(v);
                        double df = double(d)*MILLIMS_TO_METERS;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
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

void evalHMM_AstarFew_dMax(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-astarfew-d.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 10000;
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

    os << "i,A*few-d\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%10 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

            begin = std::chrono::high_resolution_clock::now();

            for(size_t t = 0; t+1 < T; ++t){
                std::list<DWGraph::node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    DWGraph::node_t u = idxToNode.at(i);

                    shortestPathFew.initialize(&distGraph, u, l);
                    shortestPathFew.run();
                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        DWGraph::weight_t d = shortestPathFew.getPathWeight(v);
                        double df = double(d)*MILLIMS_TO_METERS;
                        distMatrix[i][j] = (d == iINF ? fINF : df);
                    }
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

void evalHMM_DijkstraCache(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-dijkstra.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

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

    DijkstraOnRequest shortestPaths;
    shortestPaths.initialize(&distGraph);

    size_t failed = 0;

    os << "i,DijkstraCache\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%100 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

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
                    DWGraph::node_t u = idxToNode.at(i);

                    for(size_t j: candidateStates.at(t+1)){
                        DWGraph::node_t v = idxToNode.at(j);

                        DWGraph::weight_t d = shortestPaths.getPathWeight(u, v);
                        double df = (d == iINF ? fINF : double(d)*MILLIMS_TO_METERS);
                        distMatrix[i][j] = df;
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

std::vector<std::vector<double>> getDistMatrix(
    const size_t &K,
    const size_t &T,
    const std::vector<std::set<long>> &candidateStates,
    const std::vector<DWGraph::node_t> &idxToNode,
    ShortestPathFew &shortestPathFew,
    const DWGraph::DWGraph &distGraph
){
    std::vector<std::vector<double>> distMatrix(K, std::vector<double>(K, fINF));

    for(size_t t = 0; t+1 < T; ++t){
        std::list<DWGraph::node_t> l;
        for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

        for(size_t i: candidateStates.at(t)){
            DWGraph::node_t u = idxToNode.at(i);

            shortestPathFew.initialize(&distGraph, u, l);
            shortestPathFew.run();
            for(size_t j: candidateStates.at(t+1)){
                DWGraph::node_t v = idxToNode.at(j);

                DWGraph::weight_t d = shortestPathFew.getPathWeight(v);
                double df = double(d)*MILLIMS_TO_METERS;
                distMatrix[i][j] = (d == iINF ? fINF : df);
            }
        }
    }

    return distMatrix;
}

void evalHMM_Viterbi(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-viterbi.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

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

    os << "i,Viterbi\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%10 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix = getDistMatrix(K, T, candidateStates, idxToNode, shortestPathFew, distGraph);

            // ======== VITERBI ========
            const double sigma_z = 4.07;
            const double beta = 3;

            HiddenMarkovModel::MyPi Pi(sigma_z, S, Y[0]);
            HiddenMarkovModel::MyA A(beta, Y, distMatrix);
            HiddenMarkovModel::MyB B(sigma_z, S, Y);

            begin = std::chrono::high_resolution_clock::now();

            Viterbi viterbi;
            viterbi.initialize(T, K, &Pi, &A, &B);
            viterbi.run();
            std::vector<long> v = viterbi.getLikeliestPath();

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

void evalHMM_ViterbiOptimized(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-viterbi-o.csv");
    os << std::fixed;

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    std::chrono::_V2::system_clock::time_point begin0, end0; double dt0;

    const size_t N = 10000;
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

    os << "i,Viterbi-o\n";

    begin0 = std::chrono::high_resolution_clock::now();

    for(size_t n = 0; n < N; ++n){
        try {
            const size_t idx = rand()%trips.size();
            const auto &trip = trips[idx].coords;
            const std::vector<Coord> &Y = trip;
            const size_t &T = Y.size();

            end0 = std::chrono::high_resolution_clock::now();
            dt0 = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end0-begin0).count());
            if(n%10 == 0)
                std::cout << "n=" << n << "/" << N << ", idx=" << idx
                          << ", tripId=" << trips[idx].id << ", failed=" << failed
                          << ", Total time: " << dt0/n * NANOS_TO_SECONDS * N
                          << ", ETA: " << dt0/n * NANOS_TO_SECONDS * (N - n) << "s"
                          << std::endl;

            // ======== CLOSEST POINTS/CANDIDATE STATES (VSTRIPES) ========
            std::map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            std::vector<Coord> S;
            std::vector<DWGraph::node_t> idxToNode;
            std::vector<std::set<long>> candidateStates(T);
            getCandidates(G, closestPointsInRadius, Y, S, Sv, idxToNode, candidateStates);
            const size_t &K = S.size();

            // ======== DISTANCE MATRIX ========
            std::vector<std::vector<double>> distMatrix = getDistMatrix(K, T, candidateStates, idxToNode, shortestPathFew, distGraph);

            // ======== VITERBI ========
            const double sigma_z = 4.07;
            const double beta = 3;

            HiddenMarkovModel::MyPi Pi(sigma_z, S, Y[0]);
            HiddenMarkovModel::MyA A(beta, Y, distMatrix);
            HiddenMarkovModel::MyB B(sigma_z, S, Y);

            begin = std::chrono::high_resolution_clock::now();

            ViterbiOptimized viterbi;
            viterbi.initialize(T, K, &Pi, &A, &B, candidateStates);
            viterbi.run();
            std::vector<long> v = viterbi.getLikeliestPath();

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
