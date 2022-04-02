#include <catch2/catch_all.hpp>

#include <bits/stdc++.h>

#include "VoronoiDiagram.h"
#include "SlabDecomposition.h"
#include "SlabDecompositionRB.h"

using namespace std;

TEST_CASE("Slab decomposition", "[slab-1]"){
    unordered_map<string, Site> sites = {
        {"A", {{5,10}}},
        {"B", {{9,10}}},
        {"C", {{5,6}}},
        {"D", {{1,2}}},
        {"E", {{5,2}}},
    };
    list<Edge> edges;
    Edge e = Edge({0,0},{1,1},{-1,-1});
    e.start = { 0,12}; e.end = { 7,12}; e.site_up = nullptr       ; e.site_down = &sites.at("A"); edges.push_back(e);
    e.start = { 0,12}; e.end = { 0, 8}; e.site_up = &sites.at("A"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = { 0, 8}; e.end = { 7, 8}; e.site_up = &sites.at("A"); e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = { 7,12}; e.end = { 7, 8}; e.site_up = &sites.at("B"); e.site_down = &sites.at("A"); edges.push_back(e);
    e.start = { 7,12}; e.end = {10,12}; e.site_up = nullptr       ; e.site_down = &sites.at("B"); edges.push_back(e);
    e.start = {10,12}; e.end = {10, 5}; e.site_up = nullptr       ; e.site_down = &sites.at("B"); edges.push_back(e);
    e.start = { 7, 8}; e.end = {10, 5}; e.site_up = &sites.at("B"); e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = {10, 4}; e.end = {10, 5}; e.site_up = nullptr       ; e.site_down = &sites.at("C"); edges.push_back(e);
    e.start = { 3, 4}; e.end = {10, 4}; e.site_up = &sites.at("C"); e.site_down = &sites.at("E"); edges.push_back(e);
    e.start = { 3, 0}; e.end = { 3, 4}; e.site_up = &sites.at("E"); e.site_down = &sites.at("D"); edges.push_back(e);
    e.start = { 3, 0}; e.end = {10, 0}; e.site_up = &sites.at("E"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = {10, 0}; e.end = {10, 4}; e.site_up = nullptr       ; e.site_down = &sites.at("E"); edges.push_back(e);
    e.start = { 0, 0}; e.end = { 3, 0}; e.site_up = &sites.at("D"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = { 0, 0}; e.end = { 0, 7}; e.site_up = &sites.at("D"); e.site_down = nullptr       ; edges.push_back(e);
    e.start = { 0, 7}; e.end = { 3, 4}; e.site_up = &sites.at("C"); e.site_down = &sites.at("D"); edges.push_back(e);
    e.start = { 0, 7}; e.end = { 0, 8}; e.site_up = &sites.at("C"); e.site_down = nullptr       ; edges.push_back(e);

    SlabDecomposition s;
    s.initialize(edges);
    s.run();

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

    REQUIRE(s.getClosestPoint({1  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({2  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({3  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({4  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({5  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6  ,11}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6.9,11}) == sites.at("A").point); REQUIRE(s.getClosestPoint({7.1,11}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({8  ,11}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  ,11}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({2  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({3  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({4  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({5  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6  ,10}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6.9,10}) == sites.at("A").point); REQUIRE(s.getClosestPoint({7.1,10}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({8  ,10}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  ,10}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({2  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({3  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({4  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({5  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6  , 9}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6.9, 9}) == sites.at("A").point); REQUIRE(s.getClosestPoint({7.1, 9}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({8  , 9}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  , 9}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({2  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({3  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({4  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({5  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6  , 8.1}) == sites.at("A").point);
    REQUIRE(s.getClosestPoint({6.9, 8.1}) == sites.at("A").point); REQUIRE(s.getClosestPoint({7.1, 8.1}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({8  , 8.1}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  , 8.1}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({2  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({3  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({4  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({5  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({6  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({7  , 7.9}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({8  , 7.9}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  , 7.9}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({2  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({3  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({4  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({5  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({6  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({7  , 7  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({8  , 6.9}) == sites.at("C").point); REQUIRE(s.getClosestPoint({8  , 7.1}) == sites.at("B").point);
    REQUIRE(s.getClosestPoint({9  , 7  }) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 5.9}) == sites.at("D").point); REQUIRE(s.getClosestPoint({1  , 6.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({2  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({3  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({4  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({5  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({6  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({7  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({8  , 6  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({9  , 5.9}) == sites.at("C").point); REQUIRE(s.getClosestPoint({9  , 6.1}) == sites.at("B").point);

    REQUIRE(s.getClosestPoint({1  , 5  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 4.9}) == sites.at("D").point); REQUIRE(s.getClosestPoint({2  , 5.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({3  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({4  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({5  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({6  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({7  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({8  , 5  }) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({9  , 5  }) == sites.at("C").point);

    REQUIRE(s.getClosestPoint({1  , 4.1}) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 4.1}) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({3  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({4  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({5  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({6  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({7  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({8  , 4.1}) == sites.at("C").point);
    REQUIRE(s.getClosestPoint({9  , 4.1}) == sites.at("C").point);

    REQUIRE(s.getClosestPoint({1  , 3.9}) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 3.9}) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2.9, 3.9}) == sites.at("D").point); REQUIRE(s.getClosestPoint({3.1, 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({4  , 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({5  , 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({6  , 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({7  , 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({8  , 3.9}) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({9  , 3.9}) == sites.at("E").point);

    REQUIRE(s.getClosestPoint({1  , 3  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 3  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2.9, 3  }) == sites.at("D").point); REQUIRE(s.getClosestPoint({3.1, 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({4  , 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({5  , 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({6  , 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({7  , 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({8  , 3  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({9  , 3  }) == sites.at("E").point);

    REQUIRE(s.getClosestPoint({1  , 2  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 2  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2.9, 2  }) == sites.at("D").point); REQUIRE(s.getClosestPoint({3.1, 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({4  , 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({5  , 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({6  , 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({7  , 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({8  , 2  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({9  , 2  }) == sites.at("E").point);

    REQUIRE(s.getClosestPoint({1  , 1  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2  , 1  }) == sites.at("D").point);
    REQUIRE(s.getClosestPoint({2.9, 1  }) == sites.at("D").point); REQUIRE(s.getClosestPoint({3.1, 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({4  , 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({5  , 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({6  , 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({7  , 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({8  , 1  }) == sites.at("E").point);
    REQUIRE(s.getClosestPoint({9  , 1  }) == sites.at("E").point);
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
