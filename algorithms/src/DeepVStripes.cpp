#include "DeepVStripes.h"

#include <stdexcept>

using namespace std;

DeepVStripes::DeepVStripes(double d, size_t n){
    vstripes_vtr.clear();
    for(size_t i = 0; i < n; ++i, d *= 2.0){
        vstripes_vtr.push_back(VStripes(d));
    }
}

void DeepVStripes::initialize(const list<Vector2> &points){
    for(VStripes &vstripes: vstripes_vtr)
        vstripes.initialize(points);
}

void DeepVStripes::run(){
    for(VStripes &vstripes: vstripes_vtr)
        vstripes.run();
}

Vector2 DeepVStripes::getClosestPoint(Vector2 p) const {
    pair<bool, Vector2> ret;
    size_t i = 0;
    do {
        ret = vstripes_vtr[i++].getClosestPoint_success(p);
    } while(i < vstripes_vtr.size() && !ret.first);
    if(!ret.first){
        throw runtime_error("Could not find a solution");
    }
    return ret.second;
}
