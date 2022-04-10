#include "FortuneAlgorithm.h"

#include <cmath>
#include <limits>
#include <iostream>

FortuneAlgorithm::FortuneAlgorithm(std::vector<Site*> sites) : diagram(sites) {

}

VoronoiDiagram FortuneAlgorithm::construct() {
    // Generate site events
    for (int i = 0; i < diagram.sites.size(); i++) {
        Site* site = diagram.sites[i];
        events.push(Event(site));

        // Set bounding box limits
        x0 = std::min(x0, site->point.x);
        y0 = std::min(y0, site->point.y);
        x1 = std::max(x1, site->point.x);
        y1 = std::max(y1, site->point.y);
    }

    bounding_box = Box(Vector2(x0, y0), Vector2(x1, y1));

    while (!events.empty()) {
        Event event = events.top();
        events.pop();

        // Update sweep line position
        sweep_line = event.point.y;

        if (event.type == Event::SITE)
            handleSiteEvent(event);

        else if (invalid_events.find(event) == invalid_events.end())
            handleCircleEvent(event);
    }

    // Finish edges
    for (Edge* edge : edges) {
        Vector2 intersection;

        if (!edge->finished) {
            if (bounding_box.intersect(*edge, intersection)) {
                edge->finished = true;
                edge->end = intersection;
            }
        }

        if (!bounding_box.contains(edge->start))
            if (bounding_box.intersect(*edge, intersection))
                edge->start = intersection;

        if (edge->finished)
            diagram.addEdge(*edge);
    }

    for (Edge bound : bounding_box.bounds)
        diagram.addEdge(bound);

    diagram.bounding_box = bounding_box;

    return diagram;
}

// Add new parabola
void FortuneAlgorithm::handleSiteEvent(Event event) {
    // Beachline is empty
    if (root == nullptr) {
        root = new Arc(event.site);

        return;
    }

    Arc& arc_above = locateArcAbove(*(event.site));

    // Starting point for the two new edges
    Vector2 start = arc_above.getPoint(event.site->point.x, sweep_line);

    Edge* left_edge = new Edge(start, arc_above.site, event.site);
    Edge* right_edge = new Edge(start, event.site, arc_above.site);

    // Add edges to diagram 
    edges.push_back(left_edge);
    edges.push_back(right_edge);

    // Link up the two edges
    left_edge->adjacent = right_edge;
    right_edge->adjacent = left_edge;

    Arc* middle_arc = breakArc(&arc_above, event.site);
    Arc* left_arc = middle_arc->previous;
    Arc* right_arc = middle_arc->next;

    middle_arc->s0 = left_edge;
    middle_arc->s1 = right_edge;

    left_arc->s1 = middle_arc->s0;
    right_arc->s0 = middle_arc->s1;

    // Check for new circle events
    checkCircleEvents(left_arc);
    checkCircleEvents(right_arc);
}

// Remove parabola
void FortuneAlgorithm::handleCircleEvent(Event event) {
    Arc* arc = event.arc;
    Arc* left_arc = arc->previous;
    Arc* right_arc = arc->next;

    left_arc->next = right_arc;
    right_arc->previous = left_arc;

    Edge* left_edge = left_arc->s1;
    Edge* right_edge = right_arc->s0;

    Vector2 point = Vector2(event.point.x, event.point.y + event.radius);

    left_edge->end = point;
    right_edge->end = point;
    left_edge->finished = true;
    right_edge->finished = true;

    Edge* boundary_ray = new Edge(point, left_arc->site, right_arc->site);
    edges.push_back(boundary_ray);

    left_arc->s1 = boundary_ray;
    right_arc->s0 = boundary_ray;

    // Check to see if we need to create new circle events
    checkCircleEvents(left_arc);
    checkCircleEvents(right_arc);
}

Arc& FortuneAlgorithm::locateArcAbove(Site site) {
    Arc* current_arc = root;

    // TODO: Can be optimized to log n using binary search
    while (current_arc != nullptr) {
        // Check if site is under arc boundaries
        double a = -std::numeric_limits<double>::infinity();
        double b = std::numeric_limits<double>::infinity();

        if (current_arc->previous != nullptr)
            a = current_arc->intersect(*current_arc->previous->s1, sweep_line).x;

        if (current_arc->next != nullptr)
            b = current_arc->intersect(*current_arc->next->s0, sweep_line).x;

        if ((current_arc->previous == nullptr || a <= site.point.x) &&
            (current_arc->next == nullptr || site.point.x <= b))
            break;

        current_arc = current_arc->next;
    }

    return *current_arc;
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, Site* site) {
    Arc* middle_arc = new Arc(site, arc, arc);
    Arc* left_arc = new Arc(arc->site);
    Arc* right_arc = new Arc(arc->site);

    left_arc->s0 = arc->s0;
    right_arc->s1 = arc->s1;

    left_arc->previous = arc->previous;
    left_arc->next = middle_arc;

    if (left_arc->previous != nullptr)
        left_arc->previous->next = left_arc;

    middle_arc->previous = left_arc;
    middle_arc->next = right_arc;

    right_arc->previous = middle_arc;
    right_arc->next = arc->next;

    if (right_arc->next != nullptr)
        right_arc->next->previous = right_arc;

    if (arc == root)
        root = left_arc;

    return middle_arc;
}

void FortuneAlgorithm::checkCircleEvents(Arc* arc) {
    Vector2 intersection;

    if (arc->s0 == nullptr || arc->s1 == nullptr || !arc->s0->intersect(*arc->s1, intersection))
        return;

    Vector2 focus = arc->site->point;

    // Calculate radius of circle
    double dy = focus.y - intersection.y;
    double dx = focus.x - intersection.x;
    double radius = sqrt(pow(dx, 2) + pow(dy, 2));

    // Check if sweepline hasn't passed possible circle event location
    if (intersection.y - radius < sweep_line && bounding_box.contains(intersection)) {
        // Remove any circle events now not needed
        invalidateCircleEvent(arc);

        // Create circle event
        Event* circle_event = new Event(arc, Vector2(intersection.x, intersection.y - radius), radius);
        events.push(*circle_event);
    }
}

void FortuneAlgorithm::invalidateCircleEvent(Arc* arc) {
    if (arc->event != nullptr) 
        invalid_events.insert(*arc->event);        
}
