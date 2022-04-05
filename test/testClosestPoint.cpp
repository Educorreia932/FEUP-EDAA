#include <catch2/catch_all.hpp>

#include <bits/stdc++.h>

#include "K2DTreeClosestPoint.h"
#include "DeepVStripes.h"

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
    })); //(Lat, Lon), or (y, x)
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
    })); //(Lat, Lon), or (y, x)
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
    })); //(Lat, Lon), or (y, x)
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
