#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Grammar.h"
#include <vector>

struct Expression {
    Parameter* param1 = NULL;
    Token* op = NULL;
    Parameter* param2 = NULL;
};

class Parser {
private:
    //DatalogProgam theLog;
    std::vector<Token*> tokenList;
    unsigned int isSchemeList();
    unsigned int isFactList();
    unsigned int isRuleList();
    unsigned int isQueryList();
    unsigned int isScheme();
    unsigned int isFact();
    unsigned int isRule();
    unsigned int isQuery();
    unsigned int isHeadPredicate();
    unsigned int isPredicate();
    unsigned int isPredicateList();
    unsigned int isParameterList();
    unsigned int isStringList();
    unsigned int isIdList();
    unsigned int isParameter();
    unsigned int isExpression();
    unsigned int isOperator();
    
    std::vector<Predicate*> createSchemeList();
    std::vector<Predicate*> createFactList();
    std::vector<Rule*> createRuleList();
    std::vector<Predicate*> createQueryList();
    Predicate* createScheme();
    Predicate* createFact();
    Rule* createRule();
    Predicate* createQuery();
    Predicate* createHeadPredicate();
    Predicate* createPredicate();
    std::vector<Predicate*> createPredicateList();
    std::vector<Parameter*> createParameterList();
    Expression* createExpression();
    Parameter* createParameter();
public:
    Parser(std::vector<Token*> &tokenInput);
    ~Parser() {};
    unsigned int currentToken;
    std::vector<Token*> clearComments();
    bool isDatalogProgram();
    DatalogProgram Parse();
};

#endif