#include "Parser.h"

Parser::Parser(std::vector<Token*> &tokenInput) {
    tokenList = tokenInput;
    currentToken = 0;
}

//clears input tokens of comments
std::vector<Token*> Parser::clearComments() {
    for (int i = tokenList.size() - 1; i >= 0; i--){
        if (tokenList.at(i)->getCategory() == COMMENT){
            tokenList.erase(tokenList.begin() + i);
        }
    }
    return tokenList;
}

//checks if input tokens follow grammar structure
bool Parser::isDatalogProgram() {
    if (tokenList.at(currentToken)->getCategory() == SCHEMES) {
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == COLON) {
            currentToken++;
            switch(isScheme()) {
                case 0: break;
                case 1: return false;
                case 2: return false;
            }
            switch(isSchemeList()) {
                case 0: break;
                case 2: return false;
            }
            if (tokenList.at(currentToken)->getCategory() == FACTS) {
                currentToken++;
                if (tokenList.at(currentToken)->getCategory() == COLON) {
                    currentToken++;
                    switch(isFactList()) {
                        case 0: break;
                        case 2: return false;
                    }
                    if (tokenList.at(currentToken)->getCategory() == RULES) {
                        currentToken++;
                        if (tokenList.at(currentToken)->getCategory() == COLON) {
                            currentToken++;
                            switch(isRuleList()) {
                                case 0: break;
                                case 2: return false;
                            }
                            if (tokenList.at(currentToken)->getCategory() == QUERIES) {
                                currentToken++;
                                if (tokenList.at(currentToken)->getCategory() == COLON) {
                                    currentToken++;
                                    switch(isQuery()) {
                                        case 0: break;
                                        case 1: return false;
                                        case 2: return false;
                                    }
                                    switch(isQueryList()) {
                                        case 0: break;
                                        case 2: return false;
                                    }
                                    if (tokenList.at(currentToken)->getCategory() == ENDOF) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//the folowing functions are for determining if syntax is correct
//case 0 is a case where it was successful
//case 1 is a case where it was unsuccessful, but did not increment currentToken
//case 2 is a case where it was unsuccessful and incremented currentToken
unsigned int Parser::isSchemeList() {
    switch(isScheme()) {
        case 0: return isSchemeList();
        case 1: return 0;
        case 2: return 2;
    }
    return 0;
}

unsigned int Parser::isFactList() {
    switch(isFact()) {
        case 0: return isFactList();
        case 1: return 0;
        case 2: return 2;
    }
    return 0;
}

unsigned int Parser::isRuleList() {
    switch(isRule()) {
        case 0: return isRuleList();
        case 1: return 0;
        case 2: return 2;
    }
    return 0;
}

unsigned int Parser::isQueryList() {
    switch(isQuery()) {
        case 0: return isQueryList();
        case 1: return 0;
        case 2: return 2;
    }
    return 0;
}

unsigned int Parser::isScheme() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == ID) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == LEFT_PAREN) {
            currentToken++;
            if (tokenList.at(currentToken)->getCategory() == ID) {
                currentToken++;
                switch(isIdList()) {
                    case 0: break;
                    case 1: break;
                    case 2: return 2;
                }
                if (tokenList.at(currentToken)->getCategory() == RIGHT_PAREN) {
                    currentToken++;
                    return 0;
                }
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isFact() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == ID) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == LEFT_PAREN) {
            currentToken++;
            if (tokenList.at(currentToken)->getCategory() == STRING) {
                currentToken++;
                switch(isStringList()) {
                    case 0: break;
                    case 1: break;
                    case 2: return 2;
                }
                if (tokenList.at(currentToken)->getCategory() == RIGHT_PAREN) {
                    currentToken++;
                    if (tokenList.at(currentToken)->getCategory() == PERIOD) {
                        currentToken++;
                        return 0;
                    }
                }
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isRule() {
    bool usedToken = false;
    switch(isHeadPredicate()) {
        case 0: break;
        case 1: return 1;
        case 2: return 2;
    }
    usedToken = true;
    if (tokenList.at(currentToken)->getCategory() == COLON_DASH) {
        currentToken++;
        switch(isPredicate()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        switch(isPredicateList()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        if (tokenList.at(currentToken)->getCategory() == PERIOD) {
            currentToken++;
            return 0;
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isQuery() {
    switch(isPredicate()) {
        case 0: break;
        case 1: return 1;
        case 2: return 2;
    }
    if (tokenList.at(currentToken)->getCategory() == Q_MARK) {
        currentToken++;
        return 0;
    }
    return 2;
}

unsigned int Parser::isHeadPredicate() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == ID) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == LEFT_PAREN) {
            currentToken++;
            if (tokenList.at(currentToken)->getCategory() == ID) {
                currentToken++;
                switch(isIdList()) {
                    case 0: break;
                    case 1: break;
                    case 2: return 2;
                }
                if (tokenList.at(currentToken)->getCategory() == RIGHT_PAREN) {
                    currentToken++;
                    return 0;
                }
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isPredicate() {
        bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == ID) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == LEFT_PAREN) {
            currentToken++;
            switch(isParameter()) {
                case 0: break;
                case 1: return 2;
                case 2: return 2;
            }
            switch(isParameterList()){
                case 0: break;
                case 1: break;
                case 2: return 2;
            }
            if (tokenList.at(currentToken)->getCategory() == RIGHT_PAREN) {
                currentToken++;
                return 0;
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isPredicateList() {
    if (tokenList.at(currentToken)->getCategory() == COMMA) {
        currentToken++;
        switch(isPredicate()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        switch(isPredicateList()) {
            case 0: return 0;
            case 2: return 2;
        }
    }
    return 0;
}

unsigned int Parser::isParameterList() {
    if (tokenList.at(currentToken)->getCategory() == COMMA) {
        currentToken++;
        switch(isParameter()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        switch(isParameterList()) {
            case 0: return 0;
            case 2: return 2;
        }
    }
    return 0;
}

unsigned int Parser::isStringList() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == COMMA) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == STRING) {
            currentToken++;
            switch(isStringList()) {
                case 0: return 0;
                case 2: return 2;
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 0; }
}

unsigned int Parser::isIdList() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == COMMA) {
        usedToken = true;
        currentToken++;
        if (tokenList.at(currentToken)->getCategory() == ID) {
            currentToken++;
            switch(isIdList()) {
                case 0: return 0;
                case 2: return 2;
            }
        }
    }
    if (usedToken) { return 2; }
    else { return 0; }
}

unsigned int Parser::isParameter() {
    if (tokenList.at(currentToken)->getCategory() == STRING ||
        tokenList.at(currentToken)->getCategory() == ID) {
        currentToken++;
        return 0;
    }
    else switch(isExpression()) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
    }
    return 0;
}

unsigned int Parser::isExpression() {
    bool usedToken = false;
    if (tokenList.at(currentToken)->getCategory() == LEFT_PAREN) {
        usedToken = true;
        currentToken++;
        switch(isParameter()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        switch(isOperator()) {
            case 0: break;
            case 1: return 2;
        }
        switch(isParameter()) {
            case 0: break;
            case 1: return 2;
            case 2: return 2;
        }
        if (tokenList.at(currentToken)->getCategory() == RIGHT_PAREN) {
            currentToken++;
            return 0;
        }
    }
    if (usedToken) { return 2; }
    else { return 1; }
}

unsigned int Parser::isOperator() {
    if (tokenList.at(currentToken)->getCategory() == ADD ||
        tokenList.at(currentToken)->getCategory() == MULTIPLY) {
        currentToken++;
        return 0;
    }
    else return 1;
}


//███████████████████████████████████████████████████████████████████████████████████
//███████████████████████████████████████████████████████████████████████████████████
//Split between parser checkers and parser to create structures
//███████████████████████████████████████████████████████████████████████████████████
//███████████████████████████████████████████████████████████████████████████████████

DatalogProgram Parser::Parse() {
    DatalogProgram newDatalogProgram;
    if (tokenList.at(0)->getCategory() == SCHEMES) {
        tokenList.erase(tokenList.begin());
        if (tokenList.at(0)->getCategory() == COLON) {
            tokenList.erase(tokenList.begin());
            newDatalogProgram.addSchemes(createSchemeList());
            if (tokenList.at(0)->getCategory() == FACTS) {
                tokenList.erase(tokenList.begin());
                if (tokenList.at(0)->getCategory() == COLON) {
                    tokenList.erase(tokenList.begin());
                    newDatalogProgram.addFacts(createFactList());
                    if (tokenList.at(0)->getCategory() == RULES) {
                        tokenList.erase(tokenList.begin());
                        if (tokenList.at(0)->getCategory() == COLON) {
                            tokenList.erase(tokenList.begin());
                            newDatalogProgram.addRules(createRuleList());
                            if (tokenList.at(0)->getCategory() == QUERIES) {
                                tokenList.erase(tokenList.begin());
                                if (tokenList.at(0)->getCategory() == COLON) {
                                    tokenList.erase(tokenList.begin());
                                    newDatalogProgram.addQueries(createQueryList());
                                    if (tokenList.at(0)->getCategory() == ENDOF) {
                                        tokenList.clear();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return newDatalogProgram;
}

std::vector<Predicate*> Parser::createSchemeList() {
    std::vector<Predicate*> newSchemeList;
    if (tokenList.at(0)->getCategory() == ID) {
        newSchemeList.push_back(createScheme());
        std::vector<Predicate*> recursiveList = createSchemeList();
        newSchemeList.insert(newSchemeList.end(), recursiveList.begin(), recursiveList.end());
    }
    return newSchemeList;
}

std::vector<Predicate*> Parser::createFactList() {
    std::vector<Predicate*> newFactList;
    if (tokenList.at(0)->getCategory() == ID) {
        newFactList.push_back(createFact());
        std::vector<Predicate*> recursiveList = createFactList();
        newFactList.insert(newFactList.end(), recursiveList.begin(), recursiveList.end());
    }
    return newFactList;
}

std::vector<Rule*> Parser::createRuleList() {
    std::vector<Rule*> newRuleList;
    if (tokenList.at(0)->getCategory() == ID) {
        newRuleList.push_back(createRule());
        std::vector<Rule*> recursiveList = createRuleList();
        newRuleList.insert(newRuleList.end(), recursiveList.begin(), recursiveList.end());
    }
    return newRuleList;
}

std::vector<Predicate*> Parser::createQueryList() {
    std::vector<Predicate*> newQueryList;
    if (tokenList.at(0)->getCategory() == ID) {
        newQueryList.push_back(createQuery());
        std::vector<Predicate*> recursiveList = createQueryList();
        newQueryList.insert(newQueryList.end(), recursiveList.begin(), recursiveList.end());
    }
    return newQueryList;
}

Predicate* Parser::createScheme() {
    Predicate* newScheme = createPredicate();
    return newScheme;
}

Predicate* Parser::createFact() {
    Predicate* newFact = createPredicate();
    if (tokenList.at(0)->getCategory() == PERIOD) {
        tokenList.erase(tokenList.begin());
    }
    return newFact;
}

Rule* Parser::createRule() {
    Rule* newRule =  new Rule;
    newRule->addHead(createPredicate());
    if (tokenList.at(0)->getCategory() == COLON_DASH) {
        tokenList.erase(tokenList.begin());
        newRule->addPredicate(createPredicateList());
        if (tokenList.at(0)->getCategory() == PERIOD) {
            tokenList.erase(tokenList.begin());
        }
    }
    return newRule;
}

Predicate* Parser::createQuery() {
    Predicate* newQuery = createPredicate();
    if (tokenList.at(0)->getCategory() == Q_MARK) {
        tokenList.erase(tokenList.begin());
    }
    return newQuery;
}

Predicate* Parser::createPredicate() {
    Predicate* thePredicate = new Predicate;
    if (tokenList.at(0)->getCategory() == ID) {
        thePredicate->addFirstToken(tokenList.at(0));
        tokenList.erase(tokenList.begin());
        if (tokenList.at(0)->getCategory() == LEFT_PAREN) {
            tokenList.erase(tokenList.begin());
            thePredicate->addList(createParameterList());
            if (tokenList.at(0)->getCategory() == RIGHT_PAREN) {
                tokenList.erase(tokenList.begin());
            }
        }
    }
    return thePredicate;
}

std::vector<Predicate*> Parser::createPredicateList() {
    std::vector<Predicate*> predList;
    Predicate* imSickOfThis = createPredicate();
    predList.push_back(imSickOfThis);
    if (tokenList.at(0)->getCategory() == COMMA) {
        tokenList.erase(tokenList.begin());
        std::vector<Predicate*> recursiveList;
        recursiveList = createPredicateList();
        predList.insert(predList.end(), recursiveList.begin(), recursiveList.end());
    }
    return predList;
}

std::vector<Parameter*> Parser::createParameterList() {
    std::vector<Parameter*> ParameterList;
    if (tokenList.at(0)->getCategory() == RIGHT_PAREN) {
        return ParameterList;
    }
    if (tokenList.at(0)->getCategory() == COMMA) {
        tokenList.erase(tokenList.begin());
    }
        ParameterList.push_back(createParameter());
        std::vector<Parameter*> recursiveList = createParameterList();
        ParameterList.insert(ParameterList.end(), recursiveList.begin(), recursiveList.end());
    return ParameterList;
}

Expression* Parser::createExpression() {
    Expression* expressive = new Expression;
    tokenList.erase(tokenList.begin());
    expressive->param1 = createParameter();
    expressive->op = tokenList.at(0);
    tokenList.erase(tokenList.begin());
    expressive->param2 = createParameter();
    tokenList.erase(tokenList.begin());
    return expressive;
}

Parameter* Parser::createParameter() {
    Parameter* param;
    if (tokenList.at(0)->getCategory() == STRING ||
        tokenList.at(0)->getCategory() == ID) {
        Token* tempToken = tokenList.at(0);
        tokenList.erase(tokenList.begin());
        param = new Parameter(tempToken);
    }
    else {
        Expression* express = createExpression();
        param = new Parameter(express->param1, express->op, express->param2);
    }
    return param;
}