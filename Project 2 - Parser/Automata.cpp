#include "Automata.h"
int AutomatonMatcher::Read(const std::string& input) {
	if (input.substr(0, 1) == ",") {return 1; }
	else if (input.substr(0, 1) == ".") {return 1; }
	else if (input.substr(0, 1) == "?") {return 1; }
	else if (input.substr(0, 1) == "(") {return 1; }
	else if (input.substr(0, 1) == ")") {return 1; }
	else if (input.substr(0, 2) == ":-") { return 2; }
	else if (input.substr(0, 1) == ":") {return 1; }
	else if (input.substr(0, 1) == "*") {return 1; }
	else if (input.substr(0, 1) == "+") {return 1; }
	else if (input.substr(0, 7) == "Schemes") {return 7; }
	else if (input.substr(0, 5) == "Facts") {return 5; }
	else if (input.substr(0, 5) == "Rules") {return 5; }
	else if (input.substr(0, 7) == "Queries") {return 7; }
	return 0;
}

Token* AutomatonMatcher::CreateToken(const std::string& input, int lineNumber) {
	if (input.substr(0, 1) == ",") {return new Token(COMMA, ",", lineNumber); }
	else if (input.substr(0, 1) == ".") {return new Token(PERIOD, ".", lineNumber); }
	else if (input.substr(0, 1) == "?") {return new Token(Q_MARK, "?", lineNumber); }
	else if (input.substr(0, 1) == "(") {return new Token(LEFT_PAREN, "(", lineNumber); }
	else if (input.substr(0, 1) == ")") {return new Token(RIGHT_PAREN, ")", lineNumber); }
	else if (input.substr(0, 2) == ":-") { return new Token(COLON_DASH, ":-", lineNumber); }
	else if (input.substr(0, 1) == ":") {return new Token(COLON, ":", lineNumber); }
	else if (input.substr(0, 1) == "*") {return new Token(MULTIPLY, "*", lineNumber); }
	else if (input.substr(0, 1) == "+") {return new Token(ADD, "+", lineNumber); }
	else if (input.substr(0, 7) == "Schemes") {return new Token(SCHEMES, "Schemes", lineNumber); }
	else if (input.substr(0, 5) == "Facts") {return new Token(FACTS, "Facts", lineNumber); }
	else if (input.substr(0, 5) == "Rules") {return new Token(RULES, "Rules", lineNumber); }
	else if (input.substr(0, 7) == "Queries") {return new Token(QUERIES, "Queries", lineNumber);}
	return new Token(QUERIES, "error", -1);
}