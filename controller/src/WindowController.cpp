#include "WindowController.h"

#include <cmath>

#include <iostream>

using namespace sf;
using namespace std;

WindowController::WindowController(WindowView &windowView_): windowView(windowView_){}

void WindowController::run(){
    windowView.recalculateView();
    RenderWindow *window = windowView.getWindow();
    while (window->isOpen()){
        Event event{};
        while (window->pollEvent(event)){
            switch(event.type){
                case Event::Closed            : window->close(); break;
                case Event::Resized           : windowView.onResize(); break;
                case Event::MouseWheelScrolled: windowView.onScroll(event.mouseWheelScroll.delta); break;
                case Event::MouseButtonPressed:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            windowView.onLeftPress(event);
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseButtonReleased:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            windowView.onLeftRelease();
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseMoved:
                    windowView.onMouseMoved(event);
                    break;
                case Event::TextEntered:
                    switch(toupper((int) event.text.unicode)){
                        case 'D': windowView.setDebugMode(!windowView.getDebugMode()); break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }
        windowView.draw();
        window->display();
    }
}
