#include "Arc.h"
#include "Event.h"
#include "VoronoiDiagram.h"

#include <set>
#include <vector>
#include <queue>

class FortuneAlgorithm {
private:
    std::priority_queue<Event> events; // Site events sorted by y coordinate
    Arc *root = nullptr;
    double beach_line_y = 0.0f;
    VoronoiDiagram diagram = VoronoiDiagram();

    void handleSiteEvent(Event event);
    VoronoiDiagram handleCircleEvent(Event event);
    void addEdge(Arc *left, Arc *right);
    void addEvent(Arc *left, Arc *middle, Arc *right);
    Vector2 computeConvergencePoint(Vector2 point1, Vector2 point2, Vector2 point3, double &y);
public:
    VoronoiDiagram construct(std::vector<Site> sites);
    Arc *locateArcAbove(Vector2 point);
    void removeArc(Arc *pArc, Vertex *pVertex);
    double computeBreakpoint(Site site, Site site1);
};
