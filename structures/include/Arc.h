#pragma once

#include "Event.h"
#include "VoronoiDiagram.h"

class Arc {
    public:
        Arc* left;
        Arc* right;
        Arc* previous;
        Arc* next;

        Segment leftHalfEdge;
        Segment rightHalfEdge;

        Site* site;

        Arc(Site* site);
        Arc(Site* site, Arc* previous, Arc* next);
};
