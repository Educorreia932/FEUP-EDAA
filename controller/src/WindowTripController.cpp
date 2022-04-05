#include "WindowTripController.h"

#include <cmath>

#include <iostream>

using namespace sf;
using namespace std;

WindowTripController::WindowTripController(
    DraggableZoomableWindow &window_,
    MapTripMatchView &mapTripMatchView_,
    const vector<Trip> &trips_,
    const ClosestPoint &closestPoint_
): window(window_), mapTripMatchView(mapTripMatchView_), trips(trips_), closestPoint(closestPoint_) {}

void WindowTripController::run(){
    window.recalculateView();
    onChangeTrip();
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
                        case 'R':
                            tripIndex = rand()%trips.size();
                            onChangeTrip();
                            break;
                        default: break;
                    }
                    break;
                case Event::KeyReleased:
                    switch(event.key.code){
                        case Keyboard::Right:
                            tripIndex = (tripIndex+trips.size() + 1 )%trips.size();
                            onChangeTrip();
                            break;
                        case Keyboard::Left:
                            tripIndex = (tripIndex+trips.size() - 1)%trips.size();
                            onChangeTrip();
                            break;
                    }
                    break;
                default: break;
            }
        }
        window.draw();
        window.display();
    }
}

void WindowTripController::onChangeTrip(){
    const Trip &trip = trips.at(tripIndex);
    const std::vector<Coord> &coords = trip.coords;
    currentMatches.resize(coords.size());
    for(size_t i = 0; i < coords.size(); ++i){
        currentMatches.at(i) = Coord(closestPoint.getClosestPoint(coords.at(i)));
    }

    mapTripMatchView.setTripMatches(trip, currentMatches);
    mapTripMatchView.refresh();
}
