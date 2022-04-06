#pragma once

#include <vector>

class Viterbi {
public:
    class TransitionMatrixGenerator {
    public:
        virtual double operator()(long i, long j, long t) = 0;
    };
    class EmissionMatrixGenerator {
        virtual double operator()(long i, long t) = 0;
    };
private:
    typedef std::vector<double> VF;
    typedef std::vector<VF> VVF;
    typedef std::vector<VVF> VVVF;

    typedef std::vector<long> VI;
    typedef std::vector<VI> VVI;

    long T, K;
    VVF DP; // DP  [t][i] = T1[i,t]
    VVI prev; // prev[t][i] = T2[i,t]
    const VF   *Pi;
    const VVVF *A;
    const VVF  *B;
public:
    void initialize(long T_, long K_, const VF *Pi_, const VVVF *A_, const VVF *B_);
    void run();
    std::vector<long> getLikeliestPath() const;
};
