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

    virtual ~Event();

    bool operator<(const Event &event) const;
protected:
    Event(Type type);
};

class SiteEvent : public Event {
public:
    Site site;

    SiteEvent(Site site);
};

class CircleEvent : public Event {
public:
    Arc* arc;

    CircleEvent(Arc* arc);
};

#endif