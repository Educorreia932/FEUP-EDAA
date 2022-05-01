#include "K2DTreeClosestPoint.h"

#include <algorithm>
#include <bitset>
#include "utils.h"

using namespace std;

size_t size_bits(size_t n){
    return 32 - __builtin_clz(uint32_t(n));
}

K2DTreeClosestPoint::K2DTreeClosestPoint(){}

void K2DTreeClosestPoint::initialize(const list<Vector2> &points){
    size_t N = utils::nextPow2(points.size());
    c.resize(N);
    split.resize(N);
    copy(points.begin(), points.end(), c.begin());
    sort(c.begin(), c.begin() + points.size(), Vector2::compX);
    fill(c.begin() + points.size(), c.end(), *(c.begin() + points.size() - 1));
}

void K2DTreeClosestPoint::run(){
    const size_t N = c.size();
    for(size_t i = 1; i < N; ++i){
        size_t level = size_bits(N) - size_bits(i);
        size_t prefix = i & (~(1uL << (size_bits(i) - 1)));
        size_t l = (prefix  ) << level;
        size_t r = (prefix+1) << level;
        size_t m = l + (r-l)/2;
        bool xAxisActive = (level%2 == 1); // Active axis is X if level is odd
        if(xAxisActive) nth_element(c.begin() + l, c.begin() + m, c.begin() + r, Vector2::compX);
        else            nth_element(c.begin() + l, c.begin() + m, c.begin() + r, Vector2::compY);

        const Vector2 &medianCoord = c[m];
        const double &median = (xAxisActive ? medianCoord.x : medianCoord.y);
        split[i] = median;
    }
}

Vector2 K2DTreeClosestPoint::getClosestPoint(const Vector2 p) const {
    Vector2 cbest;
    double dbest = fINF;
    search(p, 1, cbest, dbest);
    return cbest;
}

void K2DTreeClosestPoint::search(const Vector2 &p, size_t r, Vector2 &cbest, double &dbest) const {
    const size_t &N = c.size();
    size_t level = size_bits(N) - size_bits(r);
    bool xAxisActive = (level%2 == 1);

    if(level <= 0){
        // If bin only has 1 element
        const Vector2 &candidate = c[r & ~(1uL << (size_bits(r)-1))];
        double d = p.getDistance(candidate);
        if(d < dbest){
            cbest = candidate;
            dbest = d;
        }
    } else {
        const double &median = split[r];
        double v = (xAxisActive ? p.x : p.y);
        size_t i = (r << 1) + (v < median ? 0 : 1);

        // Search in the child where p should be
        search(p, i, cbest, dbest);
        
        Vector2 median_coord = (xAxisActive ?
            Vector2(median, p.y) :
            Vector2(p.x, median)
        );
        // If there's a chance the closest point is on the brother of the
        // current tree node
        if(p.getDistance(median_coord) < dbest){
        // if(fabs(v-median) < dbest){
            // The brother of current tree node i is i^1, because we just flip
            // the least significant bit
            search(p, i^1, cbest, dbest);
        }
    }
}

