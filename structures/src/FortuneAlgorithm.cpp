#include "FortuneAlgorithm.h"

#include <cmath>
#include <limits>
#include <iostream>
#include <iomanip>

FortuneAlgorithm::FortuneAlgorithm(std::vector<Site*> sites) : diagram(sites) {

}

VoronoiDiagram FortuneAlgorithm::construct() {
    // Generate site events
    for (std::size_t i = 0; i < diagram.sites.size(); i++) {
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

        bool start_inside = bounding_box.contains(edge->start);
        bool end_inside = bounding_box.contains(edge->end);
        bool include_edge = true;
        bool intersects = bounding_box.intersect(*edge, intersection);

        // Edge is not finished (doesn't have a end point)
        if (!edge->finished) {
            // 1) Edge starts inside the bounding box
            if (start_inside) {
                edge->end = intersection;
                edge->finished = true;
            }

            // 2) Edge starts outside the bounding box and intersects it
            else if (intersects) {
                edge->start = intersection;

                bounding_box.intersect(*edge, intersection);

                edge->end = intersection;
                edge->finished = true;
            }

            // 3) Edge starts outside the bounding box and doesn't intersect it
            else
                include_edge = false;
        }

        // Edge is finished
        else {
            intersects = intersects && intersection.isOn(edge->start, edge->end);

            // 4.1) Edge starts inside and ends outside the bounding box
            if (start_inside && !end_inside)
                edge->end = intersection;

            // 4.2) Edge starts and ends outside the bounding box and doesn't intersect it
            else if (!start_inside && end_inside)
                edge->start = intersection;

            // 5) Edge starts and ends outside the bounding box and doesn't intersect it
            else if (!start_inside && !end_inside && !intersects)
                include_edge = false;

            // 6) Edge starts and ends outside the bounding box and intersects it
            else if (!start_inside && !end_inside && intersects) {
                edge->start = intersection;

                bounding_box.intersect(*edge, intersection);

                edge->end = intersection;
            }
        }

        if (include_edge && edge->finished)
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
    if (arcs.empty()) {
        arcs.push_back(new Arc(event.site));

        return;
    }

    Arc* arc_above = locateArcAbove(*event.site);

    // Starting point for the two new edges
    Vector2 start = arc_above->getPoint(event.site->point.x, sweep_line);

    std::cout << std::setprecision(9) << arc_above->site->point.x << " " << arc_above->site->point.y << std::endl;

    Arc* middle_arc = breakArc(arc_above, event.site);
    Arc* left_arc = middle_arc->previous;
    Arc* right_arc = middle_arc->next;

    Edge* left_edge = new Edge(start, arc_above->site, event.site);
    Edge* right_edge = new Edge(start, event.site, arc_above->site);

    // Add edges to diagram 
    edges.push_back(left_edge);
    edges.push_back(right_edge);

    // Link up the two edges
    left_edge->adjacent = right_edge;
    right_edge->adjacent = left_edge;

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

    removeArc(arc);

    delete arc;

    // Check to see if we need to create new circle events
    checkCircleEvents(left_arc);
    checkCircleEvents(right_arc);
}

Arc* FortuneAlgorithm::locateArcAbove(Site site) {
    Arc* arc_above = nullptr;
    Vector2 closest(site.point.x, std::numeric_limits<double>::infinity()); // Closest intersection in y-axis
    Edge beam(site.point, site.point - Vector2(1, 0), site.point + Vector2(1, 0)); // Vertical line that intersects parabolas

    // for (int i = 0; i < arcs.size(); i++) {
    for (int i = arcs.size() - 1; i >= 0; i--) {
        Arc* arc = arcs[i];
        Vector2 intersection = arc->intersect(beam, sweep_line);

        if (intersection.y < closest.y) {
            closest = intersection;
            arc_above = arc;
        }
    }

    return arc_above;
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, Site* site) {
    Arc* left_arc = new Arc(arc->site);
    Arc* right_arc = new Arc(arc->site);
    Arc* middle_arc = new Arc(site, left_arc, right_arc);

    removeArc(arc);

    arcs.push_back(left_arc);
    arcs.push_back(right_arc);
    arcs.push_back(middle_arc);

    left_arc->s0 = arc->s0;
    right_arc->s1 = arc->s1;
    left_arc->event = arc->event;
    right_arc->event = arc->event;

    left_arc->previous = arc->previous;
    left_arc->next = middle_arc;

    if (left_arc->previous != nullptr)
        left_arc->previous->next = left_arc;

    right_arc->previous = middle_arc;
    right_arc->next = arc->next;

    if (right_arc->next != nullptr)
        right_arc->next->previous = right_arc;


    // Delete old arc
    delete arc;

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
    if (intersection.y - radius < sweep_line) {
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

void FortuneAlgorithm::removeArc(Arc* arc) {
    std::vector<Arc*>::iterator position = std::find(arcs.begin(), arcs.end(), arc);

    if (position != arcs.end())
        arcs.erase(position);
}