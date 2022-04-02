#include "Event.h"

bool Event::operator<(const Event &event) const {
    return this->point.y < event.point.y;
}

Event::Event(Site site) : type(SITE), point(site.point), site(site) {

}

Event::Event(Arc* arc, Vector2 point) : type(CIRCLE), point(point), arc(arc) {
    arc->event = this;
}

