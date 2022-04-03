#pragma once

#include "VoronoiDecomposition.h"

#include "RBTree.h"

#include <map>
#include <set>

class SlabDecompositionRB : public VoronoiDecomposition {
private:
    struct cmp_t {
        coord_t::deg_t x;
        bool operator()(const Edge *lhs, const Edge *rhs) const {
            return lhs->evaluateY(x) < rhs->evaluateY(x);
        }
    };

    struct Event {
        bool start;
        const Edge *e;
    };

    std::vector<const Edge*> edges;
    std::map<coord_t::deg_t, std::list<Event>> events;
    std::map<coord_t::deg_t, RBTree<const Edge*>> slabs;
public:
    void initialize(const std::list<Edge> &edges);
    void run();
    Vector2 getClosestPoint(Vector2 p) const;
};
