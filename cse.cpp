/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/
#include "cse.h"

void cse::init(node* n){
	tree = n;
}

void cse::buildControlStructures(element* e, int d_num){
	if(d_num < delta_vec.size()){
			delta_vec[d_num].push(e);
	} else {
		queue<element*> delta;
		delta.push(e);
		delta_vec.push_back(delta);
	}
}

element* cse::buildTau(node* n){
	element* e = new element();
	e->type = "tau";
	e->num = 0;

	while(n){
		element* e1 = new element();
		string s = "";
		if(n->val[0] == '<'){
			if(n->val[1] == 'I'){
				if(n->val[2] == 'D'){
					int i = 4;
					while(n->val[i] != '>'){
						s.push_back(n->val[i]);
					}
					e1->type = "id";
					e1->child.push_back(s);
				} else {
					int i = 5;
					while(n->val[i] != '>'){
						s.push_back(n->val[i]);
						i++;
					}
					e1->type = "int";
					stringstream ss;
					ss << s;
					ss >> i;

					e1->num = i;
				}
			} else if(n->val[1] == 'S'){
				int i = 6;
				while(n->val[i] != '\''){
					s.push_back(n->val[i]);
					i++;
				}
				e1->type = "string";
				e1->child.push_back(s);
			} else if(n->val[1] == 't'){
				e1->type = "true";
			} else if(n->val[1] == 'f'){
				e1->type = "false";
			} else if(n->val[1] == 'n'){
				e1->type = "nil";
			} else if(n->val[1] == 'd'){
				e1->type = "dummy";
			}else {
				Error(0);
			}
		} else if (n->val == "tau"){
			e1 = buildTau(n->left);
		}
		e->tau.push_back(e1);
		e->num = e->num + 1;
		node* tempNode = n;
		if(tempNode->right){
			tempNode = tempNode->right;
		} else {
			break;
		}
	}

	return e;
}

void cse::parseTree(node* n, int cur){
	if(n->val == "tau"){
		element* e = new element();
		e->type = "tau";
		e->num = 0;
		if(n->left){
			e->num += 1;
		}

		node* temp = n->left;
		while(temp->right){
			e->num += 1;
			temp = temp->right;
		}

		buildControlStructures(e, cur);

		if(n->left){
			parseTree(n->left, cur);
		}

		temp = n->left;
		while(temp->right){
			parseTree(temp->right, cur);
			temp = temp->right;
		}
	} else if(n->val == "->"){
		node* B = n->left;
		node* E1 = B->right;
		node* E2 = E1->right;

		element* e = new element();
		element* e1 = new element();
		element* e2 = new element();

		e->type = "delta";
		num_deltas++;
		e->num = num_deltas;

		buildControlStructures(e, cur);
		parseTree(E1, num_deltas);

		e1->type = "delta";
		num_deltas++;
		e1->num = num_deltas;

		buildControlStructures(e1, cur);
		parseTree(E2, num_deltas);

		e2->type = "beta";
		e2->num = 0;

		buildControlStructures(e2, cur);
		parseTree(B, cur);
	} else if (n->val == "lambda"){
		element* e = new element();
		e->type = "lambda";
		num_deltas++;
		e->num = num_deltas;

		if(n->left->val == ","){
			node* temp = n->left->left;
			e->child.push_back(temp->val);

			while(temp->right){
				temp = temp->right;
				e->child.push_back(temp->val);
			}
		} else if(n->left->val == "()"){
			return;
		} else {
			e->child.push_back(n->left->val);
		}

		buildControlStructures(e, cur);
		parseTree(n->left->right, num_deltas);
	} else {
		element* e = new element();
		e->type = n->val;
		e->num = 0;
		buildControlStructures(e, cur);

		if(n->left){
			node* temp = n->left;
			
			parseTree(temp, cur);
			while(temp->right){
				parseTree(temp->right, cur);
				temp = temp->right;
			}
		}
	}
}

void cse::tempPrint(){
	for (int i = 0; i <= num_deltas; i++){
		while(!delta_vec[i].empty()){
			cout << delta_vec[i].front()->type << " ";
			if(delta_vec[i].front()->type == "lambda" || delta_vec[i].front()->type == "tau" || delta_vec[i].front()->type == ","){
				cout << delta_vec[i].front()->num << " ";
			}
			delta_vec[i].pop();
		}
		cout << endl;
	}
}

void cse::LoadControlStructure(int num){
	queue<element*> Q;
	int l = 0;
	//cout << "Loading " << delta_vec[num].size();
	while(!delta_vec[num].empty()){
		Control.push(delta_vec[num].front());
		Q.push(delta_vec[num].front());
		delta_vec[num].pop();
	}
	while(!Q.empty()){
		delta_vec[num].push(Q.front());
		Q.pop();
	}
}

void cse::cseExecute(){
	//cout << "Control: " << Control.top()->type << " Stack: " << Stack.top()->type << " " << Stack.top()->num << endl;
	//cout << "Control size: " << Control.size() << " Stack size: " << Stack.size() << " " << endl;
	/*if(Stack.top()->type == "tau"){
		cout << "TAU ";
		int j = 0;
		while(j < Stack.top()->num){
			cout << Stack.top()->tau[j]->type << " ";
			j++;
		}
		cout << endl;
	}*/
	int i;
	if(Control.top()->type == "delta"){
		i = Control.top()->num;
		Control.pop();
		
		envNode* controlEnvNode = new envNode();
		controlEnvNode->env = ++num_envs;
		controlEnvNode->parent = envStack.top();
		/*if (envStack.top()){
			cout << "Parent of: " << controlEnvNode->env << " " << envStack.top()->env << endl;
		}*/
		element* e = new element();
		e->type = "env";
		e->num = controlEnvNode->env;
		e->env = controlEnvNode;

		envStack.push(controlEnvNode);
		curEnv = controlEnvNode;
		//cout << "Total envs 1: " << num_envs << endl;
		Control.push(e);
		Stack.push(e);

		LoadControlStructure(i);
	} else if (Control.top()->type.at(0) == '<'){
		if(Control.top()->type.at(1) == 'I'){
			if(Control.top()->type.at(2) == 'D'){
				element* e = new element();
				string s;
				i = 4;

				while(Control.top()->type.at(i) != '>'){
					s.push_back(Control.top()->type.at(i));
					i++;
				}

				e->type = "id";
				e->child.push_back(s);

				Control.pop();
				Control.push(e);
			} else {
				element* e = new element();
				i = 5;
				string s;

				while(Control.top()->type.at(i) != '>'){
					s.push_back(Control.top()->type.at(i));
					i++;
				}

				int value;
				e->type = "int";
				stringstream ss;
				ss<<s;
				ss>>value;
				e->num = value;

				Control.pop();
				Control.push(e);
			}
		} else if (Control.top()->type.at(1) == 'S'){
			element* e = new element();
			i = 6;
			string s;

			while(Control.top()->type.at(i) != '\''){
				s.push_back(Control.top()->type.at(i));
				i++;
			}
			//cout << "String is : " << s << endl;
			e->type = "string";
			e->child.push_back(s);

			Control.pop();
			Control.push(e);
		} else if (Control.top()->type.at(1) == 't'){
			element* e = new element();
			e->type = "true";

			Control.pop();
			Control.push(e);
		} else if (Control.top()->type.at(1) == 'f'){
			element* e = new element();
			e->type = "false";

			Control.pop();
			Control.push(e);
		} else if (Control.top()->type.at(1) == 'd'){
			element* e = new element();
			e->type = "dummy";

			Control.pop();
			Control.push(e);
		} else if (Control.top()->type.at(1) == 'n'){
			element* e = new element();
			e->type = "nil";

			Control.pop();
			Control.push(e);
		} else {
			Error(1);
		}
	} else if (Control.top()->type == "beta"){
		Control.pop();
		if(Stack.top()->type == "true"){
			Stack.pop();
			Control.pop();
		} else {
			Stack.pop();
			element* e = Control.top();
			Control.pop();
			Control.pop();
			Control.push(e);
		}
	} else if (Control.top()->type == "gamma"){
		if(Stack.top()->type == "lambda"){
			if(Stack.top()->child.size() == 1){
				string ch = Stack.top()->child[0];
				string child = "";

				int i = 4;
				while(ch[i] != '>'){
					child.push_back(ch[i]);
					i++;
				}
				int n = Stack.top()->num;

				envNode* controlEnvNode = new envNode();
				controlEnvNode->env = ++num_envs;
				controlEnvNode->parent = Stack.top()->env;
				/*if (Stack.top()->env){
					cout << "Parent of: " << controlEnvNode->env << " " << Stack.top()->env->env << endl;
				}*/
				Stack.pop();
				controlEnvNode->varMap[child] = Stack.top();
				//cout << "Mapping " << child << " to " << Stack.top()->type << " in env " << controlEnvNode->env << endl;
				//cout << "Parent of " << controlEnvNode->env << " is " << controlEnvNode->parent->env << endl; 

				Stack.pop();

				element* e = new element();
				e->type = "env";
				e->num = controlEnvNode->env;
				e->env = controlEnvNode;

				envStack.push(controlEnvNode);
				//cout << "Total envs 2: " << num_envs << endl;
				curEnv = controlEnvNode;
				Control.pop();
				Control.push(e);
				Stack.push(e);
				LoadControlStructure(n);
			} else {
				vector<string> children = Stack.top()->child;

				int n = Stack.top()->num;

				envNode* eNode = new envNode();
				eNode->env = ++num_envs;
				eNode->parent = Stack.top()->env;
				/*if (eNode->parent){
					cout << "Parent of: " << eNode->env << " " << eNode->parent->env << endl;
				}*/
				envStack.push(eNode);
				Stack.pop();
				//cout << "Total envs 3: " << num_envs << endl;

				element* e1 = Stack.top();
				int j;
				for(j = 0; j < children.size(); j++){
					int k;
					string childstring = "";
					for (k = 4; k < children[j].size()-1; k++){
						childstring.push_back(children[j][k]);
					}

					eNode->varMap[childstring] = e1->tau[j];
					/*if(e1->tau[j]->type == "string"){
						cout << "Mapping " << childstring << " to " << e1->tau[j]->child[0] << " in env " << eNode->env << endl;
					} else {
						cout << "Mapping " << childstring << " to " << e1->tau[j]->type << " in env " << eNode->env << endl;
					}*/
				}
				if (eNode->parent){
					//cout << "Parent of " << eNode->env << " is " << eNode->parent->env << endl;
				}
				Stack.pop();

				curEnv = eNode;

				element* e = new element();
				e->type = "env";
				e->num = eNode->env;
				e->env = eNode;
				
				Control.pop();
				Control.push(e);
				Stack.push(e);

				LoadControlStructure(n);
			}
		} else if (Stack.top()->type == "Print"){
			Control.pop();
			Stack.pop();
			Printout(Stack.top());
			Stack.pop();
			element* e = new element();
			e->type = "dummy";
			Stack.push(e);
		} else if (Stack.top()->type == "Stern"){
			Stack.pop();
			if(Stack.top()->type == "string"){
				string s = Stack.top()->child[0];
				element* e = new element();
				e->type = "string";
				e->child.push_back("");
				int j = 1;
				while(s[j] != '\0'){
					e->child[0].push_back(s[j]);
					j++;
				}
				Stack.pop();
				Control.pop();
				Stack.push(e);
			} else{
				Error(3);
			}
		} else if (Stack.top()->type == "Stem"){
			Stack.pop();
			if(Stack.top()->type == "string"){
				string s = Stack.top()->child[0];
				element* e = new element();
				e->type = "string";
				e->child.push_back("");
				e->child[0].push_back(s[0]);
				Stack.pop();
				Control.pop();
				Stack.push(e);
			} else{
				Error(3);
			}
		} else if (Stack.top()->type == "Conc"){
			Stack.pop();
			if(Stack.top()->type == "string"){
				element* e = new element();
				string s = Stack.top()->child[0];
				Stack.pop();
				if(Stack.top()->type == "string"){
					s += Stack.top()->child[0];
					Stack.pop();
				}
				e->type = "string";
				e->child.push_back(s);
				Control.pop();
				Control.pop();
				Stack.push(e);
			} else{
				Error(3);
			}
		} else if (Stack.top()->type == "Order"){
			Stack.pop();
			element* e = new element();
			e->type = "int";
			if(Stack.top()->type == "nil"){
				e->num = 0;
				Stack.pop();
				Stack.push(e);
			} else if (Stack.top()->type == "tau"){
				e->num = Stack.top()->num;
				Stack.pop();
				Stack.push(e);
			} else {
				Error(4);
			}
			Control.pop();
		} else if (Stack.top()->type == "Null"){
			Stack.pop();
			element* e = new element();
			if(Stack.top()->type == "nil"){
				e->type = "true";
				Stack.pop();
				Stack.push(e);
			} else if (Stack.top()->type == "tau"){
				if(Stack.top()->num == 0){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(4);
			}
			Control.pop();
		} else if (Stack.top()->type == "Y"){
			Stack.pop();
			element* e = new element();
			e->type = "eta";
			e->num = Stack.top()->num;
			e->child = Stack.top()->child;
			e->tau = Stack.top()->tau;
			e->env = Stack.top()->env;
			Stack.pop();
			Stack.push(e);
			Control.pop();
		} else if (Stack.top()->type == "eta"){
			element* e = new element();
			e->type = "lambda";
			e->child = Stack.top()->child;
			e->env = Stack.top()->env;
			e->num = Stack.top()->num;
			Stack.push(e);
			element* e1 = new element();
			e1->type = "gamma";
			Control.push(e1);
		} else if (Stack.top()->type == "tau"){
			element* e = Stack.top();
			Stack.pop();
			int num = Stack.top()->num;
			Stack.pop();
			Stack.push(e->tau[num-1]);
			Control.pop();
		} else if (Stack.top()->type == "Istuple"){
			Stack.pop();
			element* e = new element();
			if(Stack.top()->type == "tau" || Stack.top()->type == "nil"){
				e->type = "true";
			} else {
				e->type = "false";
			}
			Control.pop();
			Stack.pop();
			Stack.push(e);
		} else if (Stack.top()->type == "Isstring"){
			Stack.pop();
			element* e = new element();
			//cout << Stack.top()->type << endl;
			if(Stack.top()->type == "string"){
				e->type = "true";
			} else {
				e->type = "false";
			}
			Control.pop();
			Stack.pop();
			Stack.push(e);
		} else if (Stack.top()->type == "Isinteger"){
			Stack.pop();
			element* e = new element();
			if(Stack.top()->type == "int"){
				e->type = "true";
			} else {
				e->type = "false";
			}
			Control.pop();
			Stack.pop();
			Stack.push(e);
		} else if (Stack.top()->type == "Istruthvalue"){
			Stack.pop();
			element* e = new element();
			if(Stack.top()->type == "true" || Stack.top()->type == "false"){
				e->type = "true";
			} else {
				e->type = "false";
			}
			Control.pop();
			Stack.pop();
			Stack.push(e);
		} else if (Stack.top()->type == "Isfunction"){
			Stack.pop();
			element* e = new element();
			if(Stack.top()->type == "lambda"){
				e->type = "true";
			} else {
				e->type = "false";
			}
			Control.pop();
			Stack.pop();
			Stack.push(e);
		} else if (Stack.top()->type == "ItoS"){
			Stack.pop();
			element* e = Stack.top();
			e->type = "string";
			stringstream ss;
			ss << Stack.top()->num;
			e->child.push_back(ss.str());
			Stack.pop();
			Control.pop();
			Stack.push(e);
		} else {
			Control.pop();
		}
	} else if (Control.top()->type == "lambda"){
		element* e = Control.top();
		e->env = curEnv;
		Control.pop();
		Stack.push(e);
	} else if (Control.top()->type == "env"){
		element* e = Stack.top();
		Control.pop();
		Stack.pop();
		Stack.pop();
		Stack.push(e);
		envStack.pop();
		if(!envStack.empty()){
			curEnv = envStack.top();
		}
	} else if (Control.top()->type == "int"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "id"){
		string varName = Control.top()->child[0];
		envNode* eNode = envStack.top();

		//cout << "Searched in " << eNode->env;
		while(eNode->varMap.find(varName) == eNode->varMap.end()){
			if(eNode->parent){
				eNode = eNode->parent;
			} else {
				break;
			}
			//cout << " " << eNode->env << " ";
		}
		//cout << endl;
		element* e = new element();
		if(eNode->varMap.find(varName) != eNode->varMap.end()){
			e = eNode->varMap.find(varName)->second;
			//cout << "FOUND " << varName << endl;
		} else {
			e->type = Control.top()->child[0];
			//cout << "NOT FOUND " << varName << endl;
			eNode = envStack.top();
		}
		Stack.push(e);
		Control.pop();
	} else if (Control.top()->type == "tau"){
		element* e = Control.top();
		e->tau.clear();

		for(int i = 0; i < e->num; i++){
			e->tau.push_back(Stack.top());
			Stack.pop();
		}

		Control.pop();
		Stack.push(e);
	} else if (Control.top()->type == "true"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "false"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "nil"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "string"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "dummy"){
		Stack.push(Control.top());
		Control.pop();
	} else if (Control.top()->type == "or"){
		Control.pop();
		element* e = new element;
		if(Stack.top()->type == "true"){
			Stack.pop();
			if(Stack.top()->type == "true"){
				Stack.pop();
				e->type = "true";
				Stack.push(e);
			} else if (Stack.top()->type == "false") {
				Stack.pop();
				e->type = "true";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else if (Stack.top()->type == "false") {
			Stack.pop();
			if(Stack.top()->type == "true"){
				Stack.pop();
				e->type = "true";
				Stack.push(e);
			} else if (Stack.top()->type == "false") {
				Stack.pop();
				e->type = "false";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "&"){
		Control.pop();
		element* e = new element;
		if(Stack.top()->type == "true"){
			Stack.pop();
			if(Stack.top()->type == "true"){
				Stack.pop();
				e->type = "true";
				Stack.push(e);
			} else if (Stack.top()->type == "false") {
				Stack.pop();
				e->type = "false";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else if (Stack.top()->type == "false") {
			Stack.pop();
			if(Stack.top()->type == "true"){
				Stack.pop();
				e->type = "false";
				Stack.push(e);
			} else if (Stack.top()->type == "false") {
				Stack.pop();
				e->type = "false";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "not"){
		Control.pop();
		element* e = new element;
		if(Stack.top()->type == "true"){
			Stack.pop();
			e->type = "false";
			Stack.push(e);
		} else if (Stack.top()->type == "false") {
			Stack.pop();
			e->type = "true";
			Stack.push(e);
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "+"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				e->num += Stack.top()->num;
				Stack.pop();
				e->type = "int";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "-"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				e->num -= Stack.top()->num;
				Stack.pop();
				e->type = "int";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "*"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				e->num *= Stack.top()->num;
				Stack.pop();
				e->type = "int";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "/"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				e->num /= Stack.top()->num;
				Stack.pop();
				e->type = "int";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "**"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				e->num = pow(e->num, Stack.top()->num);
				Stack.pop();
				e->type = "int";
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "eq"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 == Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else if(Stack.top()->type == "string"){
			string s1 = Stack.top()->child[0];
			Stack.pop();
			if(Stack.top()->type == "string"){
				if(s1 == Stack.top()->child[0]){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "ne"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 != Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else if(Stack.top()->type == "string"){
			string s1 = Stack.top()->child[0];
			Stack.pop();
			if(Stack.top()->type == "string"){
				if(s1 != Stack.top()->child[0]){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "ls"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 < Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "le"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 <= Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "gr"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 > Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "ge"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			int num1 = Stack.top()->num;
			Stack.pop();
			if(Stack.top()->type == "int"){
				if(num1 >= Stack.top()->num){
					e->type = "true";
				} else {
					e->type = "false";
				}
				Stack.pop();
				Stack.push(e);
			} else {
				Error(2);
			}
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "neg"){
		Control.pop();
		element* e = new element();
		if(Stack.top()->type == "int"){
			e->num = (-1) * Stack.top()->num;
			Stack.pop();
			e->type = "int";
			Stack.push(e);
		} else {
			Error(2);
		}
	} else if (Control.top()->type == "aug"){
		if (Stack.top()->type == "nil"){
			Stack.pop();
			element* e = new element();
			e->type = "tau";
			e->num = 0;
			Stack.push(e);
		} else if (Stack.top()->type == "tau"){
			element *e = Stack.top();
			Stack.pop();
			e->tau.push_back(Stack.top());
			e->num++;
			Stack.pop();
			Control.pop();
			Stack.push(e);
		}
	} else if (Control.top()->type == "Y"){
		Stack.push(Control.top());
		Control.pop();
	} else {
		Control.pop();
		Stack.pop();
	}
}

void cse::Printout(element* e){
	if (e->type == "int"){
		cout << e->num;
	} else if (e->type == "string"){
		for(int i = 0; i < e->child[0].size(); i++){
			if(e->child[0][i] == '\\'){
				i++;
				if(e->child[0][i] == 'n'){
					cout << endl;
				} else if (e->child[0][i] == 't'){
					cout << '\t';
				} else {
					cout << e->child[0][i-1] << e->child[0][i];
				}
			} else {
				cout << e->child[0][i];
			}
		}
	} else if (e->type == "tau"){
		cout << "(";
		int i = 0;

		while(i < e->tau.size()){
			Printout(e->tau[i]);
			i++;
			if(i < e->tau.size()){
				cout << ", ";
			}
		}
		cout << ")";
	} else if (e->type == "lambda"){
		if (e->child.size() == 1){
			string s = "";

			for(int j = 4; j < e->child[0].size() - 1; j++){
				s.push_back(e->child[0][j]);
			}

			cout << "[lambda closure: " << s << ": " << e->num << "]";
		} else {
			cout << "[lambda closure: (null): " << e->num << "]";
		}
	} else {
		cout << e->type;
	}
}


void cse::cseMachine(){
	envNode* EnvNode = new envNode();
	EnvNode->env = 0;

	element* e = new element();
	e->type = "env";
	e->num = 0;
	e->env = EnvNode;

	envStack.push(EnvNode);
	num_envs = 0;
	curEnv = EnvNode;
	//cout << "Total envs 4: " << num_envs << endl;

	Control.push(e);
	Stack.push(e);

	element* e1 = new element();
	e1->type = "delta";
	e1->num = 0;

	LoadControlStructure(0);
	while(!Control.empty()){
		cseExecute();
	}
	cout << endl;
}

void cse::Error(int num){

}

void cse::run(){
	num_deltas = 0;
	parseTree(tree, 0);
}