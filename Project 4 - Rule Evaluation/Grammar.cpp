#include <algorithm>
#include "Grammar.h"

//all functions for the DatalogProgram class
void DatalogProgram::addSchemes(std::vector<Predicate*> newSchemes) {
    schemes = newSchemes;
}
void DatalogProgram::addFacts(std::vector<Predicate*> newFacts) {
    facts = newFacts;
}
void DatalogProgram::addRules(std::vector<Rule*> newRules) {
    rules = newRules;
}
void DatalogProgram::addQueries(std::vector<Predicate*> newQueries) {
    queries = newQueries;
}
std::string DatalogProgram::toStr() {
    std::string outputString = "";
    outputString.append("Schemes(");
    outputString.append(std::to_string(schemes.size()));
    outputString.append("):\n");
    for (unsigned int i = 0; i < schemes.size(); i++) {
        outputString.append("  ");
        outputString.append(schemes.at(i)->toStr());
        outputString.append("\n");
    }
    outputString.append("Facts(");
    outputString.append(std::to_string(facts.size()));
    outputString.append("):\n");
    for (unsigned int i = 0; i < facts.size(); i++) {
        outputString.append("  ");
        outputString.append(facts.at(i)->toStr());
        outputString.append(".\n");
    }
    outputString.append("Rules(");
    outputString.append(std::to_string(rules.size()));
    outputString.append("):\n");
    for (unsigned int i = 0; i < rules.size(); i++) {
        outputString.append("  ");
        outputString.append(rules.at(i)->toStr());
        outputString.append("\n");
    }
    outputString.append("Queries(");
    outputString.append(std::to_string(queries.size()));
    outputString.append("):\n");
    for (unsigned int i = 0; i < queries.size(); i++) {
        outputString.append("  ");
        outputString.append(queries.at(i)->toStr());
        outputString.append("?\n");
    }
    std::vector<std::string> x;
    for (unsigned int i = 0; i < facts.size(); i++) {
        for (unsigned int j = 0; j < facts.at(i)->paramList.size(); j++) {
            x.push_back(facts.at(i)->paramList.at(j)->multipurpose->getTokenContent());
        }
    }
    std::sort(x.begin(), x.end());
    if (x.size() > 0) {
        for (unsigned int i = 0; i < x.size() - 1; i++) {
            if (x.at(i) == x.at(i + 1)) {
                x.erase(x.begin() + i);
                i--;
            }
        }
    }
    outputString.append("Domain(");
    outputString.append(std::to_string(x.size()));
    outputString.append("):\n");
    for (unsigned int i = 0; i < x.size(); i++) {
        outputString.append("  ");
        outputString.append(x.at(i));
        outputString.append("\n");
    }
    return outputString;
}

//all functions for the Rule class
void Rule::addHead(Predicate* newHead) {
    headPredicate = newHead;
}
void Rule::addPredicate(std::vector<Predicate*> newPredList) {
    predList = newPredList;
}
std::string Rule::toStr() {
    std::string outputString = "";
    outputString.append(headPredicate->toStr());
    outputString.append(" :- ");
    for (unsigned int i = 0; i < predList.size(); i++){
        outputString.append(predList.at(i)->toStr());
        outputString.append(",");
    }
    outputString.pop_back();
    outputString.append(".");
    return outputString;
}

//all functions for the Predicate class
void Predicate::addFirstToken(Token* newToken) {
    firstToken = newToken;
}
void Predicate::addList(std::vector<Parameter*> newParamList) {
    paramList = newParamList;
}
std::string Predicate::toStr() {
    std::string outputString = "";
    outputString.append(firstToken->getTokenContent());
    outputString.append("(");
    for (unsigned int i = 0; i < paramList.size(); i++) {
        outputString.append(paramList.at(i)->toStr());
        outputString.append(",");
    }
    outputString.pop_back();
    outputString.append(")");
    return outputString;
}

//all functions for the Parameter class
Parameter::Parameter(Token* singleToken) {
    multipurpose = singleToken;
}
Parameter::Parameter(Parameter* param1, Token* op, Parameter* param2) {
    this->param1 = param1;
    this->multipurpose = op;
    this->param2 = param2;
}
std::string Parameter::toStr() {
    std::string outputString = "";
    if (multipurpose->getCategory() == STRING || multipurpose->getCategory() == ID) {
        outputString.append(multipurpose->getTokenContent());
    }
    else {
        outputString.append("(");
        outputString.append(param1->toStr());
        outputString.append(multipurpose->getTokenContent());
        outputString.append(param2->toStr());
        outputString.append(")");
    }
    return outputString;
}