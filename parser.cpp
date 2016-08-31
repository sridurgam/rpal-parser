/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "parser.h"

// Initializes a lexer object, calls E, prints the ast 
// and closes the lexer object
void parser::run(string S){
	if(l.init(S)){
		EOFflag = 0;
		token.clear();
		E();

		//printAST();
		l.close();
	}
	return;
}

node* parser::getAst(){
	node* n;
	n = astStack.top();

	return n;
}

// Prints the ast tree for the top node on the stack. (Error if more than 
// one trees are present)
void parser::printAST(){
	node *n;

	n = astStack.top();
	astStack.pop();
	
	if(astStack.empty()){
		printTree(n, 0);
	}
	else {
		cout << "Error: Stack is not empty at the end" << endl;
	}
}

// Recursive function to print the tree in the given format.
void parser::printTree(node* n, int num){
	int i = 0;

	for(i = 0; i < num; i++){
		cout << ".";
	}

	cout << n->val << endl;

	if(n->left){
		printTree(n->left, num + 1);
	}

	if(n->right){
		printTree(n->right, num);
	}
}

// Takes a string and a number, pops as many trees as the number and
// builds a tree with the value as the string.
void parser::buildAST(string S, int num){
	node *n = new node();
	n->val = S;
	node *tempNode;
	tempNode = n;
	int tempNum = num;
	int isRightChild = 0;
	stack<node*> tempNodeStack;

	while(num > 0){
		if(!astStack.empty()){
			tempNodeStack.push(astStack.top());
			astStack.pop();
		} else {
			cout << "Pop request exceeds stack content" << endl;
			return;
		}
		num--;
	}

	while(!tempNodeStack.empty()){
		if(isRightChild == 0){
			n->left = tempNodeStack.top();
			tempNodeStack.pop();
			n = n->left;
			isRightChild = 1;
		} else {
			n->right = tempNodeStack.top();
			tempNodeStack.pop();
			n = n->right;
		}
	}

	astStack.push(tempNode);
}

// adv_Token contains any token that was read in advance. If adv_token is
// present, this functions populates the token variable from adv_Token.
// Else it gets the next token from the lexer. Stores the token and id 
// value in class variables.
void parser::NextToken(){
	if(EOFflag == 1){
		token = "";
		id = 0;
		adv_Token = "";
	}
	else {
		if (!adv_Token.empty()){
			token = adv_Token;
			adv_Token.clear();
			id = checkID(token);
		} else {
			token = l.getNextToken();
			if(token.empty()){
				id = checkID(token);
				EOFflag = 1;
			} else {
				id = checkID(token);
			}
		}
	}
}

//----------- Grammar functions block ----------------------//

int parser::E(){
	int p = 0;
	NextToken();
	if (id == 1){				//let
		p = p + D();
		NextToken();
		if (id == 2){			//in
			p = p + E();
			buildAST("let", 2);
		} else {
			Error(1);
			return 0;
		}
	} else {
		if (id == 24){			//fn
			p = p + Vb();
			NextToken();
			if (id == 25){		// .
				p = p + E();
				buildAST("lambda", p);
			} else {
				Error(3);
				return 0;
			}
		} else {
			adv_Token = token;
			p = p + Ew();
		}
	}
	return 1;
}

int parser::Ew(){
	int p = 0;
	p = p + T();
	NextToken();
	if(id == 3){				// where
		p = p + Dr();
		buildAST("where", 2);
	} else {
		adv_Token = token;
	}
	return 1;
}

int parser::T(){
	int p = 0;
	p = p + Ta();
	NextToken();
	while(id == 23){			// ,
		p = p + Ta();
		NextToken();
	}
	if (p > 1){
		buildAST("tau", p);
	}
	adv_Token = token;
	return 1;
}

int parser::Ta(){
	int p = 0;
	p = p + Tc();
	NextToken();
	while(id == 4){				// aug
		p = p + Tc();
		NextToken();
		buildAST("aug", 2);
	}
	adv_Token = token;
	return 1;
}

int parser::Tc(){
	int p = 0;
	p = p + B();
	NextToken();
	if(id == 30){				// ->
		p = p + Tc();
		NextToken();
		if(id == 31){			// |
			p = p + Tc();
			buildAST("->", 3);
		} else {
			Error(4);
			return 0;
		}
	} else {
		adv_Token = token;
	}
	return 1;
}

int parser::B(){
	int p = 0;
	p = p + Bt();
	NextToken();
	while(id == 5){				// or
		p = p + Bt();
		NextToken();
		buildAST("or", 2);
	}
	adv_Token = token;
	return 1;
}

int parser::Bt(){
	int p = 0;
	p = p + Bs();
	NextToken();
	while(id == 32){			// &
		p = p + Bs();
		NextToken();
		buildAST("&", 2);
	}
	adv_Token = token;
	return 1;
}

int parser::Bs(){
	int p = 0;
	NextToken();
	if (id == 6){				// not
		p = p + Bp();
		buildAST("not", 1);
	} else {
		adv_Token = token;
		p = p + Bp();
	}
	return 1;
}

int parser::Bp(){
	int p = 0;
	p = p + A();
	NextToken();
	if (id == 7 || id == 33){			// gr >
		p = p + A();
		buildAST("gr", 2);
	} 
	else if (id == 8 || id == 34){		// ge >=
		p = p + A();
		buildAST("ge", 2);
	}
	else if (id == 9 || id == 35){		// ls <
		p = p + A();
		buildAST("ls", 2);
	}
	else if (id == 10 || id == 36){		// ls <=
		p = p + A();
		buildAST("le", 2);
	}
	else if (id == 11){					// eq
		p = p + A();
		buildAST("eq", 2);
	} 
	else if (id == 12){					// ne
		p = p + A();
		buildAST("ne", 2);
	}
	else {
		adv_Token = token;
	}
	return 1;
}

int parser::A(){
	int p = 0;
	NextToken();

	if(id == 37){
		p = p + At();
	}
	else if(id == 38){
		p = p + At();
		buildAST("neg", 1);
	}
	else{
		adv_Token = token;
		p = p + At();
	}

	NextToken();
	while(id == 37 || id == 38){
		string temp = token;
		p = p + At();
		buildAST(temp, 2);
		NextToken();
	}
	adv_Token = token;
	return 1;
}

int parser::At(){
	int p = 0;
	p = p + Af();
	NextToken();
	while(true){
		if (id == 39){				// *
			p = p + Af();
			buildAST("*", 2);
		}
		else if (id == 40){			// /
			p = p + Af();
			buildAST("/", 2);
		}
		else {
			adv_Token = token;
			break;
		}
		NextToken();
	}
	return 1;
}

int parser::Af(){
	int p = 0;
	p = p + Ap();
	NextToken();
	if(id == 41){
		p = p + Af();
		buildAST("**", 2);		// **
	} else {
		adv_Token = token;
	}
	return 1;
}

int parser::Ap(){
	int p = 0;
	p = p + R();
	NextToken();
	while(id == 42){				// @
		NextToken();
		if (id == 26){
			buildAST("<ID:" + token + ">", 0);
			p++;
			NextToken();
			adv_Token = token;
			p = p + R();
			buildAST("@", 3);
		} else {
			Error(5);
		}
		NextToken();
	}
	adv_Token = token;
	return 1;
}

int parser::R(){
	int p = 0;
	p = p + Rn();
	NextToken();
	while(id == 13 || id == 14 || id == 15 || id == 16 || id == 20 || id == 26 || id == 27 || id == 29){
		adv_Token = token;
		p = p + Rn();
		buildAST("gamma", 2);
		NextToken();
	}
	adv_Token = token;
	return 1;
}

int parser::Rn(){
	string temp;
	int p = 0;
	NextToken();
	switch(id){
		case 13:
			buildAST("<true>", 0);
			break;
		case 14:
			buildAST("<false>", 0);
			break;
		case 15:
			buildAST("<nil>", 0);
			break;
		case 16:
			buildAST("<dummy>", 0);
			break;
		case 20:
			p = p + E();
			NextToken();
			if (id != 21){
				Error(7);
				return 1;
			}
			break;
		case 26:
			temp = "<ID:" + token + ">";
			buildAST(temp, 0);
			break;
		case 27:
			temp = "<INT:" + token + ">";
			buildAST(temp, 0);
			break;
		case 29:
			temp = "<STR:" + token + ">";
			buildAST(temp, 0);
			break;
		case 0:
			break;
		default:
			Error(6);
	}
	return 1;
}

int parser::D(){
	int p = 0;
	p = p + Da();
	NextToken();
	if(id == 17){
		p = p + D();
		buildAST("within", 2);
	} else {
		adv_Token = token;
	}
	return 1;
}

int parser::Da(){
	int p = 0;
	p = p + Dr();
	NextToken();
	while (id == 18){
		p = p + Dr();
		NextToken();
	}
	if (p > 1){
		buildAST("and", p);
	}
	adv_Token = token;
	return 1;
}

int parser::Dr(){
	int p = 0;
	NextToken();
	if(id == 19){
		p = p + Db();
		buildAST("rec", 1);
	} else {
		adv_Token = token;
		p = p + Db();
	}
	return 1;
}

int parser::Db(){
	int p = 0;
	NextToken();
	if(id == 20){									// (
		p = p + D();
		NextToken();
		if (id != 21){								// )
			Error(7);
			return 0;
		}
	}
	else if (id == 26){								// IDENTIFIER
		buildAST("<ID:" + token + ">", 0);
		p++;
		NextToken();
		if (id == 26 || id == 20){					// ID or (
			adv_Token = token;
			p = p + Vb();
			NextToken();
			if (id != 43){							// =
				Error(9);
			}
			p = p + E();
			buildAST("function_form", p);
		} else if (id == 43){						// =
			p = p + E();
			buildAST("=", 2);
		} else if (id == 23){						// ,
			adv_Token = token;
			p = p + Vl(1);
			NextToken();
			if(id == 43){
				E();
				buildAST("=", 2);
			} else {
				Error(12);
				return 0;
			}
		}
	}
	else {
		Error(8);
	}
	return 1;
}

int parser::Vb(){
	int iter = 0;
	int p = 0;

	NextToken();

	do{
		if(id == 20){
			NextToken();
			if(id == 21){
				buildAST("()", 0);
			}
			else {
				adv_Token = token;
				Vl(0);
				NextToken();
				if(id != 21){
					Error(7);
				}
			}
		}
		else if(id != 26){
			Error(11);
			return iter;
		}
		else {
			buildAST("<ID:" + token + ">", 0);
		}
		NextToken();
		iter++;
	} while(id == 26 || id == 20);

	adv_Token = token;
	return iter;
}

int parser::Vl(int flag){
	int p = 1;
	if (flag == 0){
		NextToken();
		if(id != 26){
			Error(5);
			return 0;
		}
		buildAST("<ID:" + token + ">", 0);
	}
	NextToken();

	while(id == 23){
		NextToken();
		if (id != 26){
			Error(5);
			return 0;
		}
		buildAST("<ID:" + token + ">", 0);
		NextToken();
		p++;
	}
	adv_Token = token;
	if(p > 1){
		buildAST(",", p);
	}
	return 1;
}

//------------ End of Grammar Functions Block ---------------------//

// Error handler function.
void parser::Error(int num){
	switch(num){
		case 1:
			cout << "Error 1: Expected 'in' token. Received something else." << endl;
			break;
		case 2:
			cout << "Error 2: Unidentified token received" << endl;
			break;
		case 3:
			cout << "Error 3: Expected '.' token. Received something else." << endl;
			break;
		case 4:
			cout << "Error 4: Expected '|' token. Received something else." << endl;
			break;
		case 5:
			cout << "Error 5: Expected identifier token. Received something else." << endl;
			break;
		case 6:
			cout << "Error 6: Expected one of identifier, true, false, nil, '('' or dummy. Received something else." << endl;
			break;
		case 7:
			cout << "Error 7: Expected ')' token. Received something else." << endl;
			break;
		case 8:
			cout << "Error 8: Expected '(' token or identifier. Received something else." << endl;
			break;
		case 9:
			cout << "Error 9: Expected '=' token. Received something else." << endl;
			break;
		case 10:
			cout << "Error 10: Expected ',' token. Received something else." << endl;
			break;
		case 11:
			cout << "Error 11: Expected '(' or identifier. Received something else." << endl;
			break;
		case 12:
			cout << "Error 12: Expected '=' token. Received something else." << endl;
			break;
		default:
			return;
	}
	return;
}

