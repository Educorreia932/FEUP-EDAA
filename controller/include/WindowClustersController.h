#pragma once

#include "ClustersView.h"
#include "DraggableZoomableWindow.h"

#include <SFML/Graphics.hpp>

class WindowClustersController {
private:
    DraggableZoomableWindow &window;
    ClustersView &view;
public:
    WindowClustersController(DraggableZoomableWindow &window, ClustersView &view);

    void run();
};
