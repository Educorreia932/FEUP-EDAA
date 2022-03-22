#include "Event.h"

Event::Event(int x, int y) : site(Site{ x, y }), type(Type::SITE) {

};

Event::Event(Site site) : site(site), type(Type::SITE) {

};

Event::Event(Arc arc) : arc(arc), type(Type::CIRCLE) {

};