// Each type of token(keyword, symbol or identifier/integer/string) has its own
// distinctive id value. This id value (int) is used to classify the token type
// Takes a token and returns the id value for the token.
int parser::checkID(string token){
	if (token.empty()){
		return 0;
	}
	if (token == "let"){
		return 1;
	}
	else if (token == "in"){
		return 2;
	}
	else if (token == "where"){
		return 3;
	}
	else if (token == "aug"){
		return 4;
	}
	else if (token == "or"){
		return 5;
	}
	else if (token == "not"){
		return 6;
	}
	else if (token == "gr"){
		return 7;
	}
	else if (token == "ge"){
		return 8;
	}
	else if (token == "ls"){
		return 9;
	}
	else if (token == "le"){
		return 10;
	}
	else if (token == "eq"){
		return 11;
	}
	else if (token == "ne"){
		return 12;
	}
	else if (token == "true"){
		return 13;
	}
	else if (token == "false"){
		return 14;
	}
	else if (token == "nil"){
		return 15;
	}
	else if (token == "dummy"){
		return 16;
	}
	else if (token == "within"){
		return 17;
	}
	else if (token == "and"){
		return 18;
	}
	else if (token == "rec"){
		return 19;
	}
	else if (token == "("){
		return 20;
	}
	else if (token == ")"){
		return 21;
	}
	else if (token == ";"){
		return 22;
	}
	else if (token == ","){
		return 23;
	}
	else if (token == "fn"){
		return 24;
	}
	else if (token == "."){
		return 25;
	}
	else if (token == "->"){
		return 30;
	}
	else if (token == "|"){
		return 31;
	}
	else if (token == "&"){
		return 32;
	}
	else if (token == ">"){
		return 33;
	}
	else if (token == ">="){
		return 34;
	}
	else if (token == "<"){
		return 35;
	}
	else if (token == "<="){
		return 36;
	}
	else if (token == "+"){
		return 37;
	}
	else if (token == "-"){
		return 38;
	}
	else if (token == "*"){
		return 39;
	}
	else if (token == "/"){
		return 40;
	}
	else if (token == "**"){
		return 41;
	}
	else if (token == "@"){
		return 42;
	}
	else if (token == "="){
		return 43;
	}
	else if(token[0] == 'A' || token[0] == 'B' || token[0] == 'C' || token[0] == 'D' || token[0] == 'E' || token[0] == 'F' || token[0] == 'G' || token[0] == 'H' || token[0] == 'I' || token[0] == 'J' || token[0] == 'K' || token[0] == 'L' || token[0] == 'M' || token[0] == 'N' || token[0] == 'O' || token[0] == 'P' || token[0] == 'Q' || token[0] == 'R' || token[0] == 'S' || token[0] == 'T' || token[0] == 'U' || token[0] == 'V' || token[0] == 'W' || token[0] == 'X' || token[0] == 'Y' || token[0] == 'Z' || token[0] == 'a' || token[0] == 'b' || token[0] == 'c' || token[0] == 'd' || token[0] == 'e' || token[0] == 'f' || token[0] == 'g' || token[0] == 'h' || token[0] == 'i' || token[0] == 'j' || token[0] == 'k' || token[0] == 'l' || token[0] == 'm' || token[0] == 'n' || token[0] == 'o' || token[0] == 'p' || token[0] == 'q' || token[0] == 'r' || token[0] == 's' || token[0] == 't' || token[0] == 'u' || token[0] == 'v' || token[0] == 'w' || token[0] == 'x' || token[0] == 'y' || token[0] == 'z'){
		return 26;						// IDENTIFIER
	}
	else if (token[0] == '0' || token[0] == '1' || token[0] == '2' || token[0] == '3' || token[0] == '4' || token[0] == '5' || token[0] == '6' || token[0] == '7' || token[0] == '8' || token[0] == '9'){
		return 27;						// INTEGER
	} 
	else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token == "<" || token[0] == '>' || token[0] == '&' || token[0] == '.' || token[0] == '@' || token[0] == '/' || token[0] == ':' || token[0] == '=' || token[0] == '~' || token[0] == '|' || token[0] == '$' || token[0] == '!' || token[0] == '#' || token[0] == '%' || token[0] == '^' || token[0] == '_' || token[0] == '[' || token[0] == ']' || token[0] == '{' || token[0] == '}' || token[0] == '"' || token[0] == '`' || token[0] == '?'){
		return 28;						// OPERATOR
	}
	else if (token[0] == '\''){
		return 29;						// STRING
	}
	Error(2);
	return -1;							// UNIDENTIFIED
}