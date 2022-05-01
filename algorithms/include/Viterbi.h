#pragma once

#include <vector>
#include <set>

class Viterbi {
public:
    struct InitialProbabilitiesGenerator {
        virtual double operator()(long i) const = 0;
    };
    struct TransitionMatrixGenerator {
        virtual double operator()(long i, long j, long t) const = 0;
    };
    struct EmissionMatrixGenerator {
        virtual double operator()(long i, long t) const = 0;
    };
private:
    typedef std::vector<double> VF;
    typedef std::vector<VF> VVF;

    typedef std::vector<long> VI;
    typedef std::vector<VI> VVI;

    long T, K;
    VVF DP;   // DP  [t][i] = T1[i,t]
    VVI prev; // prev[t][i] = T2[i,t]
    const InitialProbabilitiesGenerator *Pi;
    const TransitionMatrixGenerator     *A;
    const EmissionMatrixGenerator       *B;
public:
    void initialize(long T_, long K_,
        InitialProbabilitiesGenerator *Pi_,
        TransitionMatrixGenerator *A_,
        EmissionMatrixGenerator *B_
    );
    void run();
    std::vector<long> getLikeliestPath() const;
};
