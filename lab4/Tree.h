#pragma once

#include <iostream>
#include <vector>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;

class TwoDimenTree {
  /* DO NOT CHANGE SIGNATURE */
    friend istream &operator>>(istream &in,
                             TwoDimenTree &tree); 

private:
    /* data */
    /* calculator should indicate the distance calculation method being used */
    DistanceCalculator *calculator;
    TreeNode *root;

public:
    /* methods */
    /* DO NOT CHANGE SIGNATURE */
    TwoDimenTree();

    /* DO NOT CHANGE SIGNATURE */
    TreeNode *findNearestNode(const TreeNode &target);

    /* DO NOT CHANGE SIGNATURE */
    ~TwoDimenTree(); /* DO NOT CHANGE */

    void destroyTree(TreeNode* node);
    TreeNode* build(vector<TreeNode*> nodes, int depth = 0);
    TreeNode* Nearest(TreeNode* node, const TreeNode& target, int depth,
                                TreeNode* best, double& bestDist);

};
