#include "Arc.h"

Arc::Arc(Site* site) : site(site) {

}

Arc::Arc(Site* site, Arc* left, Arc* right) : left(left), right(right), site(site) {

}
