#include <chrono>
#include <iostream>

#include "Trip.h"

const double NANOS_TO_SECS = 1.0/1000000000.0;

int main(){
    std::cout << "Loading trips..." << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    std::vector<Trip> trips = Trip::loadTrips("pkdd15-i/pkdd15-i.trips");
    auto end = std::chrono::high_resolution_clock::now();
    double dt = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())*NANOS_TO_SECS;
    std::cout << "Loaded trips, took " << dt << "s" << std::endl;

    std::cout << "Storing trips in binary format..." << std::endl;
    Trip::storeTripsBin(trips, "pkdd15-i/pkdd15-i.trips.bin");
    std::cout << "Stored trips in binary format" << std::endl;

    return 0;
}
