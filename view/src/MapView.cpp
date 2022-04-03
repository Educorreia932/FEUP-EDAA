#include "MapView.h"

using namespace sf;

MapView::MapView(coord_t c0_, double scale_):c0(c0_),scale(scale_){}

void MapView::addView(View *view){
    views.push_back(view);
}

Vector2f MapView::coordToVector2f(const coord_t &coord) const{
    return Vector2f(
        +(coord.lon - c0.lon) * scale,
        -(coord.lat - c0.lat) * scale
    );
}

void MapView::draw(){
    for(View *view: views)
        view->draw();
}
