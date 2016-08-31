/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class lexer{
	private:
		ifstream file;
		char adv_Token;
	public:
		int init(string S);
		string getNextToken();
		void close();
};
