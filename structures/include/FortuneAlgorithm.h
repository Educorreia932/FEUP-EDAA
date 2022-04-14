#include "Event.h"
#include "VoronoiDiagram.h"

#include <set>
#include <vector>

class FortuneAlgorithm {
private:
    VoronoiDiagram diagram;
    std::vector<Edge*> edges;
    std::priority_queue<Event> events;
    std::set<Event> invalid_events;
    std::vector<Arc*> arcs; // List of parabolas
    double sweep_line = 0; // Current y-position of sweep line

    // Limits for bounding box
    double x0 = std::numeric_limits<double>::max();
    double y0 = std::numeric_limits<double>::max();
    double x1 = -std::numeric_limits<double>::max();
    double y1 = -std::numeric_limits<double>::max();

    Box bounding_box;

    void handleSiteEvent(Event event);
    void handleCircleEvent(Event event);
    Arc* locateArcAbove(Site site);
    Arc* breakArc(Arc* arc, Site* site);
    void invalidateCircleEvent(Arc* arc);
    void checkCircleEvents(Arc* arc);
public:
    FortuneAlgorithm(std::vector<Site*> sites);

    VoronoiDiagram construct();
};