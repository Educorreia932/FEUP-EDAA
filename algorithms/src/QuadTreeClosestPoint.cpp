#include "QuadTreeClosestPoint.h"

#include <algorithm>
#include <bitset>
#include "next_pow2.h"

using namespace std;

typedef coord_t::deg_t deg_t;

const deg_t INF = 1000000000.0L;

size_t size_bits(size_t n){
    return 32 - __builtin_clz(uint32_t(n));
}

QuadTreeClosestPoint::QuadTreeClosestPoint(){}

void QuadTreeClosestPoint::initialize(const list<coord_t> &points){
    size_t N = next_pow2(points.size());
    c.resize(N);
    split.resize(N);
    copy(points.begin(), points.end(), c.begin());
    sort(c.begin(), c.begin() + points.size());
    fill(c.begin() + points.size(), c.end(), *(c.begin() + points.size() - 1));
}

void QuadTreeClosestPoint::run(){
    const size_t N = c.size();
    for(size_t i = 1; i < N; ++i){
        size_t level = size_bits(N) - size_bits(i);
        size_t prefix = i & (~(1uL << (size_bits(i) - 1)));
        size_t l = (prefix  ) << level;
        size_t r = (prefix+1) << level;
        bool xAxisActive = (level%2 == 1); // Active axis is X if level is odd
        if(xAxisActive) stable_sort(c.begin() + l, c.begin() + r, coord_t::compx);
        else            stable_sort(c.begin() + l, c.begin() + r, coord_t::compy);

        size_t m1 = l + (r-l)/2 - 1;
        size_t m2 = l + (r-l)/2;
        coord_t medianCoord = (c[m1] + c[m2])/2;
        deg_t median = (xAxisActive ? medianCoord.getLon() : medianCoord.getLat());
        split[i] = median;
    }
}

coord_t QuadTreeClosestPoint::getClosestPoint(const coord_t p) const {
    coord_t cbest;
    deg_t dbest = INF;
    search(p, 1, cbest, dbest);
    return cbest;
}

void QuadTreeClosestPoint::search(const coord_t &p, size_t r, coord_t &cbest, deg_t &dbest) const {
    const size_t &N = c.size();
    size_t level = size_bits(N) - size_bits(r);
    bool xAxisActive = (level%2 == 1);

    if(level <= 0){
        // If bin only has 1 element
        const coord_t &candidate = c[r & ~(1uL << (size_bits(r)-1))];
        deg_t d = coord_t::getDistanceDegSqr(p, candidate);
        if(d < dbest){
            cbest = candidate;
            dbest = d;
        }
    } else {
        const deg_t &median = split[r];
        deg_t v = (xAxisActive ? p.getLon() : p.getLat());
        size_t i = (r << 1) + (v < median ? 0 : 1);

        // Search in the child where p should be
        search(p, i, cbest, dbest);
        
        coord_t median_coord = (xAxisActive ?
            coord_t(p.getLat(), median) :
            coord_t(median, p.getLon())
        );
        // If there's a chance the closest point is on the brother of the
        // current tree node
        if(coord_t::getDistanceDegSqr(p, median_coord) < dbest){
        // if(abs(v-median) < dbest){
            // The brother of current tree node i is i^1, because we just flip
            // the least significant bit
            search(p, i^1, cbest, dbest);
        }
    }
}

