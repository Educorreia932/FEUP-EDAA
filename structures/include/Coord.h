#pragma once

#include "Vector2.h"

#include <functional>

class Coord : public Vector2 {
private:
    double getMetersPerLatDeg() const;
    double getMetersPerLonDeg() const;
public:
    Coord();
    Coord(double lat, double lon);

    /**
     * @brief Get distance between two positions in SI units (meters).
     * 
     * @param p1        First position
     * @param p2        Second position
     * @return double   Distance between them in meters
     */
    static double getDistanceSI(const Coord &p1, const Coord &p2);
    
    double &lat();
    double &lon();

    const double &lat() const;
    const double &lon() const;

    Coord& operator+=(const Coord& other);
    Coord& operator-=(const Coord& other);
    Coord& operator*=(double t);
    Coord& operator/=(double t);

    Coord operator+(const Coord &rhs) const;
    Coord operator-(const Coord &rhs) const;
    Coord operator*(double t) const;
    Coord operator/(double t) const;
};

namespace std {
    template <> struct hash<Coord> {
        size_t operator()(const Coord& v) const;
    };
}
