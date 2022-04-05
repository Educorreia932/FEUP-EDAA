#include "Trip.h"

#include <fstream>

using namespace std;

vector<Trip> Trip::loadTrips(const std::string &filepath){
    vector<Trip> ret;

    ifstream is; is.exceptions(ifstream::failbit | ifstream::badbit);
    is.open(filepath);

    size_t N; is >> N;
    ret.resize(N);
    for(size_t i = 0; i < N; ++i){
        Trip &trip = ret[i];
        size_t M;
        is >> trip.id >> trip.timestamp >> M;
        trip.coords.resize(M);
        for(Coord &c: trip.coords){
            is >> c.lat() >> c.lon();
        }
    }

    return ret;
}

void Trip::storeTripsBin(const std::vector<Trip> &trips, const std::string &filepath){
    ofstream os; os.exceptions(ifstream::failbit | ifstream::badbit);
    os.open(filepath, ios::binary);
    size_t N = trips.size();
    os.write((const char*)&N, sizeof(N));
    for(const Trip &trip: trips){
        os.write((const char*)&trip.id, sizeof(trip.id));
        os.write((const char*)&trip.timestamp, sizeof(trip.timestamp));
        size_t M = trip.coords.size();
        os.write((const char*)&M, sizeof(M));
        os.write((const char*)&trip.coords[0], sizeof(Coord)*M);
    }
}

vector<Trip> Trip::loadTripsBin(const std::string &filepath){
    vector<Trip> ret;

    ifstream is; is.exceptions(ifstream::failbit | ifstream::badbit);
    is.open(filepath, ios::binary);

    size_t N; is.read((char*)&N, sizeof(N));
    ret.resize(N);
    for(size_t i = 0; i < N; ++i){
        Trip &trip = ret[i];
        is.read((char*)&trip.id, sizeof(trip.id));
        is.read((char*)&trip.timestamp, sizeof(trip.timestamp));
        size_t M; is.read((char*)&M, sizeof(M));
        trip.coords.resize(M);
        is.read((char*)&trip.coords[0], sizeof(Coord)*M);
    }

    return ret;
}
