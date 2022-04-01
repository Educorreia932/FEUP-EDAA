#include "Event.h"
#include "VoronoiDiagram.h"

#include <vector>

class FortuneAlgorithm {
private:
    VoronoiDiagram diagram;
    std::priority_queue<Event> events;
    Arc* root = nullptr;   // Binary tree for parabola arcs
    double sweep_line = 0; // Current y-position of sweep line

    void handleSiteEvent(Event event);
    void handleCircleEvent(Event event);
    Arc* locateArcAbove(Site site);
    Arc* breakArc(Arc* arc, Site site);
    void addEdge(Arc* left, Arc* right);
    void invalidateCircleEvent();
    void checkCircleEvents();
public:
    VoronoiDiagram construct(std::vector<Site> sites);
};