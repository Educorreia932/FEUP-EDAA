#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <vector>

#include "LineShape.h"
#include "MapGraph.h"
#include "MapView.h"

#include "polygon.h"

class MapGraphOsmView: public View {
private:
    sf::RenderTarget &window;
    MapView &mapView;
    const MapGraph &graph;
    std::vector<sf::Vertex> zip;
public:
    MapGraphOsmView(sf::RenderTarget &window_, MapView &mapView_, const MapGraph &graph_);
    virtual void refresh();
    virtual void draw();
};
