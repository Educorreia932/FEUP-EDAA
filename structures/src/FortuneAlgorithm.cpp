#include "FortuneAlgorithm.h"

#include <cmath>
#include <limits>

FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector2> points) : diagram(points) {

}

Arc* FortuneAlgorithm::locateArcAbove(Vector2 point) const {
    Arc* arc = root;

    while (true) {
        double breakpointLeft = -std::numeric_limits<double>::infinity();
        double breakpointRight = std::numeric_limits<double>::infinity();

        if (arc->left != nullptr)
            breakpointLeft = computeBreakpoint(*arc->left->site, *arc->site);

        if (arc->right != nullptr)
            breakpointRight = computeBreakpoint(*arc->site, *arc->right->site);

        if (point.x < breakpointLeft)
            arc = arc->left;

        else if (point.x > breakpointRight)
            arc = arc->right;

        else
            return arc;
    }
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, Site* site) {
    // Create the new subtree
    Arc* middleArc = new Arc(site, root, root);
    Arc* leftArc = new Arc(arc->site, root, root);
    Arc* rightArc = new Arc(arc->site, root, root);

    leftArc->leftHalfEdge = arc->leftHalfEdge;
    rightArc->rightHalfEdge = arc->rightHalfEdge;

    // Insert the subtree in the beachline
    middleArc->left = leftArc;
    middleArc->right = rightArc;

    return middleArc;
}

VoronoiDiagram FortuneAlgorithm::construct() {
    // Initialize event queue
    for (Site site: diagram.sites)
        events.push(Event(&site));

    // Process events
    while (!events.empty()) {
        Event event = events.top();

        events.pop();

        if (event.type == Event::Type::SITE)
            handleSiteEvent(event);

        else
            handleCircleEvent(event);
    }

    return diagram;
}

void FortuneAlgorithm::handleSiteEvent(Event event) {
    Site* site = event.site;

    // Check if beachline is empty
    if (root == nullptr) {
        root = new Arc(site, root, root);

        return;
    }

    // Look for the arc above the site
    Arc* arc_to_break = locateArcAbove(site->point);

    // Replace this arc by the new arcs
    Arc* middle_arc = breakArc(arc_to_break, site);
    Arc* left_arc = middle_arc->left;
    Arc* right_arc = middle_arc->right;

    // Add an edge in the diagram
    addEdge(left_arc, middle_arc);

    middle_arc->leftHalfEdge = middle_arc->rightHalfEdge;
    middle_arc->rightHalfEdge = middle_arc->leftHalfEdge;

    // Check circle events
    if (left_arc->right != nullptr)
        addEvent(left_arc->left, left_arc, middle_arc);

    if (right_arc->right != nullptr)
        addEvent(middle_arc, right_arc, right_arc->right);
};

void FortuneAlgorithm::handleCircleEvent(Event event) {
    if (event.valid) {
        Vector2 point = event.point;
        Arc* arc = event.arc;

        // Add vertex
        Vertex* vertex = diagram.createVertex(point);

        // Mark all the events involving this arc as invalid
        Arc* leftArc = arc->left;
        Arc* rightArc = arc->right;

        leftArc->event->valid = false;
        rightArc->event->valid = false;

        // Update the beachline and the diagram
        removeArc(arc, vertex);

        // Add new circle events
        if (leftArc->left != nullptr)
            addEvent(leftArc->left, leftArc, rightArc);

        if (rightArc->right != nullptr)
            addEvent(leftArc, rightArc, rightArc->right);
    }
}

void FortuneAlgorithm::addEdge(Arc* left, Arc* right) {
    // Create two new half-edges
    left->rightHalfEdge = diagram.createHalfEdge(left->site->face);
    right->leftHalfEdge = diagram.createHalfEdge(right->site->face);

    // Set the two half edges twins
    left->rightHalfEdge->twin = right->leftHalfEdge;
    right->leftHalfEdge->twin = left->rightHalfEdge;
}

void FortuneAlgorithm::addEvent(Arc* left, Arc* middle, Arc* right) {
    double y;
    Vector2 convergence_point = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);

    bool is_below = y <= beach_line_y;
    bool left_breakpoint_moving_right = left->site->point.y < middle->site->point.y;
    bool right_breakpoint_moving_right = middle->site->point.y < right->site->point.y;

    double left_initial_x = left_breakpoint_moving_right ? left->site->point.x : middle->site->point.x;
    double right_initial_x = right_breakpoint_moving_right ? middle->site->point.x : right->site->point.x;

    bool is_valid =
            ((left_breakpoint_moving_right && left_initial_x < convergence_point.x) ||
             (!left_breakpoint_moving_right && left_initial_x > convergence_point.x)) &&
            ((right_breakpoint_moving_right && right_initial_x < convergence_point.x) ||
             (!right_breakpoint_moving_right && right_initial_x > convergence_point.x));

    if (is_valid && is_below) {
        Event* event = new Event(y, convergence_point, middle);
        middle->event = event;
        events.push(*event);
    }
}

Vector2 FortuneAlgorithm::computeConvergencePoint(Vector2 point1, Vector2 point2, Vector2 point3, double &y) {
    Vector2 v1 = (point1 - point2).getOrthogonal();
    Vector2 v2 = (point2 - point3).getOrthogonal();
    Vector2 delta = 0.5 * (point3 - point1);
    double t = delta.getDet(v2) / v1.getDet(v2);
    Vector2 center = 0.5 * (point1 + point2) + t * v1;
    double r = center.getDistance(point1);
    y = center.y - r;

    return center;
}

void FortuneAlgorithm::removeArc(Arc* pArc, Vertex* pVertex) {

}

double FortuneAlgorithm::computeBreakpoint(Site site, Site site1) const {
    return 0;
}


Vector2 intersect(Vector2 p0, Vector2 p1, double sweep_line_y) {
    // Calculate the intersection of two parabolas
    Vector2 p = p0;
    double y;

    if (p0.x == p1.x)
        y = (p0.y + p1.y) / 2.0;

    else if (p1.x == sweep_line_y)
        y = p1.y;

    else if (p0.x == sweep_line_y) {
        y = p0.y;
        p = p1;
    }

    else {
        double z0 = 2 * (p0.x - sweep_line_y);
        double z1 = 2 * (p1.x - sweep_line_y);

        double a = 1 / z0 - 1 / z1;
        double b = -2 * (p0.y / z0 - p1.y / z1);
        double c = (pow(p0.y, 2) + pow(p0.x, 2)) / z0 - (pow(p1.y, 2) + pow(p1.x, 2) - pow(sweep_line_y, 2)) / z1;

        // Quadratic formula
        y = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    }

    double x = (pow(p.x, 2) + pow(p.y - y, 2) - pow(sweep_line_y, 2)) / (2 * p.x - 2 * sweep_line_y);

    return Vector2(x, y);
}

