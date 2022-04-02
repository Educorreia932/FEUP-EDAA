#ifndef ARC
#define ARC

#include "Event.h"
#include "VoronoiDiagram.h"

class Event;

class Arc {
public:
    // Double linked list
    Arc* previous = nullptr;
    Arc* next = nullptr;

    Edge* s0; // Left edge
    Edge* s1; // Right edge

    Site site;
    Event* event = nullptr;

    Arc(Site site);
    Arc(Site site, Arc* previous, Arc* next);

    Vector2 getPoint(double x, double sweep_line);
    Vector2 intersect(Arc arc, double sweep_line);
};

#endif