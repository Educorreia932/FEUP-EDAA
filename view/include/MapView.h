#pragma once

#include "View.h"

#include <SFML/Graphics.hpp>
#include <list>

#include "coord.h"

class MapView: public View {
private:
    coord_t c0;
    double scale;
    std::list<View*> views;
public:
    MapView(coord_t c0_, double scale_);
    
    void addView(View *view);

    sf::Vector2f coordToVector2f(const coord_t &coord) const;

    virtual void draw();
};
