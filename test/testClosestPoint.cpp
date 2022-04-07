#include <catch2/catch_all.hpp>

#include <bits/stdc++.h>

#include "K2DTreeClosestPoint.h"
#include "DeepVStripes.h"
#include "VStripesRadius.h"

using namespace std;

bool findClosestBruteForce_exact(const list<Vector2> &l, const Vector2 &u, Vector2 &cbest){
    int dbest = 1000000000;
    bool good_example = true;
    for(const Vector2 &c: l){
        int dx = c.x - u.x;
        int dy = c.y - u.y;
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

Vector2 findClosestBruteForce(const list<Vector2> &l, const Vector2 &u){
    double dbest = 1000000000.0L;
    Vector2 cbest;
    for(const Vector2 &c: l){
        double dx = c.x - u.x;
        double dy = c.y - u.y;
        double d = dx*dx + dy*dy;
        if(d < dbest){
            dbest = d;
            cbest = c;
        }
    }
    return cbest;
}

TEST_CASE("Quad Tree", "[quadtree]"){
    K2DTreeClosestPoint q;
    q.initialize(list<Vector2>({
        Vector2(1, 9),
        Vector2(4, 7),
        Vector2(8, 3),
        Vector2(4, 5),
        Vector2(2, 9),
        Vector2(7, 3),
        Vector2(8, 8),
        Vector2(1, 7)
    }));
    q.run();

    REQUIRE(q.getClosestPoint(Vector2(9,2)) == Vector2(8,3));
}

TEST_CASE("Quad Tree 2", "[quadtree-2]"){
    list<Vector2> l({
        Vector2(1, 9),
        Vector2(4, 7),
        Vector2(8, 3),
        Vector2(4, 5),
        Vector2(2, 9),
        Vector2(7, 3),
        Vector2(8, 8),
        Vector2(1, 7)
    });

    K2DTreeClosestPoint q;
    q.initialize(l);
    q.run();

    for(int x = 0; x <= 10; ++x){
        for(int y = 0; y <= 10; ++y){
            Vector2 u(y, x);
            Vector2 cbest;
            bool good_example = findClosestBruteForce_exact(l, u, cbest);
            if(good_example)
                REQUIRE(q.getClosestPoint(u) == cbest);
        }
    }
}

TEST_CASE("Quad Tree 3", "[quadtree-3]"){
    const size_t N = 1000, M = 1000;
    list<Vector2> l;
    for(size_t i = 0; i < N; ++i){
        l.push_back(Vector2(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        ));
    }

    K2DTreeClosestPoint q;
    q.initialize(l);
    q.run();

    for(size_t i = 0; i < M; ++i){
        Vector2 u(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        );

        REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
    }
}

TEST_CASE("VStripes", "[vstripes]"){
    VStripes q(2.0);
    q.initialize(list<Vector2>({
        Vector2(1, 9),
        Vector2(4, 7),
        Vector2(8, 3),
        Vector2(4, 5),
        Vector2(2, 9),
        Vector2(7, 3),
        Vector2(8, 8),
        Vector2(1, 7)
    }));
    q.run();

    REQUIRE(q.getClosestPoint(Vector2(9,2)) == Vector2(8,3));
}

TEST_CASE("DeepVStripes", "[deepvstripes]"){
    DeepVStripes q(1.0, 5);
    q.initialize(list<Vector2>({
        Vector2(1, 9),
        Vector2(4, 7),
        Vector2(8, 3),
        Vector2(4, 5),
        Vector2(2, 9),
        Vector2(7, 3),
        Vector2(8, 8),
        Vector2(1, 7)
    }));
    q.run();

    REQUIRE(q.getClosestPoint(Vector2(9,2)) == Vector2(8,3));
}

TEST_CASE("DeepVStripes 2", "[deepvstripes-2]"){
    list<Vector2> l({
        Vector2(1, 9),
        Vector2(4, 7),
        Vector2(8, 3),
        Vector2(4, 5),
        Vector2(2, 9),
        Vector2(7, 3),
        Vector2(8, 8),
        Vector2(1, 7)
    });

    DeepVStripes q(1.0, 5);
    q.initialize(l);
    q.run();

    for(int x = 0; x <= 10; ++x){
        for(int y = 0; y <= 10; ++y){
            Vector2 u(y, x);
            Vector2 cbest;
            bool good_example = findClosestBruteForce_exact(l, u, cbest);
            if(good_example)
                REQUIRE(q.getClosestPoint(u) == cbest);
        }
    }
}

TEST_CASE("DeepVStripes 3", "[deepvstripes-3]"){
    const size_t N = 1000, M = 1000;
    list<Vector2> l;
    for(size_t i = 0; i < N; ++i){
        l.push_back(Vector2(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        ));
    }

    DeepVStripes q(0.01, 8);
    q.initialize(l);
    q.run();

    for(size_t i = 0; i < M; ++i){
        Vector2 u(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        );

        REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
    }
}

TEST_CASE("VStripes Closest Points in Radius", "[vstripes-radius-1]"){
    VStripesRadius q;
    q.initialize(list<Coord>({
        Coord(1, 9),
        Coord(4, 7),
        Coord(8, 3),
        Coord(4, 5),
        Coord(2, 9),
        Coord(7, 3),
        Coord(8, 8),
        Coord(1, 7)
    }), 2.0);
    q.run();

    vector<Coord> v;

    v = q.getClosestPoints(Coord(9.00,2.00)); sort(v.begin(), v.end(), Coord::compXY); REQUIRE(v == vector<Coord>({Coord(8,3)}));
    v = q.getClosestPoints(Coord(8.00,2.00)); sort(v.begin(), v.end(), Coord::compXY); REQUIRE(v == vector<Coord>({Coord(7,3), Coord(8,3)}));
    v = q.getClosestPoints(Coord(8.00,1.01)); sort(v.begin(), v.end(), Coord::compXY); REQUIRE(v == vector<Coord>({Coord(8,3)}));
}

TEST_CASE("VStripes Closest Points in Radius 2", "[vstripes-radius-3]"){
    const size_t N = 1000, M = 1000;
    list<Coord> l;
    for(size_t i = 0; i < N; ++i){
        l.push_back(Coord(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        ));
    }

    const double d = 0.05;
    const double epsilon = d * 0.000000001;

    VStripesRadius q;
    q.initialize(l, d);
    q.run();

    for(size_t i = 0; i < M; ++i){
        Coord u(
            double(rand())/double(RAND_MAX),
            double(rand())/double(RAND_MAX)
        );
        
        vector<Coord> v = q.getClosestPoints(u);
        bool (*cmp)(const Coord&, const Coord&) = Coord::compXY;
        set<Coord, bool (*)(const Coord&, const Coord&)> s(cmp);
        s.insert(v.begin(), v.end());

        for(const Coord &p: l){
            double dist = u.getDistance(p);
            if     (dist > d+epsilon) REQUIRE(s.count(p) == 0);
            else if(dist < d-epsilon) REQUIRE(s.count(p) == 1);
        }
    }
}
