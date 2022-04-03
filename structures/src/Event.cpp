#include "Event.h"

bool Event::operator<(const Event &event) const {
    return this->point.y < event.point.y;
}

Event::Event(Site site) : type(SITE), point(site.point), site(site) {

}

Event::Event(Arc* arc, Vector2 point, double radius) : type(CIRCLE), point(point), arc(arc), radius(radius) {
    arc->event = this;
}

