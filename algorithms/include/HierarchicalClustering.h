#include <vector>

class Matrix {
private:
    std::vector<std::vector<double>> rows;
public:
    Matrix(std::vector<std::vector<double>> rows);

    void add_row(std::vector<double> row);
    void add_column(std::vector<double> column);

    void remove_row(int index);
    void remove_column(int index);

    std::size_t size() const;

    double at(int i, int j) const;
};

class Tree {
private:
    Tree* left = nullptr;
    Tree* right = nullptr;
public:
    double distance;

    Tree();
    Tree(Tree* left, Tree* right, double distance);

    std::vector<Tree*> leaves();
};

class UPGMA {
private:
    Matrix distance_matrix;

public:
    UPGMA(std::vector<std::vector<double>> distance_matrix);

    Tree calculate();
};
