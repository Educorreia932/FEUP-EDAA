#pragma once

#include "Viterbi.h"

#include <vector>
#include <set>

class ViterbiOptimized {
private:
    typedef std::vector<double> VF;
    typedef std::vector<VF> VVF;

    typedef std::vector<long> VI;
    typedef std::vector<VI> VVI;

    long T, K;
    VVF DP;   // DP  [t][i] = T1[i,t]
    VVI prev; // prev[t][i] = T2[i,t]
    const Viterbi::InitialProbabilitiesGenerator *Pi;
    const Viterbi::TransitionMatrixGenerator     *A;
    const Viterbi::EmissionMatrixGenerator       *B;
    std::vector<std::set<long>> candidates;
public:
    void initialize(long T_, long K_,
        Viterbi::InitialProbabilitiesGenerator *Pi_,
        Viterbi::TransitionMatrixGenerator *A_,
        Viterbi::EmissionMatrixGenerator *B_,
        std::vector<std::set<long>> candidates_
    );
    void run();
    std::vector<long> getLikeliestPath() const;
};
