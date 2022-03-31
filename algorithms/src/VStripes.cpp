#include "VStripes.h"

#include <algorithm>

using namespace std;

typedef coord_t::deg_t deg_t;

const deg_t INF = 1000000000.0L;

VStripes::VStripes(deg_t width):
d(width){}

void VStripes::initialize(const list<coord_t> &points){
    v = vector<coord_t>();
    v.reserve(points.size());
    for(const coord_t &c: points) v.push_back(c);
}

void VStripes::run(){
    sort(v.begin(), v.end(), coord_t::compx);

    xMin = v.begin()->getLon();
    deg_t xMax = v.rbegin()->getLon();

    deg_t l = xMin, r = xMin+d;
    size_t i = 0;
    while(l <= xMax){
        size_t N = 0;
        for(size_t j = i; j < v.size() && v[j].getLon() < r; ++j) ++N;
        stripes.push_back(vector<coord_t>(N));

        vector<coord_t> &stripe = *stripes.rbegin();
        for(size_t j = 0; i < v.size() && v[i].getLon() < r; ++i, ++j){
            stripe[j] = v[i];
        }

        l = r;
        r += d;
    }

    for(vector<coord_t> &stripe: stripes)
        sort(stripe.begin(), stripe.end(), coord_t::compy);
}

coord_t VStripes::getClosestPoint(coord_t p) const {
    pair<bool,coord_t> ret = getClosestPoint_success(p);
    if(!ret.first)
        throw runtime_error("Could not find a solution");
    return ret.second;
}

pair<bool,coord_t> VStripes::getClosestPoint_success(coord_t p) const {
    double fi = (p.getLon() - xMin)/d;
    long li = (long)fi;
    size_t i = min(long(stripes.size()-1), max(long(0), li));
    
    coord_t cBest;
    deg_t dBest = INF;

    checkStripe(p, i, dBest, cBest);
    if(long(i-1) >= 0      ) checkStripe(p, i-1, dBest, cBest);
    if(i+1 < stripes.size()) checkStripe(p, i+1, dBest, cBest);

    return pair<bool,coord_t>(dBest <= d, cBest);
}

void VStripes::checkStripe(const coord_t &p, size_t i, coord_t::deg_t &dBest, coord_t &cBest) const {
    const vector<coord_t> &stripe = stripes[i];
    auto l = lower_bound(stripe.begin(), stripe.end(), p.getLat()-d, [](const coord_t &c, const double &y){ return c.getLat() < y; });
    auto r = lower_bound(stripe.begin(), stripe.end(), p.getLat()+d, [](const coord_t &c, const double &y){ return c.getLat() < y; });
    for(; l != r; ++l){
        const coord_t &c = *l;
        deg_t d = coord_t::getDistanceDeg(c, p);
        if(d < dBest){
            dBest = d;
            cBest = c;
        }
    }
}
