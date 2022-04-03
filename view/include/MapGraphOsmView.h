#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include "LineShape.h"
#include "MapGraph.h"
#include "MapView.h"
#include "WindowView.h"

class MapGraphOsmView: public View {
private:
    WindowView &windowView;
    MapView &mapView;
    const MapGraph &graph;
    std::vector<sf::Vertex> zip;
public:
    MapGraphOsmView(WindowView &windowView_, MapView &mapView_, const MapGraph &graph_);
    virtual void draw();
};
