#pragma once

#include "Dijkstra.h"

class DijkstraFactory : public ShortestPathOneManyFactory {
public:
    virtual ShortestPathOneMany *factoryMethod();
};
