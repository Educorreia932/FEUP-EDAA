#pragma once

#include "MapGraph.h"

class MapTripsView {
private:
    const MapGraph &graph;
public:
    MapTripsView(const MapGraph &g);

    void drawTrips() const;
};
