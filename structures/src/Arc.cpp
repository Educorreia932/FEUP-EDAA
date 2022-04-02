#include "Arc.h"

#include <cmath>

Arc::Arc(Site site) : site(site) {

}

Arc::Arc(Site site, Arc* previous, Arc* next) : previous(previous), next(next), site(site) {

}

// Get parabola point for x
Vector2 Arc::get_point(double x, double sweep_line) {
    Vector2 focus = site.point;

    // Distance between the parabola's focus and its vertex
    // Or distance from the parabola's vertex to its directrix (the sweep line)
    double p = (focus.y - sweep_line) / 2;

    // x, y of parabola's vertex,
    double h = focus.x;
    double k = focus.y - p;

    double a = 1 / (4 * p);
    double b = -h / (2 * p);
    double c = (pow(h, 2) / (4 * p)) + k;

    double y = a * pow(x, 2) + b * x + c;

    return Vector2(x, y);
}

// Intersect two parabolas (left intersection)
Vector2 Arc::intersect(Arc arc, double sweep_line) {
    Vector2 g1 = site.point;
    Vector2 g2 = arc.site.point;

    // Half-point between site and sweep line
    double f1 = (g1.y - sweep_line) / 2.0;
    double f2 = (g2.y - sweep_line) / 2.0;

    // Quadratic formula
    double a = 1 / (4 * f1) - 1 / (4 * f2);
    double b = -g1.x / (2 * f1) + g2.x / (2 * f2);
    double c = pow(g1.x, 2) / (4 * f1) + g1.y + f1 - (pow(g2.x, 2) / (4 * f2) + g2.y + f2);

    double x = (a - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double y = (1 / (4 * f1)) * pow(x, 2) - (g1.x / (2 * f1)) * x + pow(g1.x, 2) / (4 * f1) + g1.y + f1;

    return Vector2(x, y);
}
