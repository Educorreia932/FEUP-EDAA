#include "HiddenMarkovModelMany.h"

#include "DijkstraDist.h"
#include "HiddenMarkovModel.h"
#include "Kosaraju.h"

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;

typedef DWGraph::node_t node_t;

using hrc = std::chrono::high_resolution_clock;

const double NANOS_TO_SECS = (1.0/1000000000.0);
const double MILLIMS_TO_METERS = (1.0/1000.0);
const double METERS_TO_MILLIMS = 1000.0;

HiddenMarkovModelMany::HiddenMarkovModelMany(
    ClosestPointsInRadius &closestPointsInRadius_,
    double d_, double sigma_z_, double beta_,
    size_t nThreads
):
    closestPointsInRadius(closestPointsInRadius_),
    d(d_), sigma_z(sigma_z_), beta(beta_),
    NUM_THREADS(nThreads)
{
}

void HiddenMarkovModelMany::initialize(
    const MapGraph *mapGraph_,
    const std::vector<Trip> &trips_
){
    mapGraph = mapGraph_;
    trips = &trips_;

    // Get distGraph
    distGraph = mapGraph->getDistanceGraph();
    DUGraph duDistGraph = (DUGraph)distGraph;
    Kosaraju kosaraju;
    kosaraju.initialize(&duDistGraph);
    kosaraju.run();
    node_t root = kosaraju.get_scc(4523960191);
    for(const node_t &u: duDistGraph.getNodes()){
        if(kosaraju.get_scc(u) != root){
            distGraph.removeNode(u);
        }
    }

    auto nodes = distGraph.getNodes();
    list<Coord> l;
    for(const node_t &u: nodes) l.push_back(mapGraph->nodeToCoord(u));
    cout << "Initializing closest points with " << l.size() << " points out of " << mapGraph->getNodes().size() << endl;
    closestPointsInRadius.initialize(l, d);
}

void HiddenMarkovModelMany::candidatesWorker(HiddenMarkovModelMany *hmm, size_t idx){
    std::set<DWGraph::node_t> &cands = hmm->candidates.at(idx);
    while(true){
        hmm->candidatesMutex.lock();
        if(hmm->observations.size() <= 0){
            hmm->candidatesMutex.unlock();
            break;
        }

        std::cout << "Thread " << idx << ", " << hmm->observations.size() << std::endl;

        std::list<Coord> *obs = hmm->observations.front();
        hmm->observations.pop_front();
        hmm->candidatesMutex.unlock();

        for(const Coord &p: *obs){
            std::vector<Coord> candsCoord = hmm->closestPointsInRadius.getClosestPoints(p);
            for(const Coord &c: candsCoord){
                cands.insert(hmm->mapGraph->coordToNode(c));
            }
        }

        delete obs;
    }
}

void HiddenMarkovModelMany::run(){
    hrc::time_point begin, end; double dt;

    // Run closest points
    closestPointsInRadius.run();

    // Run shortest paths
    std::cout << "Calculating paths..." << std::endl;
    begin = hrc::now();
    std::map<node_t, DijkstraDist> dijkstras;
    for(DWGraph::node_t s: distGraph.getNodes()){
        dijkstras.emplace(s, 650*METERS_TO_MILLIMS);
        dijkstras[s].initialize(&distGraph, s);
        dijkstras[s].run();
    }
    end = hrc::now();
    dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    std::cout << "Calculated paths, took " << dt << "s" << std::endl;

    size_t total = 0, failed = 0;

    for(const Trip &trip: *trips){
        ++total;
        try {
            vector<Coord> Y = trip.coords;
            const size_t &T = Y.size();

            begin = hrc::now();
            map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
            vector<Coord> S;
            vector<node_t> idxToNode;
            vector<set<long>> candidateStates(T);
            for(size_t t = 0; t < T; ++t){
                vector<Coord> v = closestPointsInRadius.getClosestPoints(Y.at(t));
                if(v.empty()) throw std::runtime_error("Location t=" + to_string(t) + " has no candidates");
                for(const Coord &c: v){
                    if(!Sv.count(c)){
                        long id = Sv.size();
                        Sv[c] = id;
                        S.push_back(c);
                        idxToNode.push_back(mapGraph->coordToNode(c));
                    }
                    candidateStates.at(t).insert(Sv.at(c));
                }
            }
            const size_t &K = Sv.size();

            end = hrc::now();
            dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;

            // ======== DISTANCE MATRIX (A*) ========
            VVF distMatrix(K, VF(K, fINF));
            for(size_t t = 0; t+1 < T; ++t){
                list<node_t> l;
                for(size_t j: candidateStates.at(t+1)) l.push_back(idxToNode.at(j));

                for(size_t i: candidateStates.at(t)){
                    for(size_t j: candidateStates.at(t+1)){
                        const node_t
                            &u = idxToNode.at(i),
                            &v = idxToNode.at(j);
                        DWGraph::weight_t d = dijkstras.at(u).getPathWeight(v);
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
                        dBest = min(dBest, distMatrix[i][j]);
                    }

                    if(dBest >= fINF) it = candidateStates.at(t+1).erase(it);
                    else ++it;
                }
            }

            // ======== HIDDEN MARKOV MODEL (VITERBI) ========
            HiddenMarkovModel::MyPi Pi(sigma_z, S, Y[0]);
            HiddenMarkovModel::MyA A(beta, Y, distMatrix);
            HiddenMarkovModel::MyB B(sigma_z, S, Y);

            begin = hrc::now();
            ViterbiOptimized viterbi;
            viterbi.initialize(T, K, &Pi, &A, &B, candidateStates);
            viterbi.run();

            vector<long> v = viterbi.getLikeliestPath();
            end = hrc::now();
            dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;

            // Final processing
            assert(v.size() == Y.size());
            
            vector<node_t> matches(v.size());
            for(size_t i = 0; i < v.size(); ++i){
                matches[i] = idxToNode[v[i]];
            }

            matchesMap[trip.id] = matches;

        } catch(const std::exception &e){
            cerr << "total=" << total << ", " << trip.id << ", Exception: " << e.what() << endl;
            failed++;
        }
    }
}

std::vector<DWGraph::node_t> HiddenMarkovModelMany::getMatches(long long tripId) const{

}
