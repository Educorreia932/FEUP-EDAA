#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "DraggableZoomableWindow.h"
#include "DWGraph.h"
#include "MapGraph.h"
#include "MapMatching.h"
#include "MapTripMatchView.h"
#include "Trip.h"

class WindowTripController {
private:
    DraggableZoomableWindow &window;
    MapTripMatchView &mapTripMatchView;
    const MapGraph &mapGraph;
    const DWGraph::DWGraph &graph;
    const std::vector<Trip> &trips;
    const MapMatching &mapMatching;
    size_t tripIndex = 0;
    std::vector<Coord> currentMatches;
public:
    WindowTripController(
        DraggableZoomableWindow &window_,
        MapTripMatchView &mapTripMatchView_,
        const MapGraph &mapGraph_,
        const DWGraph::DWGraph &graph_,
        const std::vector<Trip> &trips_,
        const MapMatching &mapMatching_
    );

    void run();
private:
    void onChangeTrip();
};
