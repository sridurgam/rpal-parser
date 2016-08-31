/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "lexer.h"
#include "node.h"
#include <stack>

using namespace std;

class parser{
	private:
		lexer l;
		string adv_Token;
		string token;
		int id;
		stack<node*> astStack;
		int EOFflag;
	public:
		node* getAst();
		void run(string S);
		void printAST();
		void printTree(node* n, int num);
		void buildAST(string S, int num);
		int E();
		int Ew();
		int T();
		int Ta();
		int Tc();
		int B();
		int Bt();
		int Bs();
		int Bp();
		int A();
		int At();
		int Af();
		int Ap();
		int R();
		int Rn();
		int D();
		int Da();
		int Dr();
		int Db();
		int Vb();
		int Vl(int flag);
		void Error(int num);
		int checkID(string token);
		void NextToken();
};