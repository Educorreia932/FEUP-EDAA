#include "Arc.h"
#include "Event.h"
#include "VoronoiDiagram.h"

#include <set>

class FortuneAlgorithm {
    private:
        Arc* root = nullptr;
        float beach_line_y = 0.0f;
        VoronoiDiagram diagram = VoronoiDiagram();

        VoronoiDiagram handleSiteEvent(Event event);
        VoronoiDiagram handleCircleEvent(Event event);
        Arc* locateArcAbove(Site* site);
        float computeBreakpoint(Site& p1, Site& p2) const;
    public:
        VoronoiDiagram construct(std::vector<Site> sites);
};
