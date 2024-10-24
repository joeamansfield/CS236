#ifndef LEXER_H
#define LEXER_H
#include "Automata.h"
#include "Token.h"
#include <string>
#include <vector>



class Lexer {
private:
	int lineNumber = 1;
	std::vector<Token*> tokens;
	int maxRead = 0;
public:
	Lexer() {}
	~Lexer() {}
	std::vector<Token*> Lex(std::string& input) {
		enum Machine {
			Matcher_Automaton,
			Block_Comment_Automaton,
			Line_Comment_Automaton,
			String_Automaton,
			ID_Automaton,
			Undefined_Comment,
			Undefined_String
		};
		Machine maxMachine{};
		AutomatonMatcher matcher;
		BlockCommentAtomaton blockComment;
		LineCommentAutomaton lineComment;
		StringAutomaton stringMatcher;
		IDAutomaton IDMatcher;
		UndefinedComment undefinedCommentMatcher;
		UndefinedString undefinedStringMatcher;

		while (input.size() > 0) {
			for (int i = 0; (input.size() > 0) && (i <= Undefined_String); ++i) {
				while (isspace(input.at(0))) {
					if (input.at(0) == '\n') {
						++lineNumber;
					}
					input.erase(0, 1);
					if (input.size() == 0) { 
						break; 
					}
				}
				if (input.size() == 0) {
					break;
				}
				if (i == Matcher_Automaton) {
					int x = matcher.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = Matcher_Automaton;
					}
				}
				if (i == Block_Comment_Automaton) {
					int x = blockComment.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = Block_Comment_Automaton;
					}
				}
				if (i == Line_Comment_Automaton) {
					int x = lineComment.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = Line_Comment_Automaton;
					}
				}
				if (i == String_Automaton) {
					int x = stringMatcher.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = String_Automaton;
					}
				}
				if (i == ID_Automaton) {
					int x = IDMatcher.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = ID_Automaton;
					}
				}
				if (i == Undefined_Comment) {
					int x = undefinedCommentMatcher.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = Undefined_Comment;
					}
				}
				if (i == Undefined_String) {
					int x = undefinedStringMatcher.Read(input);
					if (x > maxRead) {
						maxRead = x;
						maxMachine = Undefined_String;
					}
				}
			}
			Token* newToken;
			if (maxRead > 0) {
				if (maxMachine == Matcher_Automaton) {
					newToken = matcher.CreateToken(input, lineNumber);
				}
				if (maxMachine == Block_Comment_Automaton) {
					newToken = blockComment.CreateToken(input, lineNumber);
					lineNumber += blockComment.NewLinesRead();
				}
				if (maxMachine == Line_Comment_Automaton) {
					newToken = lineComment.CreateToken(input, lineNumber);
				}
				if (maxMachine == String_Automaton) {
					newToken = stringMatcher.CreateToken(input, lineNumber);
					lineNumber += stringMatcher.NewLinesRead();
				}
				if (maxMachine == ID_Automaton) {
					newToken = IDMatcher.CreateToken(input, lineNumber);
				}
				if (maxMachine == Undefined_Comment) {
					newToken = undefinedCommentMatcher.CreateToken(input, lineNumber);
					lineNumber += undefinedCommentMatcher.NewLinesRead();
				}
				if (maxMachine == Undefined_String) {
					newToken = undefinedStringMatcher.CreateToken(input, lineNumber);
					lineNumber += undefinedStringMatcher.NewLinesRead();
				}
			}
			else if (input.size() > 0) {
				maxRead = 1;
				newToken = new Token(UNDEFINED, input.substr(0, 1), lineNumber);
			}
			if (input.size() == 0) { break; }
			tokens.push_back(newToken);
			input.erase(0, maxRead);
			maxRead = 0;
		}
		Token* eofToken = new Token(ENDOF, "", lineNumber);
		tokens.push_back(eofToken);
		return tokens;
	}
};
#endif