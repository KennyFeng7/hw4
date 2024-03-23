#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int checkPaths(Node* node, bool& equalpath);

bool equalPaths(Node* root) {
  if (root == nullptr){
    return true; 
  }
  bool equalpath = true;
  checkPaths(root, equalpath);
  return equalpath;
}

int checkPaths(Node* node, bool& equalpath) {
  if (node == nullptr){
    return 0;
  }
  int leftd = checkPaths(node->left, equalpath);
  int rightd = checkPaths(node->right, equalpath);
  if (leftd == 0 && rightd > 0){
    leftd =rightd;
  }
  if (leftd > 0 && rightd == 0){
    rightd = leftd;
  }
  if (leftd == rightd) {
    return leftd + 1;
  } 
  else {
    equalpath = false;
    return -1;
  }
}