#include "Arc.h"

Arc::Arc(Site* site) : site(site) {

};

Arc::Arc(Site* site, Arc* previous, Arc* next) : previous(previous), next(next), site(site) {

};