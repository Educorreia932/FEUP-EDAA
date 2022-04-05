#include "Viterbi.h"

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
