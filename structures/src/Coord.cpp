#include "Coord.h"

#include <cmath>

const double DEG_TO_RAD = M_PI/180.0;
const double EARTH_RADIUS = 6371000.0;

Coord::Coord() : Vector2(){}
Coord::Coord(double lat, double lon): Vector2(lon, lat){}
Coord::Coord(const Vector2 &v): Vector2(v){}

static double haversine(double lat1, double lon1, double lat2, double lon2){
    double dLat = (lat1 - lat2) * DEG_TO_RAD;
    dLat = sin(dLat/2.0);

    double dLon = (lon1 - lon2) * DEG_TO_RAD;
    dLon = sin(dLon/2.0);

    double a = (dLat*dLat + 
                dLon*dLon *
                    cos(lat1*DEG_TO_RAD) * cos(lat2*DEG_TO_RAD));
    double c = 2.0 * asin(sqrt(a));
    return c;
}

double Coord::getDistanceArc(const Coord &p1, const Coord &p2){
    return haversine(p1.y, p1.x, p2.y, p2.x) * EARTH_RADIUS;
}
double Coord::getDistanceArc(const Vector2 &p1, const Vector2 &p2){
    return haversine(p1.y, p1.x, p2.y, p2.x) * EARTH_RADIUS;
}

double Coord::getDistanceArcSimple(const Coord &p1, const Coord &p2){
    double dy = (p1.y - p2.y)*Coord::LatDegreesToMeters();
    double dx = (p1.x - p2.x)*Coord::LonDegreesToMeters();
    return sqrt(dx*dx + dy*dy);
}

double &Coord::lat() { return y; }
double &Coord::lon() { return x; }

const double &Coord::lat() const { return y; }
const double &Coord::lon() const { return x; }

Coord& Coord::operator+=(const Coord& other){ Vector2::operator+=(other); return *this; }
Coord& Coord::operator-=(const Coord& other){ Vector2::operator-=(other); return *this; }
Coord& Coord::operator*=(double t){ Vector2::operator*=(t); return *this; }
Coord& Coord::operator/=(double t){ Vector2::operator/=(t); return *this; }

Coord Coord::operator+(const Coord &rhs) const { return Coord(Vector2::operator+(rhs)); }
Coord Coord::operator-(const Coord &rhs) const { return Coord(Vector2::operator-(rhs)); }
Coord Coord::operator*(double t) const { return Coord(Vector2::operator*(t)); }
Coord Coord::operator/(double t) const { return Coord(Vector2::operator/(t)); }

size_t std::hash<Coord>::operator()(const Coord& v) const { return hash<Vector2>()(v); }

bool Coord::compXY(const Coord &c1, const Coord &c2){
    return Vector2::compXY(c1, c2);
}

double Coord::MetersToLatDegrees(){ return 1.0/(111000.0); }
double Coord::MetersToLonDegrees(){ return 1.0/( 83500.0); }
double Coord::LatDegreesToMeters(){ return 111000.0; }
double Coord::LonDegreesToMeters(){ return  83500.0; }
