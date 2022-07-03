#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <unordered_map>

#include "DeepVStripes.h"
#include "DeepVStripesFactory.h"
#include "DijkstraFew.h"
#include "DijkstraOnRequest.h"
#include "EdgeType.h"
#include "HiddenMarkovModel.h"
#include "Kosaraju.h"
#include "MapGraph.h"
#include "K2DTreeClosestPoint.h"
#include "Trip.h"
#include "VStripesRadius.h"

#include "utils.h"

using hrc = std::chrono::high_resolution_clock;

const double METERS_TO_MILLIMS = 1000.0;
const double MILLIMS_TO_METERS = 1.0 / 1000.0;
const double NANOS_TO_SECONDS = 1.0 / 1000000000.0;

#include "eval_2dtree.h"
#include "eval_deepvstripes.h"
#include "eval_hmm.h"
#include "eval_hmm_precalc.h"
#include "eval_error.h"
#include "eval_hierarchical.h"
#include "eval_kmeans.h"

int main(int argc, char* argv[]) {
    srand(1234);

    try {
        if (argc < 2) throw std::invalid_argument("at least one argument must be provided");
        std::string opt = argv[1];

        if (opt == "2d-tree-buildmem") { eval2DTree_BuildMemory(); return 0; }

        std::cout << "Loading map..." << std::endl;
        MapGraph M("res/map/processed/AMP");
        std::cout << "Loaded map" << std::endl;

        if (opt == "2d-tree-buildtime") { eval2DTree_BuildTime(M); return 0; }
        if (opt == "deepvstripes-buildtime") { evalDeepVStripes_BuildTime(M); return 0; }

        std::cout << "Loading trips..." << std::endl;
        std::vector<Trip> trips = Trip::loadTripsBin("res/data/pkdd15-i/pkdd15-i.trips.bin");
        std::cout << "Loaded trips" << std::endl;

        if (opt == "2d-tree-querytime") eval2DTree_QueryTime(M, trips);

        if (opt == "deepvstripes-querytime-d") evalDeepVStripes_QueryTime_d(M, trips);
        if (opt == "deepvstripes-querytime") evalDeepVStripes_QueryTime(M, trips);
        if (opt == "deepvstripes-querytime-nd") evalDeepVStripes_QueryTime_nd(M, trips);

        // HMM
        if (opt == "hmm-vstripes") evalHMM_VStripes(M, trips);

        if (opt == "hmm-dijkstra-s") evalHMM_Dijkstra_earlyStopping(M, trips);
        if (opt == "hmm-dijkstra-sd") evalHMM_Dijkstra_earlyStopping_dMax(M, trips);
        if (opt == "hmm-astar") evalHMM_Astar(M, trips);
        if (opt == "hmm-astar-d") evalHMM_Astar_dMax(M, trips);
        if (opt == "hmm-astarfew") evalHMM_AstarFew(M, trips);
        if (opt == "hmm-astarfew-d") evalHMM_AstarFew_dMax(M, trips);

        if (opt == "hmm-viterbi") evalHMM_Viterbi(M, trips);
        if (opt == "hmm-viterbi-o") evalHMM_ViterbiOptimized(M, trips);

        if (opt == "hmm-dijkstra-cache") evalHMM_DijkstraCache(M, trips);

        if (opt == "hmm-precalc") evalHMM_precalc(M, trips);

        if (opt == "error-pointwise-nn") evalErrorPointwise_nn(M, trips);
        if (opt == "error-pointwise-hmm") evalErrorPointwise_hmm(M, trips);

        // Clustering
        if (opt == "hierarchical")
            evalHierarchical(M);
            
        if (opt == "kmeans")
            evalKMeans(M);

        if (opt == "elbow")
            elbowMethod(M);
    }

    catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
