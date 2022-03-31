#include "Event.h"
#include "VoronoiDiagram.h"

#include <vector>

class FortuneAlgorithm {
private:
    VoronoiDiagram diagram;
    std::priority_queue<Event> events;
    Arc* root = nullptr; // Binary tree for parabola arcs
    double sweep_line = 0;

    void handleSiteEvent(SiteEvent event);
    void handleCircleEvent(CircleEvent event);
    void checkCircleEvents(Arc* arc, double x0);
    bool checkIntersection(Site site, Arc* arc, Vector2 &intersection);
public:
    VoronoiDiagram construct(std::vector<Site> sites);
    bool circle(Vector2 a, Vector2 b, Vector2 c, double &x, Vector2 &o);
};