#ifndef QuadTreeClosestPoint
#define QuadTreeClosestPoint

#include "ClosestPoint.h"

#include <vector>
#include <set>

/**
 * @brief VStripes algorithm for Closest Point
 * 
 */
class QuadTreeClosestPoint: public ClosestPoint{
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

#endif //QuadTreeClosestPoint
