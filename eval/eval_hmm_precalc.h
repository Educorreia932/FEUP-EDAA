#pragma once

std::mutex hmm_precalc_mutex;
std::deque<std::list<Coord>> observations;
std::vector<std::set<DWGraph::node_t>> candidates;
VStripesRadius hmm_closestPointsInRadius;

void evalHMM_precalc_worker(size_t idx, const MapGraph &G, const size_t &NUM_PROBLEMS, const std::chrono::_V2::system_clock::time_point &begin){
    std::chrono::_V2::system_clock::time_point end; double dt;

    std::set<DWGraph::node_t> &cands = candidates.at(idx);
    while(true){
        hmm_precalc_mutex.lock();
        if(observations.size() <= 0){
            hmm_precalc_mutex.unlock();
            break;
        }
        end = std::chrono::high_resolution_clock::now();
        dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/1.0e9;
        double timePerIteration = dt/(NUM_PROBLEMS-observations.size());
        double totalTime = timePerIteration*NUM_PROBLEMS;
        double ETA = timePerIteration*observations.size();
        std::cout << "Thread " << idx << ", " << observations.size()
                  << ", Total time: " << totalTime
                  << ", ETA: " << ETA << std::endl;
        const std::list<Coord> obs = observations.front();
        observations.pop_front();
        hmm_precalc_mutex.unlock();

        for(const Coord &p: obs){
            std::vector<Coord> candsCoord = hmm_closestPointsInRadius.getClosestPoints(p);
            for(const Coord &c: candsCoord){
                cands.insert(G.coordToNode(c));
            }
        }
    }
}

void evalHMM_precalc(const MapGraph &M, const std::vector<Trip> &trips){
    std::ofstream os("eval/hmm-precalc.csv");
    os << std::fixed;
    os << "i,ShortestPathsPrecalc\n";

    const double d = 50;
    const size_t NUM_THREADS = 7;
    const size_t NUM_PROBLEMS = 256;

    MapGraph G = M.splitLongEdges(30.0);
    DWGraph::DWGraph distGraph = getSCC(G);

    auto nodes = distGraph.getNodes();
    std::list<Coord> l;
    for(const DWGraph::node_t &u: nodes) l.push_back(G.nodeToCoord(u));
    hmm_closestPointsInRadius.initialize(l, d);
    hmm_closestPointsInRadius.run();

    observations = std::deque<std::list<Coord>>(NUM_PROBLEMS);
    candidates = std::vector<std::set<DWGraph::node_t>>(NUM_THREADS);

    {
        size_t i = 0;
        for(const Trip &trip: trips){
            for(const Coord &c: trip.coords){
                observations.at((i++)%NUM_PROBLEMS).push_back(c);
            }
        }
    }

    std::chrono::_V2::system_clock::time_point begin, end; double dt;
    begin = std::chrono::high_resolution_clock::now();

    std::list<std::thread> threads;
    for(size_t i = 0; i < NUM_THREADS; ++i){
        threads.emplace_back(evalHMM_precalc_worker, i, G, NUM_PROBLEMS, begin);
    }
    for(std::thread &t: threads) t.join();
    std::set<DWGraph::node_t> candidatesSet;
    for(const std::set<DWGraph::node_t> &cands: candidates)
        candidatesSet.insert(cands.begin(), cands.end());

    std::cout << "There are a total of " << candidatesSet.size() << " candidates" << std::endl;
    
    begin = std::chrono::high_resolution_clock::now();
    std::vector<DijkstraDist> dijkstras(candidatesSet.size(), 650*METERS_TO_MILLIMS);
    {
        size_t i = 0;
        for(DWGraph::node_t s: candidatesSet){
            // if(i%10000 == 0) std::cout << "Done " << i << "/" << candidatesSet.size() << std::endl;
            dijkstras.at(i).initialize(&distGraph, s);
            dijkstras.at(i).run();
            ++i;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count());
    os << 0 << "," << dt << "\n";
}
