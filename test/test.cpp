#include <catch2/catch_all.hpp>

#include <bits/stdc++.h>

#include "QuadTreeClosestPoint.h"
#include "DeepVStripes.h"
#include "VoronoiDiagram.h"
#include "SlabDecomposition.h"
#include "SlabDecompositionRB.h"

using namespace std;

bool findClosestBruteForce_exact(const list<coord_t> &l, const coord_t &u, coord_t &cbest){
    int dbest = 1000000000;
    bool good_example = true;
    for(const coord_t &c: l){
        int dx = c.getLon() - u.getLon();
        int dy = c.getLat() - u.getLat();
        int d = dx*dx + dy*dy;
        if(d < dbest){
            dbest = d;
            cbest = c;
            good_example = true;
        } else if(d == dbest){
            good_example = false;
        }
    }
    return good_example;
}

coord_t findClosestBruteForce(const list<coord_t> &l, const coord_t &u){
    coord_t::deg_t dbest = 1000000000.0L;
    coord_t cbest;
    for(const coord_t &c: l){
        coord_t::deg_t dx = c.getLon() - u.getLon();
        coord_t::deg_t dy = c.getLat() - u.getLat();
        coord_t::deg_t d = dx*dx + dy*dy;
        if(d < dbest){
            dbest = d;
            cbest = c;
        }
    }
    return cbest;
}

TEST_CASE("Quad Tree", "[quadtree]"){
    QuadTreeClosestPoint q;
    q.initialize(list<coord_t>({
        coord_t(1, 9),
        coord_t(4, 7),
        coord_t(8, 3),
        coord_t(4, 5),
        coord_t(2, 9),
        coord_t(7, 3),
        coord_t(8, 8),
        coord_t(1, 7)
    })); //(Lat, Lon), or (y, x)
    q.run();

    REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
}

TEST_CASE("Quad Tree 2", "[quadtree-2]"){
    list<coord_t> l({
        coord_t(1, 9),
        coord_t(4, 7),
        coord_t(8, 3),
        coord_t(4, 5),
        coord_t(2, 9),
        coord_t(7, 3),
        coord_t(8, 8),
        coord_t(1, 7)
    });

    QuadTreeClosestPoint q;
    q.initialize(l);
    q.run();

    for(int x = 0; x <= 10; ++x){
        for(int y = 0; y <= 10; ++y){
            coord_t u(y, x);
            coord_t cbest;
            bool good_example = findClosestBruteForce_exact(l, u, cbest);
            if(good_example)
                REQUIRE(q.getClosestPoint(u) == cbest);
        }
    }
}

TEST_CASE("Quad Tree 3", "[quadtree-3]"){
    const size_t N = 1000, M = 1000;
    list<coord_t> l;
    for(size_t i = 0; i < N; ++i){
        l.push_back(coord_t(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        ));
    }

    QuadTreeClosestPoint q;
    q.initialize(l);
    q.run();

    for(size_t i = 0; i < M; ++i){
        coord_t u(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        );

        REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
    }
}

TEST_CASE("VStripes", "[vstripes]"){
    VStripes q(2.0);
    q.initialize(list<coord_t>({
        coord_t(1, 9),
        coord_t(4, 7),
        coord_t(8, 3),
        coord_t(4, 5),
        coord_t(2, 9),
        coord_t(7, 3),
        coord_t(8, 8),
        coord_t(1, 7)
    })); //(Lat, Lon), or (y, x)
    q.run();

    REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
}

TEST_CASE("DeepVStripes", "[deepvstripes]"){
    DeepVStripes q(1.0, 5);
    q.initialize(list<coord_t>({
        coord_t(1, 9),
        coord_t(4, 7),
        coord_t(8, 3),
        coord_t(4, 5),
        coord_t(2, 9),
        coord_t(7, 3),
        coord_t(8, 8),
        coord_t(1, 7)
    })); //(Lat, Lon), or (y, x)
    q.run();

    REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
}

TEST_CASE("DeepVStripes 2", "[deepvstripes-2]"){
    list<coord_t> l({
        coord_t(1, 9),
        coord_t(4, 7),
        coord_t(8, 3),
        coord_t(4, 5),
        coord_t(2, 9),
        coord_t(7, 3),
        coord_t(8, 8),
        coord_t(1, 7)
    });

    DeepVStripes q(1.0, 5);
    q.initialize(l);
    q.run();

    for(int x = 0; x <= 10; ++x){
        for(int y = 0; y <= 10; ++y){
            coord_t u(y, x);
            coord_t cbest;
            bool good_example = findClosestBruteForce_exact(l, u, cbest);
            if(good_example)
                REQUIRE(q.getClosestPoint(u) == cbest);
        }
    }
}

TEST_CASE("DeepVStripes 3", "[deepvstripes-3]"){
    const size_t N = 1000, M = 1000;
    list<coord_t> l;
    for(size_t i = 0; i < N; ++i){
        l.push_back(coord_t(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        ));
    }

    DeepVStripes q(0.01, 8);
    q.initialize(l);
    q.run();

    for(size_t i = 0; i < M; ++i){
        coord_t u(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        );

        REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
    }
}

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

struct test {
    int x;
};

TEST_CASE("Slab decomposition RB", "[slab-rb-1]"){
    RBTree<test> t0; // t0 is empty
    auto t1 = t0.insert({0}, [](const test &lhs, const test &rhs){ return lhs.x < rhs.x; }); // t1 contains {0}
    auto t2 = t1.insert({1}, [](const test &lhs, const test &rhs){ return lhs.x < rhs.x; }); // t2 contains {0, 1}
    auto t3 = t2.remove({0}, [](const test &lhs, const test &rhs){ return lhs.x < rhs.x; }); // t3 contains {1}
}
