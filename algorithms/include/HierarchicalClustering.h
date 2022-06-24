#include "MapGraph.h"

#include <set>
#include <vector>

class Node {
private:
    Coord point;
    Node* left;
    Node* right;
    Node* nn = nullptr;    // Nearest neighbour

    std::set<Node*> leaves();
    double getDistance(Node* node);
public:
    friend class UPGMA;

    double distance_to_nn; // Distance to nearest neighbour

    Node(Coord point);
    Node(Node* left, Node* right);

    void updateDistances(std::set<Node*> clusters);
};

class UPGMA {
private:
    std::set<Node*> clusters;

    void closestPair(Node* &c1, Node* &c2);
public:
    UPGMA(std::vector<Coord> points);

    Node* buildTree();
};
