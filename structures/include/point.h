#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include "coord.h"
#include "utils.h"

#include "DUGraph.h"

/**
 * @brief Point of interest
 * 
 */
class point_t {
private:
    std::string name = "";
    coord_t coord = coord_t();
public:
    /**
     * @brief Construct without arguments
     * 
     */
    point_t();

    void setName(const std::string &s);
    void setCoord(const coord_t &c);

    std::string getName() const;
    coord_t getCoord() const;

};

#endif //POINT_H_INCLUDED