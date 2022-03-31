#ifndef ARC
#define ARC

#include "Event.h"
#include "VoronoiDiagram.h"

class Event;

class Arc {
public:
    Site site;
    Arc* left;
    Arc* right;
    Segment left_segment;
    Segment right_segment;
    Event* event;

    Arc(Site site);
    Arc(Site site, Arc* left);
    Arc(Site site, Arc* left, Arc* right);

    Vector2 intersect(Arc* arc, double y);
};

#endif