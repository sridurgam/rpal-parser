all:
	g++ main.cpp parser.cpp lexer.cpp standardizer.cpp cse.cpp -o p2
cl:
	rm -f *.o p2