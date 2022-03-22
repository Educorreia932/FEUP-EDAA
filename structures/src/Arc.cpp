#include "Arc.h"

Arc::Arc(Site* site) : site(site) {

};

Arc::Arc(Site* site, Arc* previous, Arc* next) : site(site), previous(previous), next(next) {

};