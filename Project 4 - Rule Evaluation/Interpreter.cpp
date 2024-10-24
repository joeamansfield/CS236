#include "Interpreter.h"

void Interpreter::interpret() {
    while (datalog->schemes.size() > 0) {
        Relation* newRelation = evaluateSchemes();
        database->nameToRelation.insert(std::pair<std::string, Relation*> (newRelation->relationName, newRelation));
    }
    while (datalog->facts.size() > 0) {
        evaluateFacts();
    }
    bool insertedNewTuple = true;
    unsigned int numberOfLoops = 0;
    while (insertedNewTuple) {
        insertedNewTuple = evaluateRules();
        numberOfLoops++;
    }
    rulesString += "\nSchemes populated after " + std::to_string(numberOfLoops) + " passes through the Rules.\n\n";
    for (unsigned int i = 0; i < datalog->queries.size(); i++) {
        finalRelations.push_back(evaluatePredicate(*datalog->queries.at(i)));
    }
}

Relation* Interpreter::evaluateSchemes() {
    Relation* newRelation = new Relation;
    newRelation->relationName = datalog->schemes.at(0)->firstToken->getTokenContent();
    for (unsigned int i = 0; i < datalog->schemes.at(0)->paramList.size(); i++) {
        newRelation->thisIsTheHeader.attributes.push_back(datalog->schemes.at(0)->paramList.at(i)->multipurpose->getTokenContent());
    }
    datalog->schemes.erase(datalog->schemes.begin());
    return newRelation;
}

void Interpreter::evaluateFacts() {
    std::string relationName = datalog->facts.at(0)->firstToken->getTokenContent();
    Relation* modRelation = database->nameToRelation.at(relationName);
    std::vector<std::string> tupleList;
    for (unsigned int i = 0; i < datalog->facts.at(0)->paramList.size(); i++) {
        tupleList.push_back(datalog->facts.at(0)->paramList.at(i)->multipurpose->getTokenContent());
    }
    Tuple newple(tupleList);
    modRelation->tupleSet.insert(newple);
    datalog->facts.erase(datalog->facts.begin());
}

bool Interpreter::evaluateRules() {
    bool addedNewTuple = false;
    for (unsigned int i = 0; i < datalog->rules.size(); i++) {
        Relation* mainRelation = evaluatePredicate(*datalog->rules.at(i)->predList.at(0));
        for (unsigned int j = 1; j < datalog->rules.at(i)->predList.size(); j++) {
            mainRelation = mainRelation->join(evaluatePredicate(*datalog->rules.at(i)->predList.at(j)));
        }
        std::vector<unsigned int> projectColumns;
        for (unsigned j = 0; j < datalog->rules.at(i)->headPredicate->paramList.size(); j++) {
            for (unsigned int n = 0; n < mainRelation->thisIsTheHeader.attributes.size(); n++) {
                if (datalog->rules.at(i)->headPredicate->paramList.at(j)->multipurpose->getTokenContent() == mainRelation->thisIsTheHeader.attributes.at(n)) {
                    projectColumns.push_back(n);
                }
            }
        }
        mainRelation = mainRelation->project(projectColumns);
        mainRelation->relationName = datalog->rules.at(i)->headPredicate->firstToken->getTokenContent();
        std::vector<bool> tooBool4School = database->nameToRelation.at(mainRelation->relationName)->onion(mainRelation);
        std::set<Tuple>::iterator it = mainRelation->tupleSet.begin();
        rulesString += datalog->rules.at(i)->toStr() + "\n";
        for (unsigned int j = 0; j < tooBool4School.size(); j++) {
            if (tooBool4School.at(j)) {
                addedNewTuple = true;
                rulesString += "  ";
                for (unsigned int n = 0; n < it->tupolev.size(); n++) {
                    rulesString += database->nameToRelation.at(mainRelation->relationName)->thisIsTheHeader.attributes.at(n) + "=" + it->tupolev.at(n) + ", ";
                }
                rulesString.pop_back();
                rulesString.pop_back();
                rulesString += "\n";
            }
            it++;
        }
    }
    return addedNewTuple;
}

Relation* Interpreter::evaluatePredicate(const Predicate p) {
    std::vector<std::string> params;
    std::vector<unsigned int> firstInstances;
    Relation* mem;
    std::string name = p.firstToken->getTokenContent();
    Relation* newRelation = new Relation;
    for (unsigned int i = 0; i < p.paramList.size(); i++) {
        std::string newString = p.paramList.at(i)->multipurpose->getTokenContent();
        if (std::find(params.begin(), params.end(), newString) == params.end()
            && newString.at(0) != '\'') {
            firstInstances.push_back(i);
        }
        params.push_back(newString);
    }
    *newRelation = *database->nameToRelation.at(name);
    for (unsigned int i = 0; i < params.size(); i++) {
        if (params.at(i).at(0) == '\'') {
            mem = newRelation;
            newRelation = newRelation->select(i, params.at(i));
            delete mem;
        }
        else {
            mem = newRelation;
            newRelation = newRelation->select(std::find(params.begin(), params.end(), params.at(i)) - params.begin(), i);
            delete mem;
            /*if (firstInstances.end() == std::find(firstInstances.begin(), firstInstances.end(), i)) {
                for (unsigned int j = 0; j < i; j++) {
                    newRelation = newRelation->select(i,j);
                    break;
                }
            }*/
        }
    }
    mem = newRelation;
    newRelation = newRelation->project(firstInstances);
    delete mem;
    std::vector<std::string> headerValues;
    for (unsigned int i = 0; i < firstInstances.size(); i++) {
        headerValues.push_back(params.at(firstInstances.at(i)));
    }
    mem = newRelation;
    newRelation = newRelation->rename(headerValues);
    delete mem;
    return newRelation;
}

std::string Interpreter::toaster() {
    std::string output;
    output.append("Rule Evaluation\n");
    output.append(rulesString);
    output.append("Query Evaluation\n");
    for (unsigned int i = 0; i < datalog->queries.size(); i++) {
        output.append(datalog->queries.at(i)->toStr());
        output.append("?");
        if (finalRelations.at(i)->tupleSet.size() > 0) {
            output = output + " Yes(" + std::to_string(finalRelations.at(i)->tupleSet.size()) + ")";
            if (finalRelations.at(i)->thisIsTheHeader.attributes.size() > 0) {
                output.append("\n");
            }
            for (std::set<Tuple>::iterator it = finalRelations.at(i)->tupleSet.begin(); it != finalRelations.at(i)->tupleSet.end(); it++) {
                output.append("  ");
                for (unsigned int j = 0; j < it->tupolev.size(); j++) {
                    output = output + finalRelations.at(i)->thisIsTheHeader.attributes.at(j) + "=" + it->tupolev.at(j) + ", ";
                }
                output.pop_back();
                output.pop_back();
                output.append("\n");
            }
        }
        else {
            output.append(" No\n");
        }
    }
    return output;
}
