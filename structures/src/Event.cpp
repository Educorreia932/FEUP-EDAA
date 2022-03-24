#include "Event.h"

Event::Event(int x, int y) : type(Type::SITE), site(new Site{Vector2(x, y)}) {

};

Event::Event(Site *site) : type(Type::SITE), site(site) {

};

Event::Event(Arc *arc) : type(Type::CIRCLE), arc(arc) {

}

Event::Event(double y, Vector2 point, Arc *arc) : type(Type::CIRCLE), y(y), point(point), arc(arc) {

}

bool Event::operator<(const Event &event) const {
    return y < event.site->point.y;
};


