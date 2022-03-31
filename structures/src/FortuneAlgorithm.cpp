#include "FortuneAlgorithm.h"
#include "Event.h"

VoronoiDiagram FortuneAlgorithm::construct(std::vector<Site> sites) {
    // Generate site events
    for (Site site: sites)
        events.push(SiteEvent(site));

    while (!events.empty()) {
        Event event = events.top();
        events.pop();

        if (event.type == Event::SITE) {
            // Update sweep line position
            sweep_line = event.point.y;

            handleSiteEvent(*dynamic_cast<SiteEvent*>(&event));
        }

        else if (event.valid) {
            // Update sweep line position
            sweep_line = event.point.y;

            handleCircleEvent(*dynamic_cast<CircleEvent*>(&event));
        }
    }

    // TODO: Bounding box

    return diagram;
}

void FortuneAlgorithm::handleSiteEvent(SiteEvent event) {
    if (root == nullptr) {
        root = new Arc(event.site);

        return;
    }

    // Create arcs at intersections
    Arc* current_arc = root;

    // Traverse all arcs
    while (current_arc != nullptr) {
        Vector2 intersection_1;
        Vector2 intersection_2;

        if (checkIntersection(event.site, current_arc, intersection_1)) {
            bool intersect = checkIntersection(event.site, current_arc->right, intersection_2);

            if (current_arc->right != nullptr && !intersect) {
                current_arc->right->left = new Arc(event.site, current_arc, current_arc->right);
                current_arc->right = current_arc->right->left;
            }

            else
                current_arc->right = new Arc(current_arc->site, current_arc);

            current_arc->right->left_segment = current_arc->right_segment;

            current_arc->right->left = new Arc(event.site, current_arc, current_arc->right);
            current_arc->right = current_arc->right->left;

            current_arc = current_arc->right;

            // Add new edges connected to current arc's endpoints
            Segment segment = Segment{intersection_1};
            diagram.addEdge(segment);
            current_arc->left->right_segment = current_arc->right_segment = segment;

            segment = Segment{intersection_1};
            diagram.addEdge(segment);
            current_arc->right->left_segment = current_arc->right_segment = segment;

            // Check for circle events around the new arc
            checkCircleEvents(current_arc, event.point.x);
            checkCircleEvents(current_arc->left, event.point.x);
            checkCircleEvents(current_arc->right, event.point.x);

            return;
        }

        current_arc = current_arc->right; // Advance arc
    }

    // If site never intersects an arc, append it to the list
    current_arc = root;

    while (current_arc->right != nullptr)
        current_arc = current_arc->right;

    current_arc->right = new Arc(event.site, current_arc);

    // Insert new segment between p and i
    double x = 0;
    double y = (current_arc->right->site.point.y + current_arc->site.point.y) / 2.0;
    Vector2 start = Vector2(x, y);

    Segment segment = Segment{start};
    current_arc->right_segment = current_arc->right->left_segment = segment;
    diagram.addEdge(segment);
}

void FortuneAlgorithm::handleCircleEvent(CircleEvent event) {
    Arc* arc = event.arc;
    Arc* left_arc = arc->left;
    Arc* right_arc = arc->right;

    // If either have circle events, remove from queue

    // replace xl, p, xr by new edge x that starts at circumcenter of l,p and r sites.

    // Check circle events for left and right arcs
    checkCircleEvents(left_arc, event.point.x);
    checkCircleEvents(right_arc, event.point.x);
}

void FortuneAlgorithm::checkCircleEvents(Arc* arc, double x0) {
    // Look for a new circle event for arc
//    if (arc->event != nullptr && arc->event->)
//        arc->event->valid = false;

    arc->event = nullptr;

    if (arc->left == nullptr || arc->right == nullptr)
        return;

    double x;
    Vector2 o;
    bool flag = circle(arc->left->site.point, arc->site.point, arc->right->site.point, x, o);

    if (flag and x > x0) {
//        arc->event = new Event(x, o, arc);
        events.push(*arc->event);
    }
}

bool FortuneAlgorithm::circle(Vector2 a, Vector2 b, Vector2 c, double &x, Vector2 &o) {


    return false;
}

bool FortuneAlgorithm::checkIntersection(Site site, Arc* arc, Vector2 &intersection) {
    // Check whether a new arc at given site will intersect with the given arc
    if (arc->site.point.y == site.point.y)
        return false;

    // Get boundaries
    double a = 0.0;
    double b = 0.0;

    if (arc->left != nullptr)
        a = arc->intersect(arc->left, site.point.y).x;

    if (arc->right != nullptr)
        b = arc->intersect(arc->right, site.point.y).x;

    if (((arc->left == nullptr) || (a <= site.point.x)) && ((arc->right == nullptr) || (site.point.x <= b))) {
        intersection = arc->intersect(new Arc(site), site.point.y);

        return true;
    }

    return false;
}


