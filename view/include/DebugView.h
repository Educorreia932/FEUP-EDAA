#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>

#include "FpsMonitor.h"

class DebugView : public View {
private:
    static const sf::Font FONT;           ///< @brief Debug font.
    static const int FONT_SIZE = 14;      ///< @brief Debug font size, in pixels.

    sf::RenderWindow *window;
    sf::View v;
    const FpsMonitor &fpsMonitor;
    sf::Text debugText;
public:
    DebugView(sf::RenderWindow *window_, const FpsMonitor &fpsMonitor_);
    void recalculateView();
    virtual void draw();
};
