#pragma once

void evalErrorPointwise_nn(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/error-pointwise-nn.csv");
    os << std::fixed;
    os << "i,delta_match_obs,dobs,dmatch\n";

    MapGraph G = M.splitLongEdges(30.0);

    std::cerr << "Computing map matching..." << std::endl;
    DeepVStripesFactory deepVStripesFactory(0.0003, 12);
    MapMatching::FromClosestPoint mapMatching(deepVStripesFactory);
    mapMatching.initialize(&G);
    mapMatching.run();

    DWGraph::DWGraph distGraph = G.getDistanceGraph();

    size_t N = 10000;

    size_t id = 0;

    for(size_t i = 0; i < N; ++i){
        if(i%100 == 0) std::cerr << "i=" << i << "/" << N << std::endl;
        size_t idx = rand()%trips.size();
        const Trip &trip = trips.at(idx);

        try {
            std::vector<DWGraph::node_t> matches = mapMatching.getMatches(trip.coords);

            Coord prevObs;
            DWGraph::node_t prevMatchId; Coord prevMatch;
            for(size_t j = 0; j < matches.size(); ++j){
                const Coord &obs = trip.coords[j];
                const Coord &match = G.nodeToCoord(matches.at(j));

                os  << id++ << "," << Coord::getDistanceArc(obs, match) << ",";
                
                if(j > 0) os << Coord::getDistanceArc(obs, prevObs);
                os << ",";

                if(j > 0){
                    const DWGraph::node_t
                        &u = prevMatchId,
                        &v = matches.at(j);

                    MapGraph::DistanceHeuristic h(G.getNodes(), G.nodeToCoord(v), METERS_TO_MILLIMS);
                    Astar astar(&h);
                    astar.initialize(&distGraph, u, v);
                    astar.run();
                    double d = double(astar.getPathWeight())*MILLIMS_TO_METERS;
                    os << d;
                }
                os << ",";
                os << "\n";

                prevObs = obs;
                prevMatchId = matches.at(j);
                prevMatch = match;
            }
        } catch(const std::exception &e){
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}


void evalErrorPointwise_hmm(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/error-pointwise-hmm.csv");
    os << std::fixed;
    os << "i,delta_match_obs,dobs,dmatch\n";

    MapGraph G = M.splitLongEdges(30.0);

    std::cerr << "Computing map matching..." << std::endl;
    double d = 50; // in meters
    double sigma_z = 5.925232; // in meters
    double beta = 6.677601; // From https://www.mapzen.com/blog/data-driven-map-matching/
    VStripesRadius closestPointsInRadius;
    DijkstraOnRequest shortestPathFew;
    ShortestPathFew::FromAll shortestPathAll(shortestPathFew);
    HiddenMarkovModel mapMatching(closestPointsInRadius, shortestPathAll, d, sigma_z, beta);
    mapMatching.initialize(&G);
    mapMatching.run();

    DWGraph::DWGraph distGraph = G.getDistanceGraph();

    size_t N = 10000;

    size_t id = 0;

    for(size_t i = 0; i < N; ++i){
        if(i%100 == 0) std::cerr << "i=" << i << "/" << N << std::endl;
        size_t idx = rand()%trips.size();
        const Trip &trip = trips.at(idx);

        try {
            std::vector<DWGraph::node_t> matches = mapMatching.getMatches(trip.coords);

            Coord prevObs;
            DWGraph::node_t prevMatchId; Coord prevMatch;
            for(size_t j = 0; j < matches.size(); ++j){
                const Coord &obs = trip.coords[j];
                const Coord &match = G.nodeToCoord(matches.at(j));

                os  << id++ << "," << Coord::getDistanceArc(obs, match) << ",";
                
                if(j > 0) os << Coord::getDistanceArc(obs, prevObs);
                os << ",";

                if(j > 0){
                    const DWGraph::node_t
                        &u = prevMatchId,
                        &v = matches.at(j);

                    MapGraph::DistanceHeuristic h(G.getNodes(), G.nodeToCoord(v), METERS_TO_MILLIMS);
                    Astar astar(&h);
                    astar.initialize(&distGraph, u, v);
                    astar.run();
                    double d = double(astar.getPathWeight())*MILLIMS_TO_METERS;
                    os << d;
                }
                os << ",";
                os << "\n";

                prevObs = obs;
                prevMatchId = matches.at(j);
                prevMatch = match;
            }
        } catch(const std::exception &e){
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}
