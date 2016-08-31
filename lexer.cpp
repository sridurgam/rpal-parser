/*
Name: Sai Srivatsav Durgam
UFID: 56171929
*/

#include "lexer.h"

// Initializes a lexer object with a given file.
int lexer::init(string S){
	try{
		file.open(S.c_str());
	} 
	catch(exception& e){
		cerr << "Error opening file" << endl;
		return 0;
	}
	adv_Token = '\0';
	return 1;
}

// Parses the file for the next token and returns the token.
string lexer::getNextToken(){
	char x = '\0';
	string word;
	word.clear();

	// adv_Token stores any character that is read ahead.
	if(adv_Token == '\0'){
		if(!file.eof()){
			x = file.get();
		} else {
			return "";
		}
	} else {
		x = adv_Token;
		adv_Token = '\0';
	}

	// Throw the empty spaces.
	if (x == ' ' || x == '\t' || x == '\n'){
		return getNextToken();
	}

	if(x == '('){
		return "(";
	}

	if(x == ')'){
		return ")";
	}

	if(x == ';'){
		return ";";
	}

	if(x == ','){
		return ",";
	}

	// Check for integers
	if(x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9'){
		word = word + x;
		if(!file.eof()){
			x = file.get();
		} else {
			return "";
		}
		while(x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9'){
			word = word + x;
			if(!file.eof()){
				x = file.get();
			} else {
				return "";
			}
		}
		adv_Token = x;
		return word;
	}

	// Check for Identifiers
	if(x == 'A' || x == 'B' || x == 'C' || x == 'D' || x == 'E' || x == 'F' || x == 'G' || x == 'H' || x == 'I' || x == 'J' || x == 'K' || x == 'L' || x == 'M' || x == 'N' || x == 'O' || x == 'P' || x == 'Q' || x == 'R' || x == 'S' || x == 'T' || x == 'U' || x == 'V' || x == 'W' || x == 'X' || x == 'Y' || x == 'Z' || x == 'a' || x == 'b' || x == 'c' || x == 'd' || x == 'e' || x == 'f' || x == 'g' || x == 'h' || x == 'i' || x == 'j' || x == 'k' || x == 'l' || x == 'm' || x == 'n' || x == 'o' || x == 'p' || x == 'q' || x == 'r' || x == 's' || x == 't' || x == 'u' || x == 'v' || x == 'w' || x == 'x' || x == 'y' || x == 'z'){
		word = word + x;
		if(!file.eof()){
			x = file.get();
		}  else {
			return "";
		}
		while(x == 'A' || x == 'B' || x == 'C' || x == 'D' || x == 'E' || x == 'F' || x == 'G' || x == 'H' || x == 'I' || x == 'J' || x == 'K' || x == 'L' || x == 'M' || x == 'N' || x == 'O' || x == 'P' || x == 'Q' || x == 'R' || x == 'S' || x == 'T' || x == 'U' || x == 'V' || x == 'W' || x == 'X' || x == 'Y' || x == 'Z' || x == 'a' || x == 'b' || x == 'c' || x == 'd' || x == 'e' || x == 'f' || x == 'g' || x == 'h' || x == 'i' || x == 'j' || x == 'k' || x == 'l' || x == 'm' || x == 'n' || x == 'o' || x == 'p' || x == 'q' || x == 'r' || x == 's' || x == 't' || x == 'u' || x == 'v' || x == 'w' || x == 'x' || x == 'y' || x == 'z' || x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9' | x == '_'){
			word = word + x;
			if(!file.eof()){
				x = file.get();
			}
		}
		adv_Token = x;
		return word;
	}

	// Check for operators
	if(x == '+' || x == '-' || x == '*' || x == '<' || x == '>' || x == '&' || x == '.' || x == '@' || x == '/' || x == ':' || x == '=' || x == '~' || x == '|' || x == '$' || x == '!' || x == '#' || x == '%' || x == '^' || x == '_' || x == '[' || x == ']' || x == '{' || x == '}' || x == '"' || x == '`' || x == '?'){
		if (x == '/'){
			word = word + x;
			if(!file.eof()){
				x = file.get();
			}  else {
				return "";
			}
			if (x == '/'){					// Anything following this is a comment.
				word.clear();
				while(x == '\'' || x == '(' || x == ')' || x == ';' || x == ',' || x == '\\' || x == ' ' || x == '\t' || x == 'A' || x == 'B' || x == 'C' || x == 'D' || x == 'E' || x == 'F' || x == 'G' || x == 'H' || x == 'I' || x == 'J' || x == 'K' || x == 'L' || x == 'M' || x == 'N' || x == 'O' || x == 'P' || x == 'Q' || x == 'R' || x == 'S' || x == 'T' || x == 'U' || x == 'V' || x == 'W' || x == 'X' || x == 'Y' || x == 'Z' || x == 'a' || x == 'b' || x == 'c' || x == 'd' || x == 'e' || x == 'f' || x == 'g' || x == 'h' || x == 'i' || x == 'j' || x == 'k' || x == 'l' || x == 'm' || x == 'n' || x == 'o' || x == 'p' || x == 'q' || x == 'r' || x == 's' || x == 't' || x == 'u' || x == 'v' || x == 'w' || x == 'x' || x == 'y' || x == 'z' || x == '+' || x == '-' || x == '*' || x == '<' || x == '>' || x == '&' || x == '.' || x == '@' || x == '/' || x == ':' || x == '=' || x == '~' || x == '|' || x == '$' || x == '!' || x == '#' || x == '%' || x == '^' || x == '_' || x == '[' || x == ']' || x == '{' || x == '}' || x == '"' || x == '`' || x == '?' || x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9'){
					if(!file.eof()){
						x = file.get();
					}  else {
						return "";
					}
				}
				if (x != '\n'){
					return "";
				} else {
					return getNextToken();
				}
			} else {
				/*word = word + x;
				if(!file.eof()){
					x = file.get();
				}   else {
					return "";
				}*/
				while(x == '+' || x == '-' || x == '*' || x == '<' || x == '>' || x == '&' || x == '.' || x == '@' || x == '/' || x == ':' || x == '=' || x == '~' || x == '|' || x == '$' || x == '!' || x == '#' || x == '%' || x == '^' || x == '_' || x == '[' || x == ']' || x == '{' || x == '}' || x == '"' || x == '`' || x == '?'){
					word = word + x;
					if(!file.eof()){
						x = file.get();
					}   else {
						return "";
					}
				}
				adv_Token = x;
				return word;
			}
		} else {
			word = word + x;
			if(!file.eof()){
				x = file.get();
			}   else {
				return "";
			}
			while(x == '+' || x == '-' || x == '*' || x == '<' || x == '>' || x == '&' || x == '.' || x == '@' || x == '/' || x == ':' || x == '=' || x == '~' || x == '|' || x == '$' || x == '!' || x == '#' || x == '%' || x == '^' || x == '_' || x == '[' || x == ']' || x == '{' || x == '}' || x == '"' || x == '`' || x == '?'){
				word = word + x;
				if(!file.eof()){
					x = file.get();
				}   else {
					return "";
				}
			}
			adv_Token = x;
			return word;
		}
	}

	// Check for Strings
	if (x == '\''){
		word = word + x;
		if(!file.eof()){
			x = file.get();
		}   else {
			return "";
		}

		while(x == '\\' || x == '(' || x == ')' || x == ';' || x == ',' || x == ' ' || x == 'A' || x == 'B' || x == 'C' || x == 'D' || x == 'E' || x == 'F' || x == 'G' || x == 'H' || x == 'I' || x == 'J' || x == 'K' || x == 'L' || x == 'M' || x == 'N' || x == 'O' || x == 'P' || x == 'Q' || x == 'R' || x == 'S' || x == 'T' || x == 'U' || x == 'V' || x == 'W' || x == 'X' || x == 'Y' || x == 'Z' || x == 'a' || x == 'b' || x == 'c' || x == 'd' || x == 'e' || x == 'f' || x == 'g' || x == 'h' || x == 'i' || x == 'j' || x == 'k' || x == 'l' || x == 'm' || x == 'n' || x == 'o' || x == 'p' || x == 'q' || x == 'r' || x == 's' || x == 't' || x == 'u' || x == 'v' || x == 'w' || x == 'x' || x == 'y' || x == 'z' || x == '+' || x == '-' || x == '*' || x == '<' || x == '>' || x == '&' || x == '.' || x == '@' || x == '/' || x == ':' || x == '=' || x == '~' || x == '|' || x == '$' || x == '!' || x == '#' || x == '%' || x == '^' || x == '_' || x == '[' || x == ']' || x == '{' || x == '}' || x == '"' || x == '`' || x == '?' || x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9'){
			if (x == '\\'){
				word = word + x;
				if(!file.eof()){
					x = file.get();
				}   else {
					return "";
				}
				switch(x){
					case 't':
						word = word + x;
						if(!file.eof()){
							x = file.get();
						} else {
							return "";
						}
						break;
					case 'n':
						word = word + x;
						if(!file.eof()){
							x = file.get();
						} else {
							return "";
						}
						break;
					case '\\':
						word = word + x;
						if(!file.eof()){
							x = file.get();
						} else {
							return "";
						}
						break;
					case '\'':
						word = word + x;
						if(!file.eof()){
							x = file.get();
						} else {
							return "";
						}
						break;
					default:
						return "";
				}
			} else {
				word = word + x;
				if(!file.eof()){
					x = file.get();
				} else {
					return "";
				}
			}
		}

		if (x == '\''){
			word = word + x;
			return word;
		} else {
			return "";
		}
	}

	return "";
}

// Closes the lexer object
void lexer::close(){
	try{
		file.close();
	}
	catch(exception& e){

	}
}
