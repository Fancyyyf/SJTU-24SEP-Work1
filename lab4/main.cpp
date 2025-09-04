#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

#include "Tree.h"

using namespace std;

bool do_run(ifstream &testcase) {
  bool res = false;
  auto tree = new TwoDimenTree();
  try {
    testcase >> *tree;
  } catch (std::exception &e) {
    cout << e.what() << endl;
    goto out;
  }

  int testNum;
  testcase >> testNum;
  for (int i = 0; i < testNum; i++) {
    double x, y;
    double z, w;
    testcase >> x;
    testcase >> y;
    testcase >> z;
    testcase >> w;
    TreeNode target({x, y});
    TreeNode *node = tree->findNearestNode(target);
    auto coordinates = node->getCoordinates();
    TreeNode answer({z, w});
    if (answer.getCoordinates() != node->getCoordinates()) {
      cout << "case:" << x << " " << y << ",";
      cout << "expect:" << z << " " << w << ",";
      cout << "actual:" << coordinates[0] << " " << coordinates[1] << endl;
      goto out;
    }
  }
  res = true;
out:
  delete tree;
  return res;
}

void run(string name) {
  ifstream testcase;
  testcase.open(name);

  bool ret = false;
  ret = do_run(testcase);
  if (ret) {
    cout << "pass at " << name << endl;
  } else {
    cout << "Failed at " << name << endl;
  }
  testcase.close();
}

int main() {
  /* You can change the testcase path as you like :) */
  /* run_testcase(<test_file_path>); */
  run("1.txt");
  run("2.txt");
  run("3.txt");
  run("4.txt");
  run("5.txt");
  run("6.txt");

  /* You are supposed to pass all testcases to get full grade */

  return 0;
}
