#ifndef VECTOR_2
#define VECTOR_2

#include <ostream>

class Vector2 {
public:
    double x;
    double y;

    Vector2(double x = 0.0, double y = 0.0);

    // Unary operators
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(double t);

    Vector2 getOrthogonal() const;
    double getNorm() const;
    double dot(const Vector2 &other) const;
    double getDistance(const Vector2 &other) const;
    double getDet(Vector2 vector2);
};

// Binary operators
Vector2 operator+(Vector2 lhs, const Vector2 &rhs);
Vector2 operator-(Vector2 lhs, const Vector2 &rhs);
Vector2 operator*(double t, Vector2 vec);
Vector2 operator*(Vector2 vec, double t);
bool operator==(Vector2 vec, const Vector2 &rhs);

#endif