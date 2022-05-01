#pragma once

#include "ClosestPoint.h"

#include "DeepVStripes.h"

class DeepVStripesFactory: public ClosestPointFactory {
private:
    const double delta;
    const size_t L;
public:
    DeepVStripesFactory(double delta_, size_t L_);
    virtual ClosestPoint *factoryMethod();
};
