#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include "MapGraph.h"
#include "MapView.h"
#include "Trip.h"

class MapTripsView: public View {
private:
    sf::RenderTarget &window;
    MapView &mapView;
    const std::vector<Trip> &trips;
    std::vector<sf::Vertex> zip;
public:
    MapTripsView(sf::RenderTarget &window_, MapView &mapView_, const std::vector<Trip> &trips_);
    virtual void refresh();
    virtual void draw();
};
