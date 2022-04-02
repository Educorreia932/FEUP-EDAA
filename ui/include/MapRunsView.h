#pragma once

#include "MapGraph.h"

class MapRunsView {
private:
    const MapGraph &graph;
public:
    MapRunsView(const MapGraph &g);

    void drawRuns() const;
};
