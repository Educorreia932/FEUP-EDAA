#include "WindowClustersController.h"

#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

WindowClustersController::WindowClustersController(DraggableZoomableWindow& window, ClustersView& view) : window(window), view(view) {

}

void WindowClustersController::run() {
    window.recalculateView();

    while (window.isOpen()) {
        // Update/Refresh frequency
        sf::Time delta_time = sf::milliseconds(100);

        elapsed_time += r.restart();

        if (elapsed_time >= delta_time){
            // Refresh view
            view.refresh();

            // Substract the time consumed
            elapsed_time -= delta_time;
        }

        Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed: 
                    window.close(); 
                    
                    break;
                case Event::Resized: 
                    window.onResize(); 

                    break;
                case Event::MouseWheelScrolled: 
                    window.onScroll(event.mouseWheelScroll.delta); 
                    break;
                case Event::MouseButtonPressed:
                    switch (event.mouseButton.button) {
                        case Mouse::Button::Left:
                            window.onLeftPress(event);
                            break;
                        default: 
                            break;
                    }
                    break;
                case Event::MouseButtonReleased:
                    switch (event.mouseButton.button) {
                        case Mouse::Button::Left:
                            window.onLeftRelease();
                            break;
                        default: 
                        break;
                    }

                    break;
                case Event::MouseMoved:
                    window.onMouseMoved(event);

                    break;
                case Event::TextEntered:
                    switch (toupper((int)event.text.unicode)) {
                        case 'D': 
                            window.setDebugMode(!window.getDebugMode()); 
                            break;
                        default: 
                            break;
                    }
                    break;
                default: 
                    break;
            }
        }

        window.draw();
        window.display();
    }
}
