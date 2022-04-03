#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include "MapGraph.h"
#include "MapView.h"
#include "WindowView.h"
#include "Trip.h"

class MapTripsView: public View {
private:
    WindowView &windowView;
    MapView &mapView;
    const std::vector<Trip> &trips;
    std::vector<sf::Vertex> zip;
public:
    MapTripsView(WindowView &windowView_, MapView &mapView_, const std::vector<Trip> &trips_);
    virtual void draw();
};
