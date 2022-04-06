#include "Viterbi.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

void Viterbi::initialize(long T_, long K_,
    InitialProbabilitiesGenerator *Pi_,
    TransitionMatrixGenerator *A_,
    EmissionMatrixGenerator *B_
){
    T  = T_;
    K  = K_;
    Pi = Pi_;
    A  = A_;
    B  = B_;

    DP   = VVF(T, VF(K,  0));
    prev = VVI(T, VI(K, -1));
}

void Viterbi::run(){
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
        for(long j = 0; j < K; ++j){
            for(long i = 0; i < K; ++i){
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
}

vector<long> Viterbi::getLikeliestPath() const {
    vector<long> ret;
    long j; {
        double pBest = 0.0;
        for(long j_ = 0; j_ < K; ++j_){
            const double &p = DP[T-1][j_];
            if(p > pBest){
                pBest = p;
                j = j_;
            }
        }
        if(pBest == 0.0) throw runtime_error("Could not find any path");
    }

    ret.push_back(j);
    for(long t = T-1; t > 0; --t){
        long i = prev[t][j];
        ret.push_back(i);
        j = i;
    }
    reverse(ret.begin(), ret.end());

    return ret;
}
