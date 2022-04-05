#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <vector>

#include "LineShape.h"
#include "MapGraph.h"
#include "MapView.h"
#include "Trip.h"

#include "polygon.h"

class MapTripMatchView: public View {
private:
    sf::RenderTarget &window;
    MapView &mapView;
    const Trip &trip;
    const std::vector<Coord> &matches;
    std::vector<sf::Vertex> zip;
public:
    MapTripMatchView(sf::RenderTarget &window_, MapView &mapView_, const Trip &trip_, const std::vector<Coord> &matches_);
    virtual void refresh();
    virtual void draw();
};
