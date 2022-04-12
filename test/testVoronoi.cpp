#include "FortuneAlgorithm.h"

#include <catch2/catch_all.hpp>

#include <stdlib.h>
#include <iostream>

// #define CATCH_CONFIG_MAIN

VoronoiDiagram voronoi(std::vector<Site*> sites) {
    VoronoiDiagram diagram = FortuneAlgorithm(sites).construct();

    return diagram;
}

TEST_CASE("Fortune's Algorithm", "[fortune]") {
    std::vector<Site*> sites = {
        new Site{Vector2(6, 6)},
        new Site{Vector2(2, 4)},
        new Site{Vector2(4, 2)},
        new Site{Vector2(1, 1)}
    };

    VoronoiDiagram diagram = voronoi(sites);
}

TEST_CASE("Point on edge", "[edge]") {
    Edge edge_1(Vector2(0, 0), Vector2(0, 2));
    Edge edge_2(Vector2(1, 1), Vector2(3, 3));
    Vector2 point_1(0, 3);
    Vector2 point_2(0, 1);
    Vector2 point_3(2, 2);

    REQUIRE(!point_1.isOn(edge_1));
    REQUIRE(point_2.isOn(edge_1));
    REQUIRE(point_3.isOn(edge_2));
}

TEST_CASE("Edge intersection", "[edge]") {
    Edge edge_1(Vector2(0, 3), Vector2(6, -3), false);
    Edge edge_2(Vector2(0, 2), Vector2(4, 2));
    Vector2 intersection;

    edge_1.intersect(edge_2, intersection);

    REQUIRE(intersection == Vector2(1, 2));
}

TEST_CASE("Bounding Box", "[box]") {
    Box box(4, 2);
    Vector2 intersection;
    Edge edge;

    // 1) Edge starts inside the box and isn't finished
    edge = Edge(Vector2(2, 1), Vector2(6, 3), false);

    box.intersect(edge, intersection);

    edge.end = intersection;

    REQUIRE(edge == Edge(Vector2(2, 1), Vector2(4, 2)));

    // 2) Edge starts outside the box and intersects it
    edge = Edge(Vector2(0, 3), Vector2(6, -3), false);

    box.intersect(edge, intersection);

    edge.start = intersection;

    box.intersect(edge, intersection);

    edge.end = intersection;
    
    REQUIRE(edge == Edge(Vector2(1, 2), Vector2(3, 0)));

    // 3) Edge starts outside the box and doesn't intersect it
    edge = Edge(Vector2(3, 4), Vector2(5, 3), false);

    REQUIRE(!box.intersect(edge, intersection));

    // 4) Edge starts inside the box and ends outside the box
    edge = Edge(Vector2(2, 1), Vector2(0, 3));

    box.intersect(edge, intersection);

    edge.end = intersection;

    REQUIRE(edge == Edge(Vector2(2, 1), Vector2(1, 2)));

    // 5) Edge starts and ends outside the box and doesn't intersect it
    edge = Edge(Vector2(3, 4), Vector2(5, 3));

    REQUIRE(!box.intersect(edge, intersection));

    // 6) Edge starts and ends outside the box and intersects it
    edge = Edge(Vector2(0, 3), Vector2(6, -3));

    box.intersect(edge, intersection);

    edge.start = intersection;

    box.intersect(edge, intersection);

    edge.end = intersection;
    
    REQUIRE(edge == Edge(Vector2(1, 2), Vector2(3, 0)));

}