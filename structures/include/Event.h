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

    bool valid = true;
    Type type;
    double y;

    // Site event
    Site *site;

    Event(int x, int y);
    Event(Site *site);

    // Circle event
    Vector2 point;
    Arc *arc;

    Event(Arc *arc);
    Event(double y, Vector2 point, Arc *arc);

    bool operator<(const Event &event) const;
};

#endif
