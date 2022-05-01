#include "polygon.h"

#include <fstream>

using namespace std;

vector<polygon_t> polygon_t::loadPolygons(const string &filePath){
    ifstream is(filePath);
    size_t N; is >> N;
    vector<polygon_t> ret(N);
    for(size_t i = 0; i < N; ++i){
        polygon_t &polygon = ret[i];
        size_t M; char c;
        is >> polygon.id >> c >> M;
        polygon.t = static_cast<polygon_t::type>(c);
        polygon.coords.resize(M);
        for(size_t j = 0; j < M; ++j){
            is >> polygon.coords[j].lat() >> polygon.coords[j].lon();
        }
    }
    return ret;
}
