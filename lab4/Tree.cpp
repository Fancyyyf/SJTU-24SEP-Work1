#include "Tree.h"
#include <algorithm>
#include <cassert>
#include <limits>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;

/*
 * You can use this struct to compare a certain dimension of the tree nodes!
 * For example, if you want to compare the first dimension, you can simply call
 * DimComparator(0), such as DimComparator(0)(node_a, node_b).
 */
struct DimComparator {
    int dim;
    DimComparator(int d) : dim(d) {}
    bool operator()(const TreeNode *a, const TreeNode *b) const {
        assert(a->dimension() == b->dimension() && a->dimension() > dim);
        return isLessThan((*a)[dim], (*b)[dim]);
    }
    bool Equal(const TreeNode *a, const TreeNode *b){
        assert(a->dimension() == b->dimension() && a->dimension() > dim);
        return isEqual((*a)[dim], (*b)[dim]);
    }
};

TwoDimenTree::TwoDimenTree() {
    calculator = nullptr;
    root = nullptr;
}

void TwoDimenTree::destroyTree(TreeNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;  // 释放当前节点
}

TwoDimenTree::~TwoDimenTree() {
    destroyTree(root);
    root = nullptr;
    delete calculator;
}



TreeNode* TwoDimenTree::Nearest(TreeNode* node, const TreeNode& target, int depth,
                                TreeNode* best, double& bestDist){
    if (!node) return best;
    int dim = depth % 2;

    double temp = calculator->calculateDistance(*node,target);
    if (!best || isLessThan(temp, bestDist)) {
        best = node;
        bestDist = temp;
    } else if (isEqual(temp, bestDist)) {
        if (DimComparator(0)(node, best) ||
            (DimComparator(0).Equal(node, best) && DimComparator(1)(node, best))) {
            best = node;
        }
    }


    TreeNode* next = nullptr;
    TreeNode* other = nullptr;
    if (DimComparator(dim)(&target,node)) {
        next = node->left;
        other = node->right;
    } else {
        next = node->right;
        other = node->left;
    }

    best = Nearest(next,target,depth+1,best,bestDist);

    double verticalDist = calculator->calculateVerticalDistance(*node,target,dim);
    if(isLessThan(verticalDist,bestDist)){
        best = Nearest(other,target,depth+1,best,bestDist);
    }

    return best;
}


TreeNode *TwoDimenTree::findNearestNode(const TreeNode &target) {
    if (!root) return nullptr;

    TreeNode* best = nullptr;
    double bestDist = std::numeric_limits<double>::max();

    return Nearest(root,target,0,best,bestDist);
}

TreeNode* TwoDimenTree::build(vector<TreeNode*>nodes,int depth){
    if(nodes.empty()) return nullptr;
    if (nodes.size() == 1) return nodes[0];

    int dim = depth % 2;
    std::sort(nodes.begin(), nodes.end(), DimComparator(dim));

    int mid = nodes.size() / 2;
    while (mid > 0) {
        if (isEqual((*nodes[mid - 1])[dim], (*nodes[mid])[dim])) {
            mid--;
        } else {
            break;
        }
    }

        TreeNode* node = nodes[mid];
        vector<TreeNode*> left(nodes.begin(), nodes.begin() + mid);
        vector<TreeNode*> right(nodes.begin() + mid + 1, nodes.end());


        node->left = build(left, depth + 1);
        node->right = build(right, depth + 1);

        return node;
}


istream &operator>>(istream &in, TwoDimenTree &tree) {
    string Type;
    in >> Type;

    if (Type == "Euclidean") {
        tree.calculator = new EuclideanDistanceCalculator();
    } else if (Type == "Manhattan") {
        tree.calculator = new ManhattanDistanceCalculator();
    } else if (Type == "Haversine") {
        tree.calculator = new HaversineDistanceCalculator();
    }else {
        throw std::invalid_argument("Unknown distance type: " + Type);
    }

    int n;
    in >> n;
    std::vector<TreeNode*> nodes;
    for(int i = 0 ;i < n;i++){
        double x,y;
        in >> x >> y;
        nodes.push_back(new TreeNode({x, y}));
    }

    tree.root = tree.build(nodes,0);

    return in;
}
