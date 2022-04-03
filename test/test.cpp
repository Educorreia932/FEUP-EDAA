#include <catch2/catch_all.hpp>

#include "QuadTreeClosestPoint.h"
#include "DeepVStripes.h"

using namespace std;

bool findClosestBruteForce_exact(const list<coord_t>& l, const coord_t& u, coord_t& cbest) {
    int dbest = 1000000000;
    bool good_example = true;
    for (const coord_t& c : l) {
        int dx = c.getLon() - u.getLon();
        int dy = c.getLat() - u.getLat();
        int d = dx * dx + dy * dy;
        if (d < dbest) {
            dbest = d;
            cbest = c;
            good_example = true;
        }
        else if (d == dbest) {
            good_example = false;
        }
    }
    return good_example;
}

coord_t findClosestBruteForce(const list<coord_t>& l, const coord_t& u) {
    coord_t::deg_t dbest = 1000000000.0L;
    coord_t cbest;
    for (const coord_t& c : l) {
        coord_t::deg_t dx = c.getLon() - u.getLon();
        coord_t::deg_t dy = c.getLat() - u.getLat();
        coord_t::deg_t d = dx * dx + dy * dy;
        if (d < dbest) {
            dbest = d;
            cbest = c;
        }
    }
    return cbest;
}

// TEST_CASE("Quad Tree", "[quadtree]"){
//     QuadTreeClosestPoint q;
//     q.initialize(list<coord_t>({
//         coord_t(1, 9),
//         coord_t(4, 7),
//         coord_t(8, 3),
//         coord_t(4, 5),
//         coord_t(2, 9),
//         coord_t(7, 3),
//         coord_t(8, 8),
//         coord_t(1, 7)
//     })); //(Lat, Lon), or (y, x)
//     q.run();

//     REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
// }

// TEST_CASE("Quad Tree 2", "[quadtree-2]"){
//     list<coord_t> l({
//         coord_t(1, 9),
//         coord_t(4, 7),
//         coord_t(8, 3),
//         coord_t(4, 5),
//         coord_t(2, 9),
//         coord_t(7, 3),
//         coord_t(8, 8),
//         coord_t(1, 7)
//     });

//     QuadTreeClosestPoint q;
//     q.initialize(l);
//     q.run();

//     for(int x = 0; x <= 10; ++x){
//         for(int y = 0; y <= 10; ++y){
//             coord_t u(y, x);
//             coord_t cbest;
//             bool good_example = findClosestBruteForce_exact(l, u, cbest);
//             if(good_example)
//                 REQUIRE(q.getClosestPoint(u) == cbest);
//         }
//     }
// }

// TEST_CASE("Quad Tree 3", "[quadtree-3]"){
//     const size_t N = 1000, M = 1000;
//     list<coord_t> l;
//     for(size_t i = 0; i < N; ++i){
//         l.push_back(coord_t(
//             double(rand())/double(RAND_MAX),
//             double(rand())/double(RAND_MAX)
//         ));
//     }

//     QuadTreeClosestPoint q;
//     q.initialize(l);
//     q.run();

//     for(size_t i = 0; i < M; ++i){
//         coord_t u(
//             double(rand())/double(RAND_MAX),
//             double(rand())/double(RAND_MAX)
//         );

//         REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
//     }
// }

// TEST_CASE("VStripes", "[vstripes]"){
//     VStripes q(2.0);
//     q.initialize(list<coord_t>({
//         coord_t(1, 9),
//         coord_t(4, 7),
//         coord_t(8, 3),
//         coord_t(4, 5),
//         coord_t(2, 9),
//         coord_t(7, 3),
//         coord_t(8, 8),
//         coord_t(1, 7)
//     })); //(Lat, Lon), or (y, x)
//     q.run();

//     REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
// }

// TEST_CASE("DeepVStripes", "[deepvstripes]"){
//     DeepVStripes q(1.0, 5);
//     q.initialize(list<coord_t>({
//         coord_t(1, 9),
//         coord_t(4, 7),
//         coord_t(8, 3),
//         coord_t(4, 5),
//         coord_t(2, 9),
//         coord_t(7, 3),
//         coord_t(8, 8),
//         coord_t(1, 7)
//     })); //(Lat, Lon), or (y, x)
//     q.run();

//     REQUIRE(q.getClosestPoint(coord_t(9,2)) == coord_t(8,3));
// }

// TEST_CASE("DeepVStripes 2", "[deepvstripes-2]"){
//     list<coord_t> l({
//         coord_t(1, 9),
//         coord_t(4, 7),
//         coord_t(8, 3),
//         coord_t(4, 5),
//         coord_t(2, 9),
//         coord_t(7, 3),
//         coord_t(8, 8),
//         coord_t(1, 7)
//     });

//     DeepVStripes q(1.0, 5);
//     q.initialize(l);
//     q.run();

//     for(int x = 0; x <= 10; ++x){
//         for(int y = 0; y <= 10; ++y){
//             coord_t u(y, x);
//             coord_t cbest;
//             bool good_example = findClosestBruteForce_exact(l, u, cbest);
//             if(good_example)
//                 REQUIRE(q.getClosestPoint(u) == cbest);
//         }
//     }
// }

// TEST_CASE("DeepVStripes 3", "[deepvstripes-3]"){
//     const size_t N = 1000, M = 1000;
//     list<coord_t> l;
//     for(size_t i = 0; i < N; ++i){
//         l.push_back(coord_t(
//             double(rand())/double(RAND_MAX),
//             double(rand())/double(RAND_MAX)
//         ));
//     }

//     DeepVStripes q(0.01, 8);
//     q.initialize(l);
//     q.run();

//     for(size_t i = 0; i < M; ++i){
//         coord_t u(
//             double(rand())/double(RAND_MAX),
//             double(rand())/double(RAND_MAX)
//         );

//         REQUIRE(q.getClosestPoint(u) == findClosestBruteForce(l, u));
//     }
// }

#include "Arc.h"
#include "Vector2.h"
#include "VoronoiDiagram.h"

TEST_CASE("Edge slope 1", "[edge-1]") {
    Vector2 start(0, 0);
    Edge edge_1(start, Vector2(0, 1), Vector2(0, -1));

    REQUIRE(edge_1.m == 0);
}

TEST_CASE("Edge slope 2", "[edge-2]") {
    Vector2 start(0, 0);
    Edge edge_2(start, Vector2(0, 1), Vector2(1, 0));

    REQUIRE(edge_2.m == 1);
}

TEST_CASE("Edge slope 3", "[edge-3]") {
    Vector2 start(0, 0);
    Edge edge_3(start, Vector2(-1, 0), Vector2(1, 0));

    REQUIRE(edge_3.m == std::numeric_limits<double>::infinity());
}

TEST_CASE("Arc y-point", "[arc-1]") {
    Vector2 point(5, 6);
    double x = point.x;
    double sweep_line = point.y;
    Arc arc(Site{ Vector2(5, 10) });

    REQUIRE(arc.getPoint(x, sweep_line).y == 8);
}

TEST_CASE("Arc intersection 1", "[arc-2]") {
    // Intersection with edge with horizontal slope
    Arc arc(Site{ Vector2(5, 10) });
    Edge edge(Vector2(9, 10), Vector2(9, -1), Vector2(9, 1));
    double sweep_line = 8;

    REQUIRE(arc.intersect(edge, sweep_line).x == 7);
    REQUIRE(arc.intersect(edge, sweep_line).y == 10);
}

TEST_CASE("Arc intersection 2", "[arc-3]") {
    // Intersection with edge with slope equal to -1
    Arc arc(Site{ Vector2(5, 10) });
    Edge edge(Vector2(9, 8), Vector2(8, 7), Vector2(10, 9));
    double sweep_line = 8;

    REQUIRE(arc.intersect(edge, sweep_line).x == 7);
    REQUIRE(arc.intersect(edge, sweep_line).y == 10);
}

TEST_CASE("Arc intersection 3", "[arc-4]") {
    // Intersection with vertical edge
    Arc arc(Site{ Vector2(5, 10) });
    Edge edge(Vector2(7, std::numeric_limits<double>::infinity()), Vector2(5, 10), Vector2(9, 10));
    double sweep_line = 8;

    REQUIRE(arc.intersect(edge, sweep_line).x == 7);
    REQUIRE(arc.intersect(edge, sweep_line).y == 10);
}