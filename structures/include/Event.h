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
    Vector2 point;

    // Site event
    Site* site;

    // Circle event
    Arc* arc;
    double radius;

    bool operator<(const Event &event) const;
    bool operator==(const Event &event) const;
public:
    Event(Site* site);                               // Site event
    Event(Arc* arc, Vector2 point, double radius);   // Circle event
};

#endif