#pragma once

#include <list>
#include "Coord.h"

class ClosestPointsInRadius {
public:
    virtual ~ClosestPointsInRadius();

    /**
     * @brief Initializes data members
     * 
     * @param points List of provided Points
     */
    virtual void initialize(const std::list<Coord> &points, double d) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    virtual std::vector<Coord> getClosestPoints(Coord p) const = 0;
};
