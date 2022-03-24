#ifndef ARC
#define ARC

#include "Event.h"
#include "VoronoiDiagram.h"

class Event;

class Arc {
public:
    Arc *left;
    Arc *right;
    Arc *previous;
    Arc *next;

    Segment *leftHalfEdge;
    Segment *rightHalfEdge;

    Site *site;
    Event *event;

    Arc(Site *site);
    Arc(Site *site, Arc *previous, Arc *next);
};

#endif