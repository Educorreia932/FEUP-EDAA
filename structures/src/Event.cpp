#include "Event.h"

Event::Event(int x, int y) : site(Site{ x, y }) {

};

Event::Event(Site site) : site(site) {

};