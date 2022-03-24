#ifndef ARC
#define ARC

#include "Event.h"
#include "VoronoiDiagram.h"

class Event;

class Arc {
public:
    Arc* parent;
    Arc* left;
    Arc* right;

    Segment* leftHalfEdge;
    Segment* rightHalfEdge;

    Site* site;
    Event* event;

    Arc(Site* site);
    Arc(Site* site, Arc* left, Arc* right);
};

#endif