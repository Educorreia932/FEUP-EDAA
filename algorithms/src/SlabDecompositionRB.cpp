#include "SlabDecompositionRB.h"

#include <algorithm>

using namespace std;

typedef coord_t::deg_t deg_t;

void SlabDecompositionRB::initialize(const std::list<Edge> &es){
    edges.clear();
    auto it = es.begin();
    while(it != es.end()) edges.push_back(&*(it++));
}

struct cmp_t {
    deg_t x;
    bool operator()(const Edge *lhs, const Edge *rhs) const {
        return lhs->evaluateY(x) < rhs->evaluateY(x);
    }
};

void SlabDecompositionRB::run(){
    // map<deg_t, list<Event>> events;
    for(const Edge *e: edges){
        deg_t xl = e->start.x;
        deg_t xr = e->end.x;
        if(xl == xr) continue;
        if(xl > xr) swap(xl, xr);

        events[xl].push_back({true , e});
        events[xr].push_back({false, e});
    }

    // map<deg_t, set<const Edge*, cmp_t>> slabs;
    RBTree<const Edge*> prev;

    auto it1 = events.begin(),
         it2 = ++events.begin();
    while(it2 != events.end()){
        deg_t xl = it1->first,
              xr = it2->first;
        deg_t xm = xl + (xr-xl)/2.0;

        RBTree<const Edge*> &cur = slabs[xl] = prev;
        for(const Event &ev: it1->second) if(!ev.start) cur = cur.remove(ev.e, cmp_t{xm});
        for(const Event &ev: it1->second) if( ev.start) cur = cur.insert(ev.e, cmp_t{xm});

        ++it1; ++it2; prev = cur;
    }

}

Vector2 SlabDecompositionRB::getClosestPoint(Vector2 p) const {
    auto it1 = slabs.lower_bound(p.x);
    if(it1 == slabs.begin()) throw runtime_error("x-coordinate too low");
    --it1;

    double x = p.x;
    auto slab = it1->second;
    auto it2 = slab.lower_bound(p.y, [x](const Edge *e, double y){
        return e->evaluateY(x) < y;
    });
    if(it2 == nullptr) throw runtime_error("y-coordinate too high");

    const Edge *eRet = *it2;
    if(eRet->site_down == nullptr) throw runtime_error("y-coordinate too low");

    return eRet->site_down->point;
}
