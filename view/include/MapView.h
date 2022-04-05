#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <list>

#include "Coord.h"

class MapView: public View {
private:
    Coord c0;
    double scale;
    std::list<View*> views;
public:
    static sf::Vector2f wsg84ToWebMercator(const Coord &c);

    MapView(Coord c0_, double scale_);
    
    void addView(View *view);

    sf::Vector2f coordToVector2f(const Coord &coord) const;

    virtual void refresh();

    virtual void draw();
};
