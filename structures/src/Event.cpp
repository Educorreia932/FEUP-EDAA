#include "Event.h"

bool Event::operator<(const Event &event) const {
    return this->y < event.y;
}

Event::Event(Site site) : type(SITE), y(site.point.y), site(site) {

}

Event::Event(Arc* arc) : type(CIRCLE), arc(arc) {

}
