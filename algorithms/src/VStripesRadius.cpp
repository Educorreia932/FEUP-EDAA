#include "VStripesRadius.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

const double INF = 1000000000.0L;

void VStripesRadius::initialize(const list<Coord> &points, double width){
    stripes.clear();

    v.resize(points.size());
    copy(points.begin(), points.end(), v.begin());

    d = width;
}

void VStripesRadius::run(){
    sort(v.begin(), v.end(), Coord::compX);

    xMin = v.begin()->x;
    double xMax = v.rbegin()->x;

    double l = xMin, r = xMin+d;
    size_t i = 0;
    while(l <= xMax){
        size_t N = 0;
        for(size_t j = i; j < v.size() && v[j].x < r; ++j) ++N;
        stripes.push_back(vector<Coord>(N));

        vector<Coord> &stripe = *stripes.rbegin();
        for(size_t j = 0; i < v.size() && v[i].x < r; ++i, ++j){
            stripe[j] = v[i];
        }

        l = r;
        r += d*Coord::MetersToLonDegrees();
    }

    for(vector<Coord> &stripe: stripes)
        sort(stripe.begin(), stripe.end(), Coord::compY);
}

vector<Coord> VStripesRadius::getClosestPoints(Coord p) const {
    vector<Coord> sols;

    double fi = (p.x - xMin)/(d*Coord::MetersToLonDegrees());
    long li = (long)fi;
    size_t i = min(long(stripes.size()-1), max(long(0), li));

    checkStripe(p, i, sols);
    if(long(i-1) >= 0      ) checkStripe(p, i-1, sols);
    if(i+1 < stripes.size()) checkStripe(p, i+1, sols);

    return sols;
}

void VStripesRadius::checkStripe(const Coord &p, size_t i, vector<Coord> &sols) const {
    const vector<Coord> &stripe = stripes[i];
    auto l = lower_bound(stripe.begin(), stripe.end(), p.y-d*Coord::MetersToLatDegrees(), [](const Coord &c, const double &y){ return c.y < y; });
    for(; l != stripe.end() && l->y < p.y+d*Coord::MetersToLatDegrees(); ++l){
        const Coord &c = *l;
        double dist = Coord::getDistanceArcSimple(c, p);
        if(dist < d){
            sols.push_back(c);
        }
    }
}
