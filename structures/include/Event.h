#include "Arc.h"

class Event {
private:
    int y;
public:
    enum Type {
        SITE,
        CIRCLE
    };

    Type type;
    bool valid = true;

    // Site event
    Site* site;

    Event(int x, int y);
    Event(Site site);

    // Circle event
    Arc arc;

    Event(Arc arc);

    bool operator<(const Event& site) {
        return y < site.y;
    }
};