#pragma once

#include <list>
#include "Vector2.h"

class ClosestPointsInRadius {
public:
    virtual ~ClosestPointsInRadius();

    /**
     * @brief Initializes data members
     * 
     * @param points List of provided Points
     */
    virtual void initialize(const std::list<Vector2> &points, double d) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    virtual std::vector<Vector2> getClosestPoints(Vector2 p) const = 0;
};
