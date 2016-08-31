/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#ifndef NODE_H
#define NODE_H
#include <string>

using namespace std;

typedef struct node
{
  string val;
  node *left;
  node *right;
} node;

#endif