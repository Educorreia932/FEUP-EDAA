#ifndef CLOSESTPOINT_H_INCLUDED
#define CLOSESTPOINT_H_INCLUDED

#include <list>
#include "coord.h"

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
    virtual void initialize(const std::list<coord_t> &points) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    virtual coord_t getClosestPoint(coord_t p) const = 0;
};

#endif //CLOSESTPOINT_H_INCLUDED
