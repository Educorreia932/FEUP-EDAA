#include "Viterbi.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

// #include <iostream>
// #include <iomanip>

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
                if(DP[t-1][i] <= 0.0) continue;
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
        if(pBest == 0.0){
            long t;
            for(t = T-1; t >= 0; --t){
                for(j = 0; j < K && DP[t][j] == 0.0; ++j){}
                if(j < K && DP[t][j] != 0.0) break;
            }
            stringstream ss;
            ss << "Could not find any path, t=" << t;
            throw runtime_error(ss.str());
        }
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
