/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "node.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>

using namespace std;

struct element;

typedef struct envNode{
	int env;
	envNode* parent;
	map<string, element*> varMap;
}envNode;

typedef struct element{
	string type;
	int num;
	vector<string> child;
	vector<element*> tau;
	envNode* env;
}element;

class cse{
	public:
		void init(node* n);
		void run();
		void tempPrint();
		void cseMachine();
	private:
		void buildControlStructures(element* e, int d_num);
		void parseTree(node* n, int num);
		void LoadControlStructure(int num);
		void Error(int num);
		void cseExecute();
		element* buildTau(node* n);
		void Printout(element* e);
		node* tree;
		envNode* curEnv;
		int num_deltas;
		int num_envs;
		vector<queue<element*> > delta_vec;
		stack<element*> Control;
		stack<element*> Stack;
		stack<envNode*> envStack;
		map<string, envNode*> envMap;
};