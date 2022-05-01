#include "MapView.h"

#include <cmath>

using namespace sf;

#define DEG_TO_RAD (M_PI/180.0)

Vector2f MapView::wsg84ToWebMercator(const Coord &c){
    return Vector2f(
        c.lon()/360.0,
        0.5 - log(tan(M_PI/4.0 + (c.lat()*DEG_TO_RAD)/2.0))/(2.0*M_PI)
    );
}

MapView::MapView(Coord c0_, double scale_):c0(c0_),scale(scale_){}

void MapView::addView(View *view){
    views.push_back(view);
}

Vector2f MapView::coordToVector2f(const Coord &coord) const{
    Vector2f ret = (wsg84ToWebMercator(coord) - wsg84ToWebMercator(c0))*float(scale);
    return ret;
}

void MapView::refresh(){
    for(View *view: views)
        view->refresh();
}

void MapView::draw(){
    for(View *view: views)
        view->draw();
}
