#include "ViterbiOptimized.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include <sstream>

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

void ViterbiOptimized::initialize(long T_, long K_,
    Viterbi::InitialProbabilitiesGenerator *Pi_,
    Viterbi::TransitionMatrixGenerator *A_,
    Viterbi::EmissionMatrixGenerator *B_,
    vector<set<long>> candidates_
){
    T  = T_;
    K  = K_;
    Pi = Pi_;
    A  = A_;
    B  = B_;
    candidates = candidates_;

    DP   = VVF(T, VF(K,  0));
    prev = VVI(T, VI(K, -1));
}

void ViterbiOptimized::run(){
    // Initialize with Pi
    double maxProb = 0.0;
    for(long i = 0; i < K; ++i){
        double p = (*Pi)(i);
        DP[0][i] = p;
        maxProb = max(maxProb, p);
    }
    if(maxProb > 0.0) for(double &p: DP[0]) p /= maxProb;

    // Run stuff
    for(long t = 1; t < T; ++t){
        maxProb = 0.0;
        for(long i: candidates.at(t-1)){
            if(DP[t-1][i] <= 0.0) continue;
            for(long j: candidates.at(t)){
                double prob = DP[t-1][i] * (*A)(i,j,t) * (*B)(j,t);
                if(prob > DP[t][j]){
                    DP  [t][j] = prob;
                    prev[t][j] = i;
                    maxProb = max(maxProb, prob);
                }
            }
        }
        if(maxProb > 0.0) for(double &p: DP[t]) p /= maxProb;
    }

    // cout << setprecision(1);
    // cout << "\n\t";
    // for(long t = 0; t < T; ++t) cout << t << "\t";
    // for(long i = 0; i < K; ++i){
    //     cout << i << "\t";
    //     for(long t = 0; t < T; ++t){
    //         cout << DP[t][i] << "\t";
    //     }
    //     cout << endl;
    // }
}

vector<long> ViterbiOptimized::getLikeliestPath() const {
    vector<long> ret;
    long t, j; {
        double pBest = 0.0;
        for(t = T-1; t >= 0; --t){
            for(long j_ = 0; j_ < K; ++j_){
                const double &p = DP[t][j_];
                if(p > pBest){
                    pBest = p;
                    j = j_;
                }
            }
            if(pBest != 0.0) break;
        }
    }

    if(t != T-1){
        stringstream ss;
        ss << "Could not find any path, t=" << t;
        throw runtime_error(ss.str());
    }

    ret.push_back(j);
    for(; t > 0; --t){
        long i = prev.at(t).at(j);
        ret.push_back(i);
        j = i;
    }
    reverse(ret.begin(), ret.end());

    return ret;
}
