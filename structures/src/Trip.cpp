#include "Trip.h"

#include <fstream>

using namespace std;

vector<Trip> Trip::loadTrips(const std::string &filepath){
    vector<Trip> ret;

    ifstream is; is.exceptions(ifstream::failbit | ifstream::badbit);
    is.open(filepath);

    size_t N; is >> N;
    ret.reserve(N);
    for(size_t i = 0; i < N; ++i){
        ret.push_back(Trip());
        Trip &r = *ret.rbegin();
        size_t M;
        is >> r.id >> r.timestamp >> M;
        r.coords.reserve(M);
        Coord c;
        for(size_t j = 0; j < M; ++j){
            is >> c.lat() >> c.lon();
            r.coords.push_back(c);
        }
    }

    return ret;
}
