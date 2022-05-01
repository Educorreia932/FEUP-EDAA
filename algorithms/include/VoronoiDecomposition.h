#pragma once

#include <list>
#include "Coord.h"
#include "VoronoiDiagram.h"

class VoronoiDecomposition {
public:
    virtual ~VoronoiDecomposition();

    /**
     * @brief Initializes data members
     * 
     * @param points List of provided edges
     */
    virtual void initialize(const std::list<Edge> &edges) = 0;

    /**
     * @brief Executes the algorithm
     * 
     */
    virtual void run() = 0;

    virtual Vector2 getClosestPoint(Vector2 p) const = 0;
};
