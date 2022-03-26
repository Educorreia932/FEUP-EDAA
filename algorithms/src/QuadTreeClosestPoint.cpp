#include "QuadTreeClosestPoint.h"

#include <algorithm>
#include <bitset>
#include "next_pow2.h"

using namespace std;

typedef coord_t::deg_t deg_t;

const deg_t INF = 1000000000.0L;

bool compx(const coord_t &lhs, const coord_t &rhs){
    return (lhs.getLon() < rhs.getLon());
}
bool compy(const coord_t &lhs, const coord_t &rhs){
    return (lhs.getLat() < rhs.getLat());
}

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
        size_t prefix = i & (~(1L << (size_bits(i) - 1)));
        size_t l = (prefix  ) << level;
        size_t r = (prefix+1) << level;
        bool xAxisActive = (level%2 == 1); // Active axis is X if level is odd
        if(xAxisActive) sort(c.begin() + l, c.begin() + r, compx);
        else            sort(c.begin() + l, c.begin() + r, compy);

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
    const size_t N = c.size();
    size_t level = size_bits(N) - size_bits(r);
    bool xAxisActive = (level%2 == 1);

    if(level <= 0){
        // If bin only has 1 element
        coord_t candidate = c[r & ~(1L << (size_bits(r)-1))];
        deg_t d = coord_t::getDistanceDeg(p, candidate);
        if(d < dbest){
            cbest = candidate;
            dbest = d;
        }
    } else {
        deg_t v = (xAxisActive ? p.getLon() : p.getLat());
        size_t i;
        if(v < split[r]) i = (r << 1);
        else             i = (r << 1) + 1;

        // Search in the child where p should be
        search(p, i, cbest, dbest);
        
        deg_t median = split[r];
        // coord_t median_coord = (xAxisActive ?
        //     coord_t(median, p.getLat()) :
        //     coord_t(p.getLon(), median)
        // );
        // If there's a chance the closest point is on the brother of the
        // current tree node
        if(abs(v - median) < dbest){
        // if(coord_t::getDistanceDeg(p, median_coord) < dbest){
            // The brother of current tree node i is i^1, because we just flip
            // the least significant bit
            search(p, i^1, cbest, dbest);
        }
    }
}

