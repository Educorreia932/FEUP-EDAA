#include "WindowController.h"

#include <cmath>

#include <iostream>

using namespace sf;
using namespace std;

WindowController::WindowController(DraggableZoomableWindow &window_): window(window_){}

void WindowController::run(){
    window.recalculateView();
    while (window.isOpen()){
        Event event{};
        while (window.pollEvent(event)){
            switch(event.type){
                case Event::Closed            : window.close(); break;
                case Event::Resized           : window.onResize(); break;
                case Event::MouseWheelScrolled: window.onScroll(event.mouseWheelScroll.delta); break;
                case Event::MouseButtonPressed:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            window.onLeftPress(event);
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseButtonReleased:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            window.onLeftRelease();
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseMoved:
                    window.onMouseMoved(event);
                    break;
                case Event::TextEntered:
                    switch(toupper((int) event.text.unicode)){
                        case 'D': window.setDebugMode(!window.getDebugMode()); break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }
        window.draw();
        window.display();
    }
}
