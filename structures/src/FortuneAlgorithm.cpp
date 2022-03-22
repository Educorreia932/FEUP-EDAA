#include "FortuneAlgorithm.h"

#include <cmath>
#include <limits>
#include <queue>

VoronoiDiagram FortuneAlgorithm::construct(std::vector<Site> sites) {
    std::priority_queue<Event> events; // Site events sorted by y coordinate

    // Initialize event queue
    for (Site site : sites) 
        events.push(Event(site));

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
};

VoronoiDiagram FortuneAlgorithm::handleSiteEvent(Event event) {
    Site* site = event.site;

    // Check if beachline is empty
    if (root == nullptr) {
        root = new Arc(site);

        return;
    }

    // Look for the arc above the site
    Arc* arcToBreak = locateArcAbove(site);

    // Replace this arc by the new arcs
    Arc* middleArc = breakArc(arcToBreak, site);
    Arc* leftArc = middleArc->previous;
    Arc* rightArc = middleArc->next;

    // Add an edge in the diagram
    addEdge(leftArc, middleArc);

    middleArc->leftHalfEdge = middleArc->rightHalfEdge;
    middleArc->rightHalfEdge = middleArc->leftHalfEdge;

    // Check circle events  
    if (leftArc->next != nullptr)
        addEvent(leftArc->previous, leftArc, middleArc);

    if (rightArc->next != nullptr)
        addEvent(middleArc, rightArc, rightArc->next);
};

VoronoiDiagram FortuneAlgorithm::handleCircleEvent(Event event) {
    if (event.valid) {
        Segment edge = Segment {event.site.x, event.site.y};
        
        diagram.addEdge(edge);

        // Remove associated arc
        Arc arc = event.arc;

        if (arc.previous != nullptr) {
            arc.previous->next = arc.next;
            arc.previous->leftHalfEdge = edge;
        }

        if (arc.next = nullptr) {
            arc.next->previous = arc.previous;
            arc.next->rightHalfEdge = edge;
        }

        // Finish the edges before and after the arc
        if (arc.rightHalfEdge)
    }
};

Arc* FortuneAlgorithm::locateArcAbove(Site* site) {
    Arc* arc = root;

    while (true) {
        double breakpointLeft = -std::numeric_limits<double>::infinity();
        double breakpointRight = std::numeric_limits<double>::infinity();

        if (arc->previous != nullptr)
            breakpointLeft = computeBreakpoint(*arc->previous->site, *arc->site);

        if (arc->next != nullptr)
            breakpointRight = computeBreakpoint(*arc->site, *arc->next->site);

        if (site->x < breakpointLeft)
            arc = arc->left;

        else if (site->x > breakpointRight)
            arc = arc->right;

        else
            return arc;
    }
}

float FortuneAlgorithm::computeBreakpoint(Site& p0, Site& p1) const {
    float x0 = p0.x;
    float y0 = p0.y;
    float x1 = p1.x;
    float y1 = p1.y;

    double d0 = 1.0 / (2.0 * (y0 - beach_line_y));
    double d1 = 1.0 / (2.0 * (y1 - beach_line_y));
    
    // Factors for quadratic formula
    double a = d0 - d1;
    double b = 2.0 * (x1 * d1 - x0 * d0);
    double c = (pow(y0, 2) + pow(x0, 2) - pow(beach_line_y, 2)) * d0 - (pow(y1, 2) + pow(x1, 2) - pow(beach_line_y, 2)) * d1;
    double delta = pow(b, 2) - 4 * a * c;

    // Solve quadratic equation 
    return (-b + sqrt(delta)) / (2.0 * a);
}

Arc* breakArc(Arc* arc, Site* site) {
    // Create the new subtree
    Arc* middleArc = new Arc(site);
    Arc* leftArc = new Arc(arc->site);
    Arc* rightArc = new Arc(arc->site);

    leftArc->leftHalfEdge = arc->leftHalfEdge;
    rightArc->rightHalfEdge = arc->rightHalfEdge;

    // Insert the subtree in the beachline
    arc = middleArc;
    middleArc->previous = leftArc;
    middleArc->next = rightArc;

    return middleArc;
}

void addEdge() {

}

void addEvent(Arc* left, Arc* middle, Arc* right) {
    Site convergence_point = 
}

bool intersect(Site site, Arc arc) {
    return false;
}

Site intersect(Site p0, Site p1, float sweep_line_y) {
    // Calculate the intersection of two parabolas
    Site p = p0;
    float y;

    if (p0.x == p1.x) 
        y = (p0.y + p1.y) / 2.0;

    else if (p1.x == sweep_line_y)
        y = p1.y;

    else if (p0.x == sweep_line_y) {
        y = p0.y;
        p = p1;
    }

    else {
        float z0 = 2 * (p0.x - sweep_line_y);
        float z1 = 2 * (p1.x - sweep_line_y);

        float a = 1 / z0 - 1 / z1;
        float b = -2 * (p0.y / z0 - p1.y / z1);
        float c = (pow(p0.y, 2) + pow(p0.x, 2)) / z0 - (pow(p1.y, 2) + pow(p1.x, 2) - pow(sweep_line_y, 2)) / z1;

        // Quadratic formula
        y = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    }

    float x = (pow(p.x, 2) + pow(p.y - y, 2) - pow(sweep_line_y, 2)) / (2 * p.x - 2 * sweep_line_y);

    return Site{x, y};
}
