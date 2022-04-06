#include "Viterbi.h"

#include <cassert>
#include <stdexcept>

using namespace std;

typedef std::vector<double> VF;
typedef std::vector<VF> VVF;
typedef std::vector<VVF> VVVF;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

void Viterbi::initialize(long T_, long K_, const VF *Pi_, const VVVF *A_, const VVF *B_){
    T  = T_;
    K  = K_;
    Pi = Pi_;
    A  = A_;
    B  = B_;

    assert((*Pi)      .size() == K);
    assert((*A )      .size() == K);
    assert((*A )[0]   .size() == K);
    assert((*A )[0][0].size() == T);
    assert((*B )      .size() == K);
    assert((*B )      .size() == T-1);

    DP   = VVF(T, VF(K,  0));
    prev = VVI(T, VI(K, -1));
}

void Viterbi::run(){
    // Initialize with Pi
    for(long i = 0; i < K; ++i) DP[0][i] = (*Pi)[i];

    // Run stuff
    for(long t = 1; t < T; ++t){
        for(long j = 0; j < K; ++j){
            for(long i = 0; i < K; ++i){
                double prob = DP[t-1][i] * (*A)[i][j][t] * (*B)[j][t];
                if(prob > DP[t][j]){
                    DP  [t][j] = prob;
                    prev[t][j] = i;
                }
            }
        }
    }
}

vector<long> Viterbi::getLikeliestPath() const {
    vector<long> ret;
    long j; {
        double pBest = 0.0;
        for(size_t j_ = 0; j_ < K; ++j_){
            const double &p = DP[T-1][j_];
            if(p > pBest){
                pBest = p;
                j = j_;
            }
        }
        if(pBest == 0.0) throw runtime_error("Could not find any path");
    }

    ret.push_back(j);
    for(size_t t = T-1; t > 0; --t){
        long i = prev[t][j];
        ret.push_back(i);
        j = i;
    }

    return ret;
}
