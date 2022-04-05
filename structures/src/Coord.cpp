#include "Coord.h"

#include <cmath>

Coord::Coord() : Vector2(){}
Coord::Coord(double lat, double lon): Vector2(lon, lat){}

double Coord::getDistanceSI(const Coord &p1, const Coord &p2){
    Coord m = (p1+p2)/2;
    double dlat = p2.lat() - p1.lat();
    double dlon = p2.lon() - p1.lon();
    double dx = m.getMetersPerLatDeg() * dlat;
    double dy = m.getMetersPerLonDeg() * dlon;
    return sqrt(dx*dx + dy*dy);
}

double Coord::getMetersPerLatDeg() const{
    double phi = lat()*M_PI/180.0;
    // const double phi = 41.2*M_PI/180.0;
    return 111132.92
          -   559.82  *cos(2*phi)
          +     1.175 *cos(4*phi)
          -     0.0023*cos(6*phi);
}

double Coord::getMetersPerLonDeg() const{
    double phi = lat()*M_PI/180.0;
    // const double phi = 41.2*M_PI/180.0;
    return 111412.84 *cos(  phi)
          -    93.5  *cos(3*phi)
          +     0.118*cos(5*phi);
}

double &Coord::lat() { return y; }
double &Coord::lon() { return x; }

const double &Coord::lat() const { return y; }
const double &Coord::lon() const { return x; }

Coord& Coord::operator+=(const Coord& other){ Vector2::operator+=(other); return *this; }
Coord& Coord::operator-=(const Coord& other){ Vector2::operator-=(other); return *this; }
Coord& Coord::operator*=(double t){ Vector2::operator*=(t); return *this; }
Coord& Coord::operator/=(double t){ Vector2::operator/=(t); return *this; }

Coord Coord::operator+(const Coord &rhs) const { Vector2::operator+(rhs); return *this; }
Coord Coord::operator-(const Coord &rhs) const { Vector2::operator-(rhs); return *this; }
Coord Coord::operator*(double t) const { Vector2::operator*(t); return *this; }
Coord Coord::operator/(double t) const { Vector2::operator/(t); return *this; }

size_t std::hash<Coord>::operator()(const Coord& v) const { return hash<Vector2>()(v); }
