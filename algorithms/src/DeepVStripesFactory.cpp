#include "DeepVStripesFactory.h"

DeepVStripesFactory::DeepVStripesFactory(double delta_, size_t L_):
delta(delta_), L(L_)
{}

ClosestPoint *DeepVStripesFactory::factoryMethod(){
    return new DeepVStripes(delta, L);
}
