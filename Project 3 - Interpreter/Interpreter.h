#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "Parser.h"
#include "Database.h"

class Interpreter {
private:
public:
    DatalogProgram* datalog = NULL;
    Database* database = new Database;
    std::vector<Relation*> finalRelations;
    Interpreter() {}
    Interpreter(DatalogProgram* newDatalog) {
        datalog = newDatalog;
    }
    ~Interpreter() {}
    void interpret();
    Relation* evaluatePredicate(const Predicate p);
    Relation* evaluateSchemes();
    void evaluateFacts();
    std::string toaster();
};

#endif