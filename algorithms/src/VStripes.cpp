#include "VStripes.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

const double INF = 1000000000.0L;

VStripes::VStripes(double width):
d(width){}

void VStripes::initialize(const list<Vector2> &points){
    stripes.clear();

    v.resize(points.size());
    copy(points.begin(), points.end(), v.begin());
}

void VStripes::run(){
    sort(v.begin(), v.end(), Vector2::compX);

    xMin = v.begin()->x;
    double xMax = v.rbegin()->x;

    double l = xMin, r = xMin+d;
    size_t i = 0;
    while(l <= xMax){
        size_t N = 0;
        for(size_t j = i; j < v.size() && v[j].x < r; ++j) ++N;
        stripes.push_back(vector<Vector2>(N));

        vector<Vector2> &stripe = *stripes.rbegin();
        for(size_t j = 0; i < v.size() && v[i].x < r; ++i, ++j){
            stripe[j] = v[i];
        }

        l = r;
        r += d;
    }

    for(vector<Vector2> &stripe: stripes)
        sort(stripe.begin(), stripe.end(), Vector2::compY);
}

Vector2 VStripes::getClosestPoint(Vector2 p) const {
    pair<bool,Vector2> ret = getClosestPoint_success(p);
    if(!ret.first)
        throw runtime_error("Could not find a solution");
    return ret.second;
}

pair<bool,Vector2> VStripes::getClosestPoint_success(Vector2 p) const {
    double fi = (p.x - xMin)/d;
    long li = (long)fi;
    size_t i = min(long(stripes.size()-1), max(long(0), li));
    
    Vector2 cBest;
    double dBest = INF;

    checkStripe(p, i, dBest, cBest);
    if(long(i-1) >= 0      ) checkStripe(p, i-1, dBest, cBest);
    if(i+1 < stripes.size()) checkStripe(p, i+1, dBest, cBest);

    return pair<bool,Vector2>(dBest <= d, cBest);
}

void VStripes::checkStripe(const Vector2 &p, size_t i, double &dBest, Vector2 &cBest) const {
    const vector<Vector2> &stripe = stripes[i];
    auto l = lower_bound(stripe.begin(), stripe.end(), p.y-d, [](const Vector2 &c, const double &y){ return c.y < y; });
    for(; l != stripe.end() && l->y < p.y+d; ++l){
        const Vector2 &c = *l;
        double d = c.getDistance(p);
        if(d < dBest){
            dBest = d;
            cBest = c;
        }
    }
}
