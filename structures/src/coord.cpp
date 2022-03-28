#include "coord.h"

#include <cmath>

coord_t::coord_t(){}
coord_t::coord_t(coord_t::deg_t lat_, coord_t::deg_t lon_): lat(lat_),lon(lon_){}

double coord_t::getDistanceSI(const coord_t &p1, const coord_t &p2){
    coord_t m = (p1+p2)/2;
    double dlat = p2.lat - p1.lat;
    double dlon = p2.lon - p1.lon;
    double dx = m.getMetersPerLatDeg() * dlat;
    double dy = m.getMetersPerLonDeg() * dlon;
    return sqrt(dx*dx + dy*dy);
}

coord_t::deg_t coord_t::getDistanceDeg(const coord_t &p1, const coord_t &p2){
    double dlat = p2.lat - p1.lat;
    double dlon = p2.lon - p1.lon;
    return sqrt(dlat*dlat + dlon*dlon);
}

coord_t::deg_t coord_t::getDistanceDegSqr(const coord_t &p1, const coord_t &p2){
    double dlat = p2.lat - p1.lat;
    double dlon = p2.lon - p1.lon;
    return dlat*dlat + dlon*dlon;
}

coord_t coord_t::operator+(const coord_t &p) const{
    return coord_t(lat+p.lat, lon+p.lon);
}

coord_t coord_t::operator/(double d) const{
    return coord_t(lat/d, lon/d);
}

bool coord_t::operator==(const coord_t &c) const{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    return (lat == c.lat && lon == c.lon);
    #pragma GCC diagnostic pop
}

bool coord_t::operator<(const coord_t &c) const{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    if(lat != c.lat) return (lat < c.lat);
    return (lon < c.lon);
    #pragma GCC diagnostic pop
}

double coord_t::getMetersPerLatDeg() const{
    double phi = lat*M_PI/180.0;
    // const double phi = 41.2*M_PI/180.0;
    return 111132.92
          -   559.82  *cos(2*phi)
          +     1.175 *cos(4*phi)
          -     0.0023*cos(6*phi);
}

double coord_t::getMetersPerLonDeg() const{
    double phi = lat*M_PI/180.0;
    // const double phi = 41.2*M_PI/180.0;
    return 111412.84 *cos(  phi)
          -    93.5  *cos(3*phi)
          +     0.118*cos(5*phi);
}

coord_t::deg_t coord_t::getLat() const{ return lat; }
coord_t::deg_t coord_t::getLon() const{ return lon; }
