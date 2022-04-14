#include "Event.h"

#include <cmath>

Event::Event(Site* site) : type(SITE), point(site->point), site(site) {

}

Event::Event(Arc* arc, Vector2 point, double radius) : type(CIRCLE), point(point), arc(arc), radius(radius) {
    arc->event = this;
}

bool Event::operator<(const Event &event) const {
    if (fabs(this->point.y - event.point.y) < 0.000001L)
        return this->point.x < event.point.x;

    return this->point.y < event.point.y;
}

bool Event::operator==(const Event &event) const {
    return this->point == event.point;
}

