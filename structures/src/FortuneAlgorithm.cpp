#include "FortuneAlgorithm.h"
#include "Event.h"

#include <cmath>
#include <limits>

VoronoiDiagram FortuneAlgorithm::construct(std::vector<Site> sites) {
    // Generate site events
    for (Site site: sites)
        events.push(Event(site));

    while (!events.empty()) {
        Event event = events.top();
        events.pop();

        if (event.type == Event::SITE)
            handleSiteEvent(event);

        else if (event.valid)
            handleCircleEvent(event);

        // Update sweep line position
        sweep_line = event.point.y;
    }

    // TODO: Bounding box

    return diagram;
}

// Add new parabola
void FortuneAlgorithm::handleSiteEvent(Event event) {
    // Beachline is empty
    if (root == nullptr) {
        root = new Arc(event.site);

        return;
    }

    Arc* arc_above = locateArcAbove(event.site);
    Arc* middle_arc = breakArc(arc_above, event.site);
    Arc* left_arc = middle_arc->previous;
    Arc* right_arc = middle_arc->next;

    // Starting point for the two new edges
    Vector2 start = middle_arc->get_point(event.site.point.x, sweep_line);

    middle_arc->s0 = new Edge(start, arc_above->site.point, event.site.point);
    middle_arc->s1 = new Edge(start, event.site.point, arc_above->site.point);

    // Link up the two edges
    middle_arc->s0->adjacent = middle_arc->s1;
    middle_arc->s1->adjacent = middle_arc->s0;

    left_arc->s1 = middle_arc->s0;
    right_arc->s0 = middle_arc->s1;

    // TODO: Invalidate circle events

    // Check for new circle events
    checkCircleEvents(middle_arc);
}

// Remove parabola
void FortuneAlgorithm::handleCircleEvent(Event event) {
    Arc* arc = event.arc;
    Arc* left_arc = arc->previous;
    Arc* right_arc = arc->next;

    left_arc->next = right_arc;
    right_arc->previous = left_arc;

    delete arc;

    // Set end points for edges
    Edge* left_edge = left_arc->s1;
    Edge* right_edge = right_arc->s0;

    left_edge->end = event.point;
    right_edge->end = event.point;

    // Add finished edges to diagram
    diagram.addEdge(*left_edge);
    diagram.addEdge(*right_edge);

    // TODO: Remove any circle events now not needed
    // TODO: Check to see if we need to create new circle events
}

Arc* FortuneAlgorithm::locateArcAbove(Site site) {
    Arc* current_arc = root;

    while (current_arc != nullptr) {
        // Check if site is under arc boundaries
        double a = -std::numeric_limits<double>::infinity();
        double b = std::numeric_limits<double>::infinity();

        if (current_arc->previous != nullptr)
            a = current_arc->intersect(*current_arc->previous, sweep_line).x;

        if (current_arc->next != nullptr)
            b = current_arc->next->intersect(*current_arc, sweep_line).x;

        if ((current_arc->previous == nullptr || a <= site.point.x) &&
            (current_arc->next == nullptr || site.point.x <= b))
            break;

        current_arc = current_arc->next;
    }

    return current_arc;
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, Site site) {
    Arc* middle_arc = new Arc(site, arc, arc);
    Arc* left_arc = new Arc(arc->site);
    Arc* right_arc = new Arc(arc->site);

    left_arc->s0 = arc->s0;
    right_arc->s1 = arc->s1;

    left_arc->previous = arc->next;
    left_arc->next = middle_arc;

    middle_arc->previous = left_arc;
    middle_arc->next = right_arc;

    right_arc->previous = middle_arc;
    right_arc->next = arc->next;

    // Delete old arc
    delete arc;

    return middle_arc;
}

void FortuneAlgorithm::checkCircleEvents(Arc* arc) {
    Vector2 intersection;
    Vector2 focus = arc->site.point;

    if (arc->s0->intersect(*arc->s1, intersection))
        return;

    // Calculate radius of circle
    double dy = focus.y - intersection.y;
    double dx = focus.x - intersection.x;
    double radius = sqrt(pow(dx, 2) + pow(dy, 2));

    // Check if sweepline hasn't passed possible circle event location
    if (intersection.y - radius > sweep_line) {
        Event circle_event = Event(arc, intersection.y - radius);
        events.push(circle_event);
    }
}

