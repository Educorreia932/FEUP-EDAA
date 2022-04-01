#include <limits>
#include "FortuneAlgorithm.h"
#include "Event.h"

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
        sweep_line = event.y;
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

    middle_arc->s0 = Edge(start, arc_above->site.point, event.site.point);
    middle_arc->s1 = Edge(start, event.site.point, arc_above->site.point);

    // Link up the two edges
    middle_arc->s0.adjacent = &middle_arc->s1;
    middle_arc->s1.adjacent = &middle_arc->s0;

    left_arc->s1 = middle_arc->s0;
    right_arc->s0 = middle_arc->s1;

    // TODO: Invalidate circle events


    // TODO: Check for new circle events
}

// Remove parabola
void FortuneAlgorithm::handleCircleEvent(Event event) {

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

void FortuneAlgorithm::checkCircleEvents() {

}

