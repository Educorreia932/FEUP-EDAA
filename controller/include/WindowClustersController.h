#pragma once

#include "ClustersView.h"
#include "DraggableZoomableWindow.h"

#include <SFML/Graphics.hpp>

class WindowClustersController {
private:
    DraggableZoomableWindow &window;
    ClustersView &view;
    
    sf::Time elapsed_time;
    sf::Clock r;
public:
    WindowClustersController(DraggableZoomableWindow &window, ClustersView &view);

    void run();
};
