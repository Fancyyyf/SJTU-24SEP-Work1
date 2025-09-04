#include <stdexcept>

#include "TreeNode.h"

TreeNode::TreeNode(initializer_list<double> coords) {
    coordinates = coords;
    left = right = nullptr;
}

const double &TreeNode::operator[](int index) const {
    if (index < 0 || index >= coordinates.size()) {
        throw std::out_of_range("TreeNode index out of range");
    }
    return coordinates[index];
}

int TreeNode::dimension() const {
    return 2;
}

const vector<double> &TreeNode::getCoordinates() const {
    return coordinates;
}

TreeNode::~TreeNode() {} // Even though the implementation might be empty, it's
                         // a good practice to define it.