#pragma once

#include <SFML/Graphics.hpp>

#include "DraggableZoomableWindow.h"

class WindowController {
private:
    DraggableZoomableWindow &window;
public:
    WindowController(DraggableZoomableWindow &window);

    void run();
};
