#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include "Coord.h"
#include "utils.h"

#include "DUGraph.h"

/**
 * @brief Point of interest
 * 
 */
class point_t {
private:
    std::string name = "";
    Coord coord = Coord();
public:
    /**
     * @brief Construct without arguments
     * 
     */
    point_t();

    void setName(const std::string &s);
    void setCoord(const Coord &c);

    std::string getName() const;
    Coord getCoord() const;

};

#endif //POINT_H_INCLUDED