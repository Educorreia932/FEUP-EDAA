#include "Event.h"

Event::Event(Type type) : type(type) {

}

Event::~Event() {

}

bool Event::operator<(const Event &event) const {
    return this->point.y < event.point.y;
}

SiteEvent::SiteEvent(Site site) : Event(SITE), site(site) {

}


CircleEvent::CircleEvent(Arc* arc) : Event(CIRCLE), arc(arc) {

}
