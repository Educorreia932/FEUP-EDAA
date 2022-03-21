#include "Site.h"

class Event {
private:
    int y;
public:
    enum Type {
        SITE,
        CIRCLE
    };

    Type type;

    // Site event
    Site site;

    // Circle event
    Arc arc;

    Event(int x, int y);
    Event(Site site);

    bool operator<(const Event& site) {
        return y < site.y;
    }
};