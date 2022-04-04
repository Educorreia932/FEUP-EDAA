#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <vector>

#include "LineShape.h"
#include "MapGraph.h"
#include "MapGraphOsmView.h"
#include "MapView.h"

#include "polygon.h"

class MapOsmView: public View {
private:
    sf::RenderTarget &window;
    MapView &mapView;
    std::vector<sf::Vertex> land;
    std::vector<sf::Vertex> water;
    std::vector<sf::Vertex> building;
    std::vector<sf::Vertex> buildingOutlines;
    MapGraphOsmView mapGraphOsmView;
public:
    MapOsmView(sf::RenderTarget &window_, MapView &mapView_, const MapGraph &graph_, const std::vector<polygon_t> &polygons_);
    virtual void draw();
};
