#include "HierarchicalClustering.h"

Node::Node(Coord point) : point(point), left(nullptr), right(nullptr) {

}

Node::Node(Node* left, Node* right) : left(left), right(right) {

}

std::set<Node*> Node::leaves() {
    // Leaf node
    if (this->left == nullptr && this->right == nullptr)
        return { this };

    else {
        auto leaves = left->leaves();
        auto right_leaves = right->leaves();

        leaves.insert(right_leaves.begin(), right_leaves.end());

        return leaves;
    }
}

double Node::getDistance(Node* node) {
    // Get the arithmetic mean distance between the node and another
    auto si = leaves().size();
    auto sj = node->leaves().size();
    double distance = 0;

    for (auto n1 : leaves())
        for (auto n2 : node->leaves())
            distance += n1->point.getDistance(n2->point);

    return distance / (si + sj);
}

void Node::updateDistances(std::set<Node*> clusters) {
    nn = nullptr;
    distance_to_nn = std::numeric_limits<double>::max();

    for (auto node : clusters) {
        if (node == this)
            continue;

        double distance_to_node = this->getDistance(node);

        bool no_nn = (nn == nullptr);

        if (no_nn || (distance_to_node < distance_to_nn)) {
            distance_to_nn = distance_to_node;
            nn = node;
        }
    }
}

UPGMA::UPGMA(std::vector<Coord> points) {
    for (auto point : points)
        clusters.insert(new Node(point));
}

void UPGMA::closestPair(Node*& c1, Node*& c2) {
    double minimum_distance = std::numeric_limits<double>::max();

    for (auto node : clusters)
        if (node->distance_to_nn < minimum_distance) {
            minimum_distance = node->distance_to_nn;
            c1 = node;
            c2 = node->nn;
        }
}

Node* UPGMA::buildTree() {
    for (auto node : clusters)
        node->updateDistances(clusters);

    std::size_t iterations = clusters.size();

    for (std::size_t i = 0; i < iterations - 1; i++) {
        // Find globally closest pair
        Node* c1, *c2;

        closestPair(c1, c2);

        // Join the clusters
        clusters.erase(c1);
        clusters.erase(c2);

        auto new_node = new Node(c1, c2);

        clusters.insert(new_node);

        new_node->updateDistances(clusters);

        // Recalculate NN's for each cluster previously having one of the joined clusters as its NN
        for (auto node : clusters)
            if (node->nn == c1 or node->nn == c2)
                node->updateDistances(clusters);
    }

    return *clusters.end();
}
