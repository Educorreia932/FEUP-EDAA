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
    std::list<Coord> path;
    std::vector<sf::Vertex> zip;
    std::vector<sf::Vertex> matchLines;
    std::vector<sf::Text> texts;
    sf::CircleShape beginCircle;
    sf::CircleShape endCircle;
    std::list<sf::CircleShape> circles;
public:
    MapTripMatchView(sf::RenderTarget &window_, MapView &mapView_);
    MapTripMatchView(sf::RenderTarget &window_, MapView &mapView_,
        const Trip &trip_, const std::vector<Coord> &matches_,
        const std::list<Coord> &path_);
    void setTripMatches(
        const Trip &trip_, const std::vector<Coord> &matches_,
        const std::list<Coord> &path_);
    virtual void refresh();
    virtual void draw();
};
