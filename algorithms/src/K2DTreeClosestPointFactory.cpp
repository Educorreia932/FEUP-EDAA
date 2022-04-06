#include "K2DTreeClosestPointFactory.h"

ClosestPoint *K2DTreeClosestPointFactory::factoryMethod(){
    return new K2DTreeClosestPoint();
}
