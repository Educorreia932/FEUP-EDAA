#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <vector>

#include "LineShape.h"
#include "MapGraph.h"
#include "MapView.h"
#include "WindowView.h"

#include "polygon.h"

class MapGraphOsmView: public View {
private:
    WindowView &windowView;
    MapView &mapView;
    const MapGraph &graph;
    std::vector<sf::Vertex> land;
    std::vector<sf::Vertex> water;
    std::vector<sf::Vertex> zip;
public:
    MapGraphOsmView(WindowView &windowView_, MapView &mapView_, const MapGraph &graph_, const std::vector<polygon_t> &polygons_);
    virtual void draw();
};
