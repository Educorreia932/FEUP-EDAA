#include "MapTripMatchView.h"

#include <mapbox/earcut.hpp>

#include "loadFont.h"

using namespace sf;
using namespace std;

typedef DWGraph::node_t node_t;
typedef DWGraph::weight_t weight_t;
typedef MapGraph::way_t way_t;

const sf::Font FONT = loadFont("/../../res/fonts/inconsolata.ttf");

const double CIRCLE_SIZE = 5;
const size_t CIRCLE_POINT_COUNT = 24;

MapTripMatchView::MapTripMatchView(RenderTarget &window_, MapView &mapView_):
    window(window_), mapView(mapView_),
    beginCircle(CIRCLE_SIZE, CIRCLE_POINT_COUNT), endCircle(CIRCLE_SIZE, CIRCLE_POINT_COUNT)
{
    beginCircle.setFillColor(Color::Green); beginCircle.setOrigin(CIRCLE_SIZE/2, CIRCLE_SIZE/2);
    endCircle  .setFillColor(Color::Red  ); endCircle  .setOrigin(CIRCLE_SIZE/2, CIRCLE_SIZE/2);
}

MapTripMatchView::MapTripMatchView(
    RenderTarget &window_, MapView &mapView_,
    const Trip &trip_, const vector<Coord> &matches_, const list<Coord> &path_
):
    window(window_), mapView(mapView_),
    trip(trip_), matches(matches_), path(path_),
    beginCircle(CIRCLE_SIZE, CIRCLE_POINT_COUNT), endCircle(CIRCLE_SIZE, CIRCLE_POINT_COUNT)
{
    beginCircle.setFillColor(Color::Green);  beginCircle.setOrigin(CIRCLE_SIZE/2, CIRCLE_SIZE/2);
    endCircle  .setFillColor(Color::Red  );  endCircle  .setOrigin(CIRCLE_SIZE/2, CIRCLE_SIZE/2);
    refresh();
}

void MapTripMatchView::setTripMatches(
    const Trip &trip_, const vector<Coord> &matches_,
    const list<Coord> &path_
){
    trip = trip_;
    matches = matches_;
    path = path_;
}

void MapTripMatchView::refresh(){
    zip.clear();

    texts.clear();
    int i = 0;
    for(const Coord &c: trip.coords){
        sf::Vector2f u = mapView.coordToVector2f(c);
        sf::Text t(sf::String(to_string(i++)), FONT, 10);
        t.setFillColor(Color::Black);
        t.setOrigin(-3, 6);
        t.setPosition(u);
        texts.push_back(t);
    }

    if(trip.coords.size() >= 2){
        for(
            auto it1 = trip.coords.begin(), it2 = ++trip.coords.begin();
            it2 != trip.coords.end();
            ++it1, ++it2
        ){
            sf::Vector2f u = mapView.coordToVector2f(*it1),
                         v = mapView.coordToVector2f(*it2);
            FullLineShape e(u, v, 2);
            e.setFillColor(Color::Blue);
            for(size_t i = 0; i < e.getVertexCount(); ++i)
                zip.push_back(e[i]);
        }
    }

    matchLines.clear();
    for(size_t i = 0; i < min(trip.coords.size(), matches.size()); ++i){
        sf::Vector2f u = mapView.coordToVector2f(trip.coords.at(i)),
                     v = mapView.coordToVector2f(matches.at(i));
        matchLines.push_back(Vertex(u, Color::Blue));
        matchLines.push_back(Vertex(v, Color::Blue));
    }

    if(path.size() >= 2){
        for(auto it1 = path.begin(), it2 = ++path.begin(); it2 != path.end(); ++it1, ++it2){
            sf::Vector2f u = mapView.coordToVector2f(*it1),
                         v = mapView.coordToVector2f(*it2);
            FullLineShape e(u, v, 1);
            e.setFillColor(Color::Magenta);
            for(size_t i = 0; i < e.getVertexCount(); ++i)
                zip.push_back(e[i]);
        }
    }

    beginCircle.setPosition(mapView.coordToVector2f(*trip.coords. begin()));
    endCircle  .setPosition(mapView.coordToVector2f(*trip.coords.rbegin()));

    circles.clear();
    for(const Coord &c: trip.coords){
        sf::Vector2f u = mapView.coordToVector2f(c);
        sf::CircleShape circ(2,3);
        circ.setOrigin(2,2);
        circ.setPosition(u);
        circ.setFillColor(Color::Red);
        circ.setOutlineColor(Color::Black);
        circ.setOutlineThickness(0.05);
        circles.push_back(circ);
    }
    for(const Coord &c: matches){
        sf::Vector2f u = mapView.coordToVector2f(c);
        sf::CircleShape circ(2,3);
        circ.setOrigin(2,2);
        circ.setPosition(u);
        circ.setFillColor(Color::Green);
        circ.setOutlineColor(Color::Black);
        circ.setOutlineThickness(0.05);
        circles.push_back(circ);
    }
}

void MapTripMatchView::draw(){
    window.draw(&zip[0], zip.size(), Quads);
    window.draw(beginCircle);
    window.draw(endCircle);
    for(sf::CircleShape &c: circles)
        window.draw(c);
    window.draw(&matchLines[0], matchLines.size(), Lines);
    for(const sf::Text &t: texts)
        window.draw(t);
}
