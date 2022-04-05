#include <catch2/catch_all.hpp>

#include <bits/stdc++.h>

#include "VoronoiDiagram.h"
#include "SlabDecomposition.h"
#include "SlabDecompositionRB.h"

using namespace std;

unordered_map<string, Site> sites;
list<Edge> edges;

void initializeVoronoiDecomposition(VoronoiDecomposition &s){
    sites.clear();
    sites = {
        {"A", {Vector2(5,10)}},
        {"B", {Vector2(9,10)}},
        {"C", {Vector2(5,6)}},
        {"D", {Vector2(1,2)}},
        {"E", {Vector2(5,2)}},
    };
    
    edges.clear();
    Edge e = Edge(Vector2(0,0),Vector2(1,1),Vector2(-1,-1));
    e.start = Vector2( 0,12); e.end = Vector2( 7,12); e.site_up = nullptr       ; e.site_down = &sites.at("A"); edges.push_back(e);
    e.start = Vector2( 0,12); e.end = Vector2( 0, 8); e.site_up = &sites.at("A"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = Vector2( 0, 8); e.end = Vector2( 7, 8); e.site_up = &sites.at("A"); e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = Vector2( 7,12); e.end = Vector2( 7, 8); e.site_up = &sites.at("B"); e.site_down = &sites.at("A"); edges.push_back(e);
    e.start = Vector2( 7,12); e.end = Vector2(10,12); e.site_up = nullptr       ; e.site_down = &sites.at("B"); edges.push_back(e);
    e.start = Vector2(10,12); e.end = Vector2(10, 5); e.site_up = nullptr       ; e.site_down = &sites.at("B"); edges.push_back(e);
    e.start = Vector2( 7, 8); e.end = Vector2(10, 5); e.site_up = &sites.at("B"); e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = Vector2(10, 4); e.end = Vector2(10, 5); e.site_up = nullptr       ; e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = Vector2( 3, 4); e.end = Vector2(10, 4); e.site_up = &sites.at("C"); e.site_down = &sites.at("E"); edges.push_back(e);
    e.start = Vector2( 3, 0); e.end = Vector2( 3, 4); e.site_up = &sites.at("E"); e.site_down = &sites.at("D"); edges.push_back(e);
    e.start = Vector2( 3, 0); e.end = Vector2(10, 0); e.site_up = &sites.at("E"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = Vector2(10, 0); e.end = Vector2(10, 4); e.site_up = nullptr       ; e.site_down = &sites.at("E"); edges.push_back(e);
    e.start = Vector2( 0, 0); e.end = Vector2( 3, 0); e.site_up = &sites.at("D"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = Vector2( 0, 0); e.end = Vector2( 0, 7); e.site_up = &sites.at("D"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = Vector2( 0, 7); e.end = Vector2( 3, 4); e.site_up = &sites.at("C"); e.site_down = &sites.at("D"); edges.push_back(e);
    e.start = Vector2( 0, 7); e.end = Vector2( 0, 8); e.site_up = &sites.at("C"); e.site_down = nullptr       ; edges.push_back(e);

    s.initialize(edges);
    s.run();
}

void evaluateVoronoiDecomposition(const VoronoiDecomposition &s){
    REQUIRE(s.getClosestPoint(Vector2(1  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(2  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(3  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(4  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(5  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6  ,11)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6.9,11)) == sites.at("A").point); REQUIRE(s.getClosestPoint(Vector2(7.1,11)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(8  ,11)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  ,11)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(2  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(3  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(4  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(5  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6  ,10)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6.9,10)) == sites.at("A").point); REQUIRE(s.getClosestPoint(Vector2(7.1,10)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(8  ,10)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  ,10)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 9)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6.9, 9)) == sites.at("A").point); REQUIRE(s.getClosestPoint(Vector2(7.1, 9)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 9)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 9)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 8.1)) == sites.at("A").point);
    REQUIRE(s.getClosestPoint(Vector2(6.9, 8.1)) == sites.at("A").point); REQUIRE(s.getClosestPoint(Vector2(7.1, 8.1)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 8.1)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 8.1)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 7.9)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 7.9)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 7.9)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 7  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 6.9)) == sites.at("C").point); REQUIRE(s.getClosestPoint(Vector2(8  , 7.1)) == sites.at("B").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 7  )) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 5.9)) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(1  , 6.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 6  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 5.9)) == sites.at("C").point); REQUIRE(s.getClosestPoint(Vector2(9  , 6.1)) == sites.at("B").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 5  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 4.9)) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(2  , 5.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 5  )) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 5  )) == sites.at("C").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 4.1)) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 4.1)) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(3  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 4.1)) == sites.at("C").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 4.1)) == sites.at("C").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 3.9)) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 3.9)) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2.9, 3.9)) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(3.1, 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 3.9)) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 3.9)) == sites.at("E").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 3  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 3  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2.9, 3  )) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(3.1, 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 3  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 3  )) == sites.at("E").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 2  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 2  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2.9, 2  )) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(3.1, 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 2  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 2  )) == sites.at("E").point);

    REQUIRE(s.getClosestPoint(Vector2(1  , 1  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2  , 1  )) == sites.at("D").point);
    REQUIRE(s.getClosestPoint(Vector2(2.9, 1  )) == sites.at("D").point); REQUIRE(s.getClosestPoint(Vector2(3.1, 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(4  , 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(5  , 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(6  , 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(7  , 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(8  , 1  )) == sites.at("E").point);
    REQUIRE(s.getClosestPoint(Vector2(9  , 1  )) == sites.at("E").point);
}

TEST_CASE("Slab decomposition", "[slab-1]"){
    
    SlabDecomposition s;
    initializeVoronoiDecomposition(s);

    // for(const auto &p: s.events){
    //     std::cout << "x = " << p.first << std::endl;
    //     for(const SlabDecomposition::Event &ev: p.second){
    //         std::cout << "    " << ev.start << " "
    //                   << "(" << ev.e->start.x << "," << ev.e->start.y << ")" << "--"
    //                   << "(" << ev.e->end  .x << "," << ev.e->end  .y << ")" << std::endl;
    //     }
    // }

    // std::cout << "\n\n";

    // for(const auto &p: s.slabs){
    //     std::cout << "xl = " << p.first << std::endl;
    //     for(const Edge *e: p.second){
    //         std::cout << "    " << "(" << e->start.x << "," << e->start.y << ")"
    //                   << "--"   << "(" << e->end  .x << "," << e->end  .y << ")" << std::endl;
    //     }
    // }

    evaluateVoronoiDecomposition(s);
}

TEST_CASE("Persistent Red-Black Tree", "[persistent-rbtree-1]"){
    RBTree<int> t0;
    auto t1 = t0.insert(0);
    auto t2 = t1.insert(1);
    auto t3 = t2.remove(0);

    REQUIRE( t0.lower_bound(-1) == nullptr);
    REQUIRE( t0.lower_bound( 0) == nullptr);
    REQUIRE( t0.lower_bound(+1) == nullptr);
    REQUIRE( t0.lower_bound(+2) == nullptr);

    REQUIRE(*t1.lower_bound(-1) == 0);
    REQUIRE(*t1.lower_bound( 0) == 0);
    REQUIRE( t1.lower_bound(+1) == nullptr);
    REQUIRE( t1.lower_bound(+2) == nullptr);

    REQUIRE(*t2.lower_bound(-1) == 0);
    REQUIRE(*t2.lower_bound( 0) == 0);
    REQUIRE(*t2.lower_bound(+1) == 1);
    REQUIRE( t2.lower_bound(+2) == nullptr);

    REQUIRE(*t3.lower_bound(-1) == 1);
    REQUIRE(*t3.lower_bound( 0) == 1);
    REQUIRE(*t3.lower_bound(+1) == 1);
    REQUIRE( t3.lower_bound(+2) == nullptr);
}

TEST_CASE("Slab decomposition RB", "[slab-rb-1]"){
    SlabDecompositionRB s;
    initializeVoronoiDecomposition(s);
    evaluateVoronoiDecomposition(s);
}
