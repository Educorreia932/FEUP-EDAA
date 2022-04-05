#include "WindowTripController.h"

#include <cmath>
#include <iostream>

#include "Astar.h"

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;

const double SECONDS_TO_MICROS = 1000000;
const double KPH_TO_MPS        = (1.0/3.6);

WindowTripController::WindowTripController(
    DraggableZoomableWindow &window_,
    MapTripMatchView &mapTripMatchView_,
    const MapGraph &mapGraph_,
    const DWGraph::DWGraph &graph_,
    const vector<Trip> &trips_,
    const ClosestPoint &closestPoint_
):
    window(window_),
    mapTripMatchView(mapTripMatchView_),
    mapGraph(mapGraph_),
    graph(graph_),
    trips(trips_),
    closestPoint(closestPoint_)
{}

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

    list<Coord> pathCoord;
    pathCoord.push_back(currentMatches[0]);

    
    for(size_t i = 0; i+1 < currentMatches.size(); ++i){
        const Coord &uCoord = currentMatches[i];
        const Coord &vCoord = currentMatches[i+1];
        node_t u = mapGraph.coordToNode(uCoord);
        node_t v = mapGraph.coordToNode(vCoord);

        MapGraph::DistanceHeuristic heuristic(mapGraph.getNodes(), vCoord, double(SECONDS_TO_MICROS)/(120.0*KPH_TO_MPS));
        Astar astar(&heuristic);
        astar.initialize(&graph, u, v);
        astar.run();
        
        
        std::list<node_t> path = astar.getPath();
        for(auto it = ++path.begin(); it != path.end(); ++it)
            pathCoord.push_back(mapGraph.nodeToCoord(*it));
    }

    mapTripMatchView.setTripMatches(trip, currentMatches, pathCoord);
    mapTripMatchView.refresh();
}
