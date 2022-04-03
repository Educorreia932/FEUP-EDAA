#pragma once

#include "View.h"
#include "DebugView.h"
#include "FpsMonitor.h"

#include <SFML/Graphics.hpp>

class WindowView : public View {
public:
    static const int DEFAULT_WIDTH  = 800;
    static const int DEFAULT_HEIGHT = 600;
private:
    sf::RenderWindow *window = nullptr;     ///< @brief Window.
    sf::View *view = nullptr;               ///< @brief Default view, to draw the graph.

    float scale = 1.0;                          ///< @brief Scale (changed with scroll).
    /**
     * @brief Scale factor.
     * 
     * Scale is updated after a scroll of delta by calculating
     * 
     * @code{.cpp}
     * scale = scale * pow(SCALE_DELTA, -delta);
     * @endcode
     */
    static constexpr float SCALE_DELTA = 1.5;
    sf::Vector2f center;                        ///< @brief Coordinates of center of the window.

    bool debug_mode = false;                    ///< @brief True if debug mode is enabled, false otherwise.
    DebugView *debugView = nullptr;

    View *v = nullptr;

    bool isLeftClickPressed = false;
    sf::Vector2f centerInitial;
    sf::Vector2f posMouseInitial;

    sf::Color background_color = sf::Color::White; ///< @brief Background color.

    FpsMonitor fpsMonitor = FpsMonitor(1000);  ///< @brief FPS monitor.
public:
    WindowView(sf::Vector2f center_);

    void setView(View *view);

    void draw();
    
    void onResize();
    void onScroll(float delta);
    void recalculateView();

    sf::RenderWindow *getWindow();
    bool getDebugMode() const;
    void setDebugMode(bool b);

    void onLeftPress(const sf::Event &event);
    void onLeftRelease();
    void onMouseMoved(const sf::Event &event);
};
