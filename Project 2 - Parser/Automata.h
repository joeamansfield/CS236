#ifndef AUTOMATA_H
#define AUTOMATA_H
#include "Token.h"
#include <string>

class Automata {
protected:
	int newLines = 0;
	int category = COMMA;
public:
	Automata() {}
	~Automata() {}

	virtual int Read(const std::string& input) = 0;
	int NewLinesRead() { return newLines; }
	virtual Token* CreateToken(const std::string& input, int lineNumber) = 0;
};

class AutomatonMatcher : public Automata {
private:
public:
	AutomatonMatcher() {
		category = COMMA;
	}
	~AutomatonMatcher() {}
	int Read(const std::string& input);
	Token* CreateToken(const std::string& input, int lineNumber);
};

class BlockCommentAtomaton : public Automata {
private:
public:
	BlockCommentAtomaton() { category = COMMENT; }
	~BlockCommentAtomaton() {}
	int Read(const std::string& input) {
		newLines = 0;
		if (input.substr(0, 2) == "#|") {
			int i = 0;
			for (; (input.substr(i, 2) != "|#") && (static_cast<int>(input.size()) > i); ++i) {
				if (input.at(i) == '\n') { newLines++; }
				if ((static_cast<int>(input.size()) == (i + 2)) && (input.substr((i + 1), 2) != "|#")) { return 0; }
			}
			i = i + 2;
			return i;
		}
		else { return 0; }
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		int i = 0;
		for (; (input.substr(i, 2) != "|#") && (static_cast<int>(input.size()) > i); ++i) {}
		i = i + 2;
		return new Token(COMMENT, input.substr(0, i), lineNumber);
	
	}
};

class LineCommentAutomaton : public Automata {
private:
public:
	LineCommentAutomaton() { category = COMMENT; }
	~LineCommentAutomaton() {}
	int Read(const std::string& input) {
		newLines = 0;
		if ((input.at(0) == '#') && (input.substr(0, 2) != "#|")) {
			int i = 0;
			for (; (input.at(i) != '\n') && (static_cast<int>(input.size()) > i); ++i) {}
			return i;
		}
		else { return 0; }
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		int i = 0;
		for (; (input.at(i) != '\n') && (static_cast<int>(input.size()) > i); ++i) {}
		return new Token(COMMENT, input.substr(0, i), lineNumber);
	}
};

class StringAutomaton : public Automata {
private:
public:
	StringAutomaton() { category = STRING; }
	~StringAutomaton() {}
	int Read(const std::string& input) {
		newLines = 0;
		if (input.at(0) == '\'') {
			int i = 0;
			while (input.at(i) == '\'') {
				++i;
				for (; (input.at(i) != '\'') && (static_cast<int>(input.size()) > i); ++i) {
					if (input.at(i) == '\n') { newLines++; }
					if ((static_cast<int>(input.size()) == (i + 1)) && (input.at(i) != '\'')) { return 0; }
				}
				++i;
			}
			return i;
		}
		else { return 0; }
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		int i = 0;
		while (input.at(i) == '\'') {
			++i;
			for (; (input.at(i) != '\'') && (static_cast<int>(input.size()) > i); ++i) {}
			++i;
		}
		return new Token(STRING, input.substr(0, i), lineNumber);
	}
};

class IDAutomaton : public Automata {
private:
public:
	IDAutomaton() { category = ID; }
	~IDAutomaton() {}
	int Read(const std::string& input) {
		newLines = 0;
		if (isalpha(input.at(0))) {
			int i = 0;
			for (; isdigit(input.at(i)) || isalpha(input.at(i)); i++) {}
			return i;
		}
		else { return 0; }
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		int i = 0;
		for (; isdigit(input.at(i)) || isalpha(input.at(i)); i++) {}
		return new Token(ID, input.substr(0, i), lineNumber);
	}
};

class UndefinedComment : public Automata {
private:
public:
	UndefinedComment() { category = UNDEFINED; }
	~UndefinedComment() {}
	int Read(const std::string& input) {
		newLines = 0;
		if (input.substr(0, 2) == "#|") {
			int i = 0;
			for (; (input.substr(i, 2) != "|#") && (static_cast<int>(input.size()) > i); ++i) {
				if (input.at(i) == '\n') { newLines++; }
				if ((static_cast<int>(input.size()) == (i + 2)) && (input.substr((i + 1), 2) != "|#")) {
					if (input.back() == '\n') { newLines++; }
					return input.size();
				}
			}
		}
		return 0;
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		return new Token(UNDEFINED, input, lineNumber);
	}
};

class UndefinedString : public Automata {
private:
public:
	UndefinedString() { category = UNDEFINED; }
	~UndefinedString() {}
	int Read(const std::string& input) {
		newLines = 0;
		if (input.at(0) == '\'') {
			int i = 0;
			while (input.at(i) == '\'') {
				++i;
				for (; (input.at(i) != '\'') && (static_cast<int>(input.size()) > i); ++i) {
					if (input.at(i) == '\n') { newLines++; }
					if ((static_cast<int>(input.size()) == (i + 1)) && (input.at(i) != '\'')) { return input.size(); }
				}
				++i;
			}
		}
		return 0;
	}
	Token* CreateToken(const std::string& input, int lineNumber) {
		return new Token(UNDEFINED, input, lineNumber);
	}
};

#endif