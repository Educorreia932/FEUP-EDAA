#include "DeepVStripes.h"

using namespace std;

typedef coord_t::deg_t deg_t;

DeepVStripes::DeepVStripes(deg_t d, size_t n){
    vstripes_vtr.clear();
    for(size_t i = 0; i < n; ++i, d *= 2.0){
        vstripes_vtr.push_back(VStripes(d));
    }
}

void DeepVStripes::initialize(const list<coord_t> &points){
    for(VStripes &vstripes: vstripes_vtr)
        vstripes.initialize(points);
}

void DeepVStripes::run(){
    for(VStripes &vstripes: vstripes_vtr)
        vstripes.run();
}

coord_t DeepVStripes::getClosestPoint(coord_t p) const {
    pair<bool, coord_t> ret;
    size_t i = 0;
    do {
        ret = vstripes_vtr[i++].getClosestPoint_success(p);
    } while(i < vstripes_vtr.size() && !ret.first);
    if(!ret.first){
        throw runtime_error("Could not find a solution");
    }
    return ret.second;
}
