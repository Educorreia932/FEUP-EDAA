#pragma once

#include <SFML/Graphics.hpp>

#include "WindowView.h"

class WindowController {
private:
    WindowView &windowView;
public:
    WindowController(WindowView &windowView_);

    void run();
};
