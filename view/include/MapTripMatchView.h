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
    Trip trip;
    std::vector<Coord> matches;
    std::vector<sf::Vertex> zip;
    sf::CircleShape beginCircle;
    sf::CircleShape endCircle;
public:
    MapTripMatchView(sf::RenderTarget &window_, MapView &mapView_);
    MapTripMatchView(sf::RenderTarget &window_, MapView &mapView_, const Trip &trip_, const std::vector<Coord> &matches_);
    void setTripMatches(const Trip &trip_, const std::vector<Coord> &matches_);
    virtual void refresh();
    virtual void draw();
};
