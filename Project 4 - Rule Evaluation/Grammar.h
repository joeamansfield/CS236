#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "Token.h"
#include <string>
#include <vector>

class Parameter {
private:
    Parameter* param1 = NULL;
    Parameter* param2 = NULL;
public:
    Token* multipurpose = NULL;
    Parameter(Token* singleToken);
    Parameter(Parameter* param1, Token* op, Parameter* param2);
    ~Parameter();
    std::string toStr();
};

class Predicate {
private:
    //Scheme and Fact follow the form but store tokens instead of parameters like Query does
public:
    Token* firstToken = NULL;
    std::vector<Parameter*> paramList;
    Predicate() {}
    ~Predicate() {}
    void addFirstToken(Token* newToken); 
    void addList(std::vector<Parameter*> newParamList);
    std::string toStr();
};

class Rule {
private:
public:
    //first ID is before an open paren rest are separated by commas before the close paren
    Predicate* headPredicate = NULL;
    std::vector<Predicate*> predList;
    Rule() {}
    ~Rule() {}
    void addHead(Predicate* newHead);
    void addPredicate(std::vector<Predicate*> newPredList);
    std::string toStr();
};

class DatalogProgram {
private:
public:
    std::vector<Predicate*> schemes;
    std::vector<Predicate*> facts;
    std::vector<Rule*> rules;
    std::vector<Predicate*> queries;
    DatalogProgram() {}
    ~DatalogProgram() {}
    void addSchemes(std::vector<Predicate*> newSchemes);
    void addFacts(std::vector<Predicate*> newFacts);
    void addRules(std::vector<Rule*> newRules);
    void addQueries(std::vector<Predicate*> newQueries);
    std::string toStr();
};

#endif