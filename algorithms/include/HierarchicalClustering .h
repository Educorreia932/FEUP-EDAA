#include <vector>

class Item {
private:
    Item &left;
    Item &right;
public:
    friend class HierarchicalClustering;

    Item();
};

class HierarchicalClustering {
public:
    HierarchicalClustering(std::vector<Item> items);

    void buildTree();
};
