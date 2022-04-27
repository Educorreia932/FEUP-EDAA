#include "HiddenMarkovModelMany.h"

#include "HiddenMarkovModel.h"
#include "Kosaraju.h"

using namespace std;
using namespace utils;

typedef vector<double> VF;
typedef vector<VF> VVF;

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
    threadPool(nThreads)
{
}

HiddenMarkovModelMany::~HiddenMarkovModelMany(){
    while(!tripTasks.empty()){
        delete tripTasks.begin()->second;
        tripTasks.erase(tripTasks.begin());
    }
    
}

void HiddenMarkovModelMany::initialize(
    const MapGraph *mapGraph_,
    const vector<Trip> &trips_
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
    set<DWGraph::node_t> &cands = hmm->candidates.at(idx);
    while(true){
        hmm->candidatesMutex.lock();
        if(hmm->observations.size() <= 0){
            hmm->candidatesMutex.unlock();
            break;
        }

        cout << "Thread " << idx << ", " << hmm->observations.size() << endl;

        list<Coord> *obs = hmm->observations.front();
        hmm->observations.pop_front();
        hmm->candidatesMutex.unlock();

        for(const Coord &p: *obs){
            vector<Coord> candsCoord = hmm->closestPointsInRadius.getClosestPoints(p);
            for(const Coord &c: candsCoord){
                cands.insert(hmm->mapGraph->coordToNode(c));
            }
        }

        delete obs;
    }
}

class DijkstraDistTask: public ThreadPool::Task {
private:
    DijkstraDist &dijkstraDist;
public:
    DijkstraDistTask(DijkstraDist &dd):dijkstraDist(dd){}
    virtual void run(){ dijkstraDist.run(); }
};

HiddenMarkovModelMany::TripTask::TripTask(
    const HiddenMarkovModelMany &hmm_,
    const Trip &trip_,
    size_t index_
):hmm(hmm_), trip(trip_), index(index_){}
HiddenMarkovModelMany::TripTask::~TripTask(){}
void HiddenMarkovModelMany::TripTask::run(){
    if(index % 1000 == 0) cout << "Index: " << index << endl;
    try {
        const vector<Coord> &Y = trip.coords;
        const size_t &T = Y.size();

        map<Coord, long, bool (*)(const Vector2&, const Vector2&)> Sv(Vector2::compXY);
        vector<Coord> S;
        vector<node_t> idxToNode;
        vector<set<long>> candidateStates(T);
        for(size_t t = 0; t < T; ++t){
            vector<Coord> v = hmm.closestPointsInRadius.getClosestPoints(Y.at(t));
            if(v.empty()) throw runtime_error("Location t=" + to_string(t) + " has no candidates");
            for(const Coord &c: v){
                if(!Sv.count(c)){
                    long id = Sv.size();
                    Sv[c] = id;
                    S.push_back(c);
                    idxToNode.push_back(hmm.mapGraph->coordToNode(c));
                }
                candidateStates.at(t).insert(Sv.at(c));
            }
        }
        const size_t &K = Sv.size();

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
                    DWGraph::weight_t d = hmm.dijkstras.at(u).getPathWeight(v);
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
        HiddenMarkovModel::MyPi Pi(hmm.sigma_z, S, Y[0]);
        HiddenMarkovModel::MyA A(hmm.beta, Y, distMatrix);
        HiddenMarkovModel::MyB B(hmm.sigma_z, S, Y);

        ViterbiOptimized viterbi;
        viterbi.initialize(T, K, &Pi, &A, &B, candidateStates);
        viterbi.run();

        vector<long> v = viterbi.getLikeliestPath();

        // Final processing
        assert(v.size() == Y.size());
        
        matches = vector<node_t>(v.size());
        for(size_t i = 0; i < v.size(); ++i){
            matches[i] = idxToNode[v[i]];
        }
        success = true;
    } catch(const exception &e){
        success = false;
    }
}
const Trip &HiddenMarkovModelMany::TripTask::getTrip() const { return trip; }
bool HiddenMarkovModelMany::TripTask::succeeded() const { return success; }
const vector<node_t> &HiddenMarkovModelMany::TripTask::getMatches() const { return matches; }

void HiddenMarkovModelMany::run(){
    cout << "Total number of trips: " << trips->size() << endl;

    hrc::time_point begin, end; double dt;

    // Run closest points
    closestPointsInRadius.run();

    // Run shortest paths
    cout << "Calculating paths..." << endl;
    begin = hrc::now();

    dijkstras.clear();
    list<DijkstraDistTask> dijkstraTasks;
    for(const DWGraph::node_t &s: distGraph.getNodes()){
        dijkstras.emplace(s, 650*METERS_TO_MILLIMS);
        dijkstras.at(s).initialize(&distGraph, s);
        dijkstraTasks.emplace_back(dijkstras.at(s));
    }

    cout << "Calculating paths in parallel..." << endl;
    
    for(ThreadPool::Task &task: dijkstraTasks) threadPool.submit(&task);
    for(ThreadPool::Task &task: dijkstraTasks) task.wait();
    dijkstraTasks.clear();

    end = hrc::now();
    dt = double(chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << "Calculated paths, took " << dt << "s" << endl;

    // Process all trips
    cout << "Matching trips..." << endl;
    begin = hrc::now();

    tripTasks.clear();
    size_t index = 0;
    for(const Trip &trip: *trips) tripTasks[trip.id] = new TripTask(*this, trip, index++);

    cout << "Matching trips in parallel..." << endl;

    for(auto &p: tripTasks) threadPool.submit(p.second);
    for(auto &p: tripTasks) p.second->wait();
    for(auto it = tripTasks.begin(); it != tripTasks.end();){
        if(it->second->succeeded()) ++it;
        else it = tripTasks.erase(it);
    }

    end = hrc::now();
    dt = double(chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    cout << "Matched trips, took " << dt << "s" << endl;
}

const vector<node_t> &HiddenMarkovModelMany::getMatches(long long tripId) const{
    return tripTasks.at(tripId)->getMatches();
}
