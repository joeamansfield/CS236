#include "Lexer.h"
#include "Token.h"
#include "Parser.h"
#include "Grammar.h"
#include "Interpreter.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

void printToken(Token*);

int main(int argc, char** argv) {
	std::ifstream inFile(argv[1]);
	if (!inFile) {
		std::cerr << "Could not open " << argv[1];
		return 2;
	}
	
	std::string inputString = "";
	while (!inFile.eof()) {
		std::string inputLine;
		getline(inFile, inputLine);
		inputString.append(inputLine + "\n");
	}
	inputString.pop_back();
	Lexer lexing;
	std::vector<Token*> tokens = lexing.Lex(inputString);
	Parser parsing(tokens);
	DatalogProgram dataling;
	tokens = parsing.clearComments();
	if (parsing.isDatalogProgram()) {
		dataling = parsing.parse();
	}
	else {
		std::cout << "Failure to parse!\n  ";
		printToken(tokens.at(parsing.currentToken));
		return 1;
	}
	Interpreter project3interpret(&dataling);
	project3interpret.interpret();
	std::cout << project3interpret.toaster();
	//print(tokens);
	for (int i = 0; i < static_cast<int>(tokens.size()); ++i) {
		delete tokens.at(i);
	}
	return 0;
}

void printToken(Token* token) {
	std::vector<std::string> tokenTypes{
		"COMMA",
		"PERIOD",
		"Q_MARK",
		"LEFT_PAREN",
		"RIGHT_PAREN",
		"COLON",
		"COLON_DASH",
		"MULTIPLY",
		"ADD",
		"SCHEMES",
		"FACTS",
		"RULES",
		"QUERIES",
		"ID",
		"STRING",
		"COMMENT",
		"UNDEFINED",
		"EOF"
	};
	
	std::cout << "(" << tokenTypes.at(token->getCategory()) << ",\"" << token->getTokenContent() << "\"," << token->getLineNumber() << ")\n";
}





