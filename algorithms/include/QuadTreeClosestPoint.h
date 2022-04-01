#pragma once

#include "ClosestPoint.h"

#include <vector>
#include <set>

/**
 * @brief VStripes algorithm for Closest Point
 * 
 */
class QuadTreeClosestPoint: public ClosestPoint{
private:
    std::vector<coord_t> c;
    std::vector<coord_t::deg_t> split;

    void search(const coord_t &p, size_t r, coord_t &cbest, coord_t::deg_t &dbest) const;
public:
    /**
     * @brief Construct from degrees
     * 
     * @param Delta value in degress
     */
    QuadTreeClosestPoint();

    /**
     * @brief Initializes data members
     * 
     * @param points List of provided Points
     */
    void initialize(const std::list<coord_t> &points);

    /**
     * @brief Executes the algorithm
     * 
     */
    void run();

    coord_t getClosestPoint(const coord_t p) const;
};
