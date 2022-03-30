#include "Run.h"

#include <fstream>

using namespace std;

vector<Run> Run::loadRuns(const string &filepath){
    vector<Run> ret;

    ifstream is; is.exceptions(ifstream::failbit | ifstream::badbit);
    is.open(filepath);

    size_t N; is >> N;
    ret.reserve(N);
    for(size_t i = 0; i < N; ++i){
        ret.push_back(Run());
        Run &r = *ret.rbegin();
        size_t M;
        is >> r.id >> r.timestamp >> M;
        r.coords.reserve(M);
        coord_t c;
        for(size_t j = 0; j < M; ++j){
            is >> c.lat >> c.lon;
            r.coords.push_back(c);
        }
    }

    return ret;
}
