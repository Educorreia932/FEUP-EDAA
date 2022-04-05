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
    std::vector<Vector2> c;
    std::vector<double> split;

    void search(const Vector2 &p, size_t r, Vector2 &cbest, double &dbest) const;
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
    void initialize(const std::list<Vector2> &points);

    /**
     * @brief Executes the algorithm
     * 
     */
    void run();

    Vector2 getClosestPoint(const Vector2 p) const;
};
