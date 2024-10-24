#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	MULTIPLY,
	ADD,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	COMMENT,
	UNDEFINED,
	ENDOF
};

class Token {
private:
	int category;
	int lineNumber = 0;
	std::string tokenContent = "";
public:
	Token(int newType, std::string newContent, int newLineNumber) {
		category = newType;
    	lineNumber = newLineNumber;
    	tokenContent = newContent;
	}
	~Token() {}
	int getCategory() { return category; }
	int getLineNumber() { return lineNumber; }
	std::string getTokenContent() { return tokenContent; }
};

#endif