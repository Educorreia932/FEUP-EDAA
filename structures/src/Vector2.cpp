#include "Vector2.h"

#include <cmath>

Vector2::Vector2(double x, double y) : x(x), y(y) {

}

Vector2 &Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;

    return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;

    return *this;
}

Vector2 &Vector2::operator*=(double t) {
    x *= t;
    y *= t;

    return *this;
}

bool operator==(Vector2 vec, const Vector2 &rhs) {
    return vec.x == rhs.x && vec.y == rhs.y;
}

Vector2 Vector2::getOrthogonal() const {
    return Vector2(-y, x);
}

double Vector2::dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
}

double Vector2::getNorm() const {
    return std::sqrt(x * x + y * y);
}

double Vector2::getDistance(const Vector2 &other) const {
    return (*this - other).getNorm();
}

double Vector2::getDet(Vector2 v) {
    return x * v.y - y * v.x;
}

Vector2 operator+(Vector2 lhs, const Vector2 &rhs) {
    lhs += rhs;

    return lhs;
}

Vector2 operator-(Vector2 lhs, const Vector2 &rhs) {
    lhs -= rhs;

    return lhs;
}

Vector2 operator*(double t, Vector2 vec) {
    vec *= t;

    return vec;
}

Vector2 operator*(Vector2 vec, double t) {
    return t * vec;
}

bool operator==(const Vector2 &lhs, const Vector2 &rhs){
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool Vector2::operator<(const Vector2 &vector) const {
    return x < vector.x && y < vector.y;
}

bool collinear(Vector2 a, Vector2 b, Vector2 c) {
    return (b.x - a.x) * (c.y - a.y) == (c.x - a.x) * (b.y - a.y);
}

bool within(double p, double q, double r) {
    return (p <= q && q <= r) || (r <= q && q <= p);
}

bool Vector2::isOn(Vector2 a, Vector2 b) const {
    Vector2 c = *this;

    return collinear(a, b, c) && (a.x != b.x ? within(a.x, c.x, b.x) : within(a.y, c.y, b.y));
}