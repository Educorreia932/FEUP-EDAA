#include "HierarchicalClustering.h"

Matrix::Matrix(std::vector<std::vector<double>> rows) : rows(rows) {

};

std::size_t Matrix::size() const {
    return rows.size();
}

std::vector<double> Matrix::operator[](int index) const {
    return rows[index];
}

void Matrix::add_row(std::vector<double> row) {
    rows.push_back(row);
}

void Matrix::add_column(std::vector<double> column) {
    for (int i = 0; i < size(); i++)
        rows[i].push_back(column[i]);
}

void Matrix::remove_row(int index) {
    rows.erase(rows.begin() + index);
}

void Matrix::remove_column(int index) {
    for (int i = 0; i < size(); i++)
        rows[i].erase(rows[i].begin() + index);
}

Tree::Tree() {

}

Tree::Tree(Tree* left, Tree* right) : left(left), right(right) {

}

std::vector<Tree*> Tree::leaves() {
    // Leaf node
    if (left == nullptr && right == nullptr)
        return { this };

    else {
        auto leaves = left->leaves();
        auto right_leaves = right->leaves();

        leaves.insert(leaves.end(), right_leaves.begin(), right_leaves.end());

        return leaves;
    }
}


UPGMA::UPGMA(std::vector<std::vector<double>> distance_matrix) : distance_matrix(distance_matrix) {

}

Tree UPGMA::calculate() {
    std::vector<Tree*> trees;

    // Create a tree for each entry
    for (int i = 0; i < distance_matrix.size(); i++)
        trees.push_back(new Tree());

    // Execute the algorithm 
    int iterations = trees.size();

    for (int k = iterations; k >= 1; k--) {
        // Find minimum distance
        int indexes[2] = { 0, 0 };

        for (int i = 0; i < distance_matrix.size(); i++)
            for (int j = 0; j < distance_matrix[0].size(); j++) {
                int minimum_distance = distance_matrix[indexes[0]][indexes[1]];

                if (distance_matrix[i][j] < minimum_distance) {
                    indexes[0] = i;
                    indexes[1] = j;
                }
            }

        // Create a new tree to join the clusters with minimum distance
        int i = indexes[0];
        int j = indexes[1];

        auto tree = new Tree(trees[i], trees[j]);

        if (k > 2) {
            // Remove from the list of trees the joined branches
            Tree* tree_i = trees[i];
            trees.erase(trees.begin() + i);

            Tree* tree_j = trees[j];
            trees.erase(trees.begin() + j);

            // Calculate the distances for the new cluster
            std::vector<double> distances;

            for (int x = 0; x <= distance_matrix.size(); x++)
                if (x != i && x != j) {
                    double si = tree_i->leaves().size();
                    double sj = tree_j->leaves().size();

                    // Use the weighted average to calculate the distances between the clusters
                    double distance = (si * distance_matrix[i][x] + sj * distance_matrix[j][x]) / (si + sj);
                    distances.push_back(distance);
                }

            // Remove column corresponding to j
            distance_matrix.remove_column(i);
            distance_matrix.remove_column(j);

            // Remove row corresponding to i
            distance_matrix.remove_row(i);
            distance_matrix.remove_row(j);

            // Add row with new distances
            distance_matrix.add_row(distances);

            // Add column with zero distances: of len (|dists| + 1)
            distance_matrix.add_column(std::vector<double>(distances.size() + 1, 0));

            trees.push_back(tree);
        }

        else
            return *tree;
    }

}