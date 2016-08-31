/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "parser.h"
#include "standardizer.h"
#include "cse.h"

int main(int argc, char* argv[]){
	parser p;
	node* n;
	standardizer s;
	cse C;

/*	if(argc == 3){
		p.run(argv[2]);

		n = p.getAst();

		p.printAST();
	} else {
*/		p.run(argv[1]);

		n = p.getAst();

		s.run(n);

		n = s.getTree();
		C.init(n);
		C.run();
		C.cseMachine();
//	}

	return 0;
}