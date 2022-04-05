#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <vector>

#include "LineShape.h"
#include "MapGraph.h"
#include "MapGraphOsmView.h"
#include "MapView.h"

#include "polygon.h"

class MapBuildingsOsmView: public View {
private:
    sf::RenderTarget &window;
    MapView &mapView;
    std::vector<sf::Vertex> building;
    std::vector<sf::Vertex> buildingOutlines;
public:
    MapBuildingsOsmView(sf::RenderTarget &window_, MapView &mapView_, const std::vector<polygon_t> &polygons_);
    virtual void draw();
};
