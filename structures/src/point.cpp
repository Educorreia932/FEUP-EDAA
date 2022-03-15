#include "point.h"

point_t::point_t(){}

void point_t::setName(const std::string &s){
    name = s;
}

void point_t::setCoord(const coord_t &c){
    coord = c;
}

std::string point_t::getName() const {
    return name;
}

coord_t point_t::getCoord() const {
    return coord;
}
