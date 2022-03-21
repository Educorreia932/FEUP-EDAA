#include "FortuneAlgorithm.h"

#include <queue>

VoronoiDiagram FortuneAlgorithm::construct(std::vector<Site> sites) {
    std::priority_queue<Event> events; // Site events sorted by y coordinate

    // Generate events for each site
    for (Site site : sites) 
        events.push(Event(site));

    VoronoiDiagram diagram = VoronoiDiagram();

    while (!events.empty()) {
        Event event = events.top();

        if (event.type == Event::Type::SITE)
            handleSiteEvent(event);

        else
            handleCircleEvent(event);
    }

    return diagram;
};

VoronoiDiagram FortuneAlgorithm::handleSiteEvent(Event event) {
    // Search in T for the arc alpha vertically above p
    // If alpha has pointer to circle event in Q, delete this event

    // Split alpha into alpha0 and alpha2
    // Let alpha1 be the new arc of p   
};

VoronoiDiagram FortuneAlgorithm::handleCircleEvent(Event event) {
    sweep_status.erase(sweep_status.find(event.arc));
};
