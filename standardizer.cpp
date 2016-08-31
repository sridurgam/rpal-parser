#include "standardizer.h"

void standardizer::run(node* root){
	init(root);
	Tree = standardize(Tree);
}

void standardizer::init(node* root){
	Tree = root;
}

void standardizer::tempPrint(node* root, int num){
	for (int i = 0; i < num; i++){
		cout << ".";
	}
	cout << root->val << endl;
	if(root->left){
		tempPrint(root->left, num + 1);
	}
	if(root->right){
		tempPrint(root->right, num + 1);
	}
}

node* standardizer::standardize(node* root){
	if(root->left){
		root->left = standardize(root->left);
	}
	root = standardizeNode(root);
	if(root->right){
		root->right = standardize(root->right);
	}
	return root;
}

node* standardizer::standardizeNode(node* root){
	node* X;
	node* P;
	node* E;
	node* temp;

	if(root->val == "let"){
		if(root->left->val == "="){
			X = root->left->left;
			E = X->right;
			P = root->left->right;

			root->val = "gamma";
			root->left->val = "lambda";
			root->left->left = X;
			X->right = P;
			root->left->right = E;

			return root;
		} else {
			printError(1);
		}
	}

	if(root->val == "where"){
		if(root->left->right->val == "="){
			P = root->left;
			temp = P->right;
			X = temp->left;
			E = X->right;

			root->val = "gamma";
			temp->val = "lambda";
			root->left = temp;
			temp->right = E;
			temp->left = X;
			X->right = P;
			P->right = NULL;

			return root;
		} else {
			printError(1);
		}
	}

	if(root->val == "within"){
		if(root->left->right->val == "=" && root->left->val == "="){
			node* X2;
			node* E2;
			node* temp1;

			temp = root->left;
			X = temp->left;
			temp1 = temp->right;
			E = X->right;
			X2 = temp1->left;
			E2 = X2->right; 

			root->val = "=";
			root->left = X2;
			X2->right = temp1;
			temp1->val = "gamma";
			temp1->left = temp;
			temp->val = "lambda";
			temp->left = X;
			temp->right = E;
			X->right = E2;

			return root;
		} else {
			printError(1);
		}
	}

	if(root->val == "function_form"){
		node* V = new node();
		node* temp1 = new node();
		P = root->left;
		root->val = "=";
		if(P->right){
			V = P->right;
		}
		if(V->right){
			temp1 = V->right;
		}

		while(temp1){
			temp = new node();
			temp->val = "lambda";
			P->right = temp;
			temp->left = V;
			if(temp1->right){
				temp1 = temp1->right;
				P = V;
				V = V->right;
			} else {
				temp1 = NULL;
			}
		}

		return root;
	}

	if(root->val == "@"){
		X = root->left;
		P = X->right;
		E = P->right;

		root->val = "gamma";
		node* temp = new node();
		temp->val = "gamma";
		root->left = temp;
		temp->right = E;
		temp->left = P;
		P->right = X;
		X->right = NULL;
	
		return root;
	}

	if(root->val == "and"){
		node* temp1;
		node* X1;
		node* E1;

		root->val = "=";
		temp = root->left;
		temp->val = ",";
		temp1 = temp->right;
		temp1->val = "tau";
		X = temp->left;
		E = X->right;
		
		X1 = temp1->left;
		E1 = X1->right;
		X->right = X1;
		X1->right = NULL;
		temp1->left = E;
		E->right = E1;

		while(temp1->right){
			temp1 = temp1->right;
			X = temp1->left;
			E = X->right;

			X1->right = X;
			X->right = NULL;
			E1->right = E;

			X1 = X;
			E1 = E;
		}

		temp->right->right = NULL;
		return root;
	}

	if(root->val == "lambda"){
		X = root->left;
		P = X->right;
		while(P->right){
			temp = new node();
			temp->val = "lambda";
			X->right = temp;
			temp->left = P;

			E = P->right;
			X = P;
			P = E;
		}
		return root;
	}

	if(root->val == "rec"){
		if(root->left->val == "="){
			X = root->left->left;
			E = X->right;

			node* X1 = new node();
			X1->val = X->val;
			X1->right = X->right;
			X1->left = X->left;

			root->val = "=";
			root->left = X;

			node* gamma = new node();
			gamma->val = "gamma";
			X->right = gamma;

			node* Y = new node();
			Y->val = "Y";
			gamma->left = Y;

			node* lambda = new node();
			lambda->val = "lambda";
			Y->right = lambda;

			lambda->left = X1;		
		} else {
			printError(4);
		}
	}

	return root;
}

void standardizer::printError(int n){
	switch(n){
		case 1:
			cout << "Expected '=', received something else." << endl;
			break;
		default:
			break;
	}
}

node* standardizer::getTree(){
	return Tree;
}