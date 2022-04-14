#include "Arc.h"

#include <cmath>
#include <limits>

Arc::Arc(Site* site) : site(site) {

}

Arc::Arc(Site* site, Arc* previous, Arc* next) : previous(previous), next(next), site(site) {

}

// Get parabola point for x
Vector2 Arc::getPoint(double x, double sweep_line) {
    Vector2 focus = site->point;

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
    Vector2 point1 = this->site->point;
    Vector2 point2 = arc.site->point;

    double x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
    
    double d1 = 1.0 / (2.0 * (y1 - sweep_line));
	double d2 = 1.0 / (2.0 * (y2 - sweep_line));

	double a = d1 - d2;
	double b = 2.0 * (x2 * d2 - x1 * d1);
	double c = (y1 * y1 + x1 * x1 - sweep_line * sweep_line) * d1 - (y2 * y2 + x2 * x2 - sweep_line * sweep_line) * d2;
	double delta = b * b - 4.0 * a * c;

    double x = (-b + std::sqrt(delta)) / (2.0 * a);

    return getPoint(x, sweep_line);
}

// Intersect parabola with edge
Vector2 Arc::intersect(Edge edge, double sweep_line) {
    Vector2 focus = site->point;
    double x;

    if (edge.m == std::numeric_limits<double>::infinity())
        x = edge.start.x;

    else {
        // Distance between the parabola's focus and its vertex
        // Or distance from the parabola's vertex to its directrix (the sweep line)
        double p = (focus.y - sweep_line) / 2;

        // x, y of parabola's vertex,
        double h = focus.x;
        double k = focus.y - p;

        double a = 1 / (4 * p);
        double b = (-h / (2 * p)) - edge.m;
        double c = ((pow(h, 2) / (4 * p)) + k) - edge.c;

        double delta = b * b - 4 * a * c;
        double x1 = (-b + sqrt(delta)) / (2 * a);
        double x2 = (-b - sqrt(delta)) / (2 * a);

        double min = x1 < x2 ? x1 : x2;
        double max = x1 > x2 ? x1 : x2;
        x = edge.direction.x < 0 ? min : max;
    }

    return getPoint(x, sweep_line);
}
