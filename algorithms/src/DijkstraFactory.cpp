#include "DijkstraFactory.h"

ShortestPathOneMany *DijkstraFactory::factoryMethod(){
    return new Dijkstra();
}
