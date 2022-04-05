#include "SlabDecomposition.h"

#include <algorithm>

using namespace std;

void SlabDecomposition::initialize(const std::list<Edge> &es){
    edges.clear();
    auto it = es.begin();
    while(it != es.end()) edges.push_back(&*(it++));
}

struct cmp_t {
    double x;
    bool operator()(const Edge *lhs, const Edge *rhs) const {
        return lhs->evaluateY(x) < rhs->evaluateY(x);
    }
};

void SlabDecomposition::run(){
    // map<double, list<Event>> events;
    for(const Edge *e: edges){
        double xl = e->start.x;
        double xr = e->end.x;
        if(xl == xr) continue;
        if(xl > xr) swap(xl, xr);

        events[xl].push_back({true , e});
        events[xr].push_back({false, e});
    }

    // map<double, set<const Edge*, cmp_t>> slabs;
    set<const Edge*, cmp_t> prev;

    auto it1 = events.begin(),
         it2 = ++events.begin();
    while(it2 != events.end()){
        double xl = it1->first,
              xr = it2->first;
        double xm = xl + (xr-xl)/2.0;

        set<const Edge*, cmp_t> cur = set<const Edge*, cmp_t>({xm});
        cur.insert(prev.begin(), prev.end());

        for(const Event &ev: it1->second) if(!ev.start) cur.erase(ev.e);
        for(const Event &ev: it1->second) if(ev.start) cur.insert(ev.e);

        auto &slab = slabs[xl] = vector<const Edge*>(cur.begin(), cur.end());
        sort(slab.begin(), slab.end(), cmp_t{xm});

        ++it1; ++it2; prev = cur;
    }

}

Vector2 SlabDecomposition::getClosestPoint(Vector2 p) const {
    auto it1 = slabs.lower_bound(p.x);
    if(it1 == slabs.begin()) throw runtime_error("x-coordinate too low");
    --it1;

    double x = p.x;
    auto slab = it1->second;
    auto it2 = lower_bound(slab.begin(), slab.end(), p.y, [x](const Edge *e, double y){
        return e->evaluateY(x) < y;
    });
    if(it2 == slab.end()) throw runtime_error("y-coordinate too high");

    const Edge *eRet = *it2;
    if(eRet->site_down == nullptr) throw runtime_error("y-coordinate too low");

    return eRet->site_down->point;
}
