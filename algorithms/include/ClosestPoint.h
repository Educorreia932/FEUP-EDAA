#pragma once

#include <list>
#include "Vector2.h"

/**
 * @brief Closest Point Class (find closest point to the give coordinates)
 * 
 */
class ClosestPoint {
public:
    virtual ~ClosestPoint();

    /**
     * @brief Initializes data members
     * 
     * @param points List of provided Points
     */
    virtual void initialize(const std::list<Vector2> &points) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    virtual Vector2 getClosestPoint(Vector2 p) const = 0;
};

class ClosestPointFactory {
public:
    virtual ClosestPoint *factoryMethod() = 0;
};
