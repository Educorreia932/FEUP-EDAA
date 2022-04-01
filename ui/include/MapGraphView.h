#pragma once

#include "MapGraph.h"

class MapGraphView {
private:
    enum Display {
        MOTORWAY    = 1,
        TRUNK       = 2,
        PRIMARY     = 4,
        SECONDARY   = 8,
        TERTIARY    = 16,
        ROAD        = 32,
        RESIDENTIAL = 64,
        SLOW        = 128
    };

    static const std::unordered_map<edge_type_t, Display> display_map;

    const MapGraph &graph;
public:
    MapGraphView(const MapGraph &g);

    void drawRoads () const;
    void drawSpeeds() const;
    void drawVoronoi() const;
};
