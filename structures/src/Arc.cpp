#include "Arc.h"

#include <cmath>

Arc::Arc(Site site) : site(site) {

}

Arc::Arc(Site site, Arc* left) : site(site), left(left) {

}

Arc::Arc(Site site, Arc* left, Arc* right) : site(site), left(left), right(right) {

}

Vector2 Arc::intersect(Arc* arc, double l) {
    Vector2 g1 = site.point;
    Vector2 g2 = arc->site.point;

    // Half-point between site and sweep line
    double f1 = (g1.y - l) / 2.0;
    double f2 = (g2.y - l) / 2.0;

    // Quadratic formula
    double a = 1 / (4 * f1) - 1 / (4 * f2);
    double b = -g1.x / (2 * f1) + g2.x / (2 * f2);
    double c = pow(g1.x, 2) / (4 * f1) + g1.y + f1 - (pow(g2.x, 2) / (4 * f2) + g2.y + f2);

    double x = (a - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double y = (1 / (4 * f1)) * pow(x, 2) - (g1.x / (2 * f1)) * x + pow(g1.x, 2) / (4 * f1) + g1.y + f1;

    return Vector2(x, y);
}




