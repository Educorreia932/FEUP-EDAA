#include "Arc.h"
#include "Event.h"
#include "VoronoiDiagram.h"

#include <set>

class FortuneAlgorithm {
    private:
        std::set<Arc> sweep_status;

        VoronoiDiagram handleSiteEvent(Event event);
        VoronoiDiagram handleCircleEvent(Event event);
    public:
        VoronoiDiagram construct(std::vector<Site> sites);
};
