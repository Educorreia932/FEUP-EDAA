#ifndef EVENT
#define EVENT

#include "Arc.h"
#include "VoronoiDiagram.h"

class Arc;

class Event {
public:
    enum Type {
        SITE,
        CIRCLE
    };

    Type type;
    bool valid = true;
    double y;

    // Site event
    Site site;

    // Circle event
    Arc* arc;

    bool operator<(const Event &event) const;
public:
    Event(Site site);           // Site event
    Event(Arc* arc, double y);  // Circle event
};

#endif