/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "node.h"
#include <iostream>

using namespace std;

class standardizer{
	private:
		node* Tree;
		void init(node* root);
		node* standardize(node* root);
		void printError(int n);
		node* standardizeNode(node* root);
	public:
		void run(node* root);
		void tempPrint(node* root, int num);
		node* getTree();
};