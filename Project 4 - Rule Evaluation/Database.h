#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>

class Tuple {
private:
public:
    std::vector<std::string> tupolev;
    Tuple() {}
    Tuple(std::vector<std::string> tuper) {
        for (unsigned int i = 0; i < tuper.size(); i++) {
            tupolev.push_back(tuper.at(i));
        }
    }
    ~Tuple() {}
    bool operator< (const Tuple & other) const {
        for (unsigned int i = 0; i < tupolev.size(); i++) {
            if (tupolev.at(i) < other.tupolev.at(i)) return true;
            else if (other.tupolev.at(i) < tupolev.at(i)) return false;
        }
        return false; //happens if all values were equal
    }
};

class Header {
private:
public:
    std::vector<std::string> attributes;
    Header() {}
    Header(std::vector<std::string> headTuper) {
        attributes = headTuper;
    }
    ~Header() {}
};

class Relation {
private:
public:
    std::string relationName;
    Header thisIsTheHeader;
    std::set<Tuple> tupleSet;
    Relation() {};
    Relation(std::string name, Header newHeader, std::set<Tuple> newTupleSet) {
        relationName = name;
        thisIsTheHeader = newHeader;
        tupleSet = newTupleSet;
    }
    ~Relation() {}

    Relation* select(unsigned int column, std::string value) {
        std::set<Tuple> newpolev;
        std::set<Tuple>::iterator it = tupleSet.begin();
        while (it != tupleSet.end()) {
            if (it->tupolev.at(column) == value) {
                newpolev.insert(*it);
            }
            it++;
        }
        Relation* returnThis = new Relation(relationName, thisIsTheHeader, newpolev);
        return returnThis;
    }

    Relation* select(unsigned int column1, unsigned int column2) {
        std::set<Tuple> newpolev;
        std::set<Tuple>::iterator it = tupleSet.begin();
        while (it != tupleSet.end()) {
            if (it->tupolev.at(column1) == it->tupolev.at(column2)) {
                newpolev.insert(*it);
            }
            it++;
        }
        Relation* returnThis = new Relation(relationName, thisIsTheHeader, newpolev);
        return returnThis;
    }

    Relation* project(std::vector<unsigned int> columns) {
        std::set<Tuple> newpolev;
        std::vector<std::string> headerValues;
        for (unsigned  int i = 0; i < columns.size(); i++) {
            headerValues.push_back(thisIsTheHeader.attributes.at(columns.at(i)));
        }
        Header newHeader(headerValues);
        std::set<Tuple>::iterator it = tupleSet.begin();
        while (it != tupleSet.end()) {
            std::vector<std::string> newpolevValues;
            for (unsigned int i = 0; i < columns.size(); i++) {
                newpolevValues.push_back(it->tupolev.at(columns.at(i)));
            }
            newpolev.insert(Tuple(newpolevValues));
            it++;
        }
        Relation* returnThis = new Relation(relationName, thisIsTheHeader, newpolev);
        return returnThis;
    }

    Relation* join(Relation* joinRelation) {
        std::vector<std::string> newAttributes = thisIsTheHeader.attributes;
        newAttributes.insert(newAttributes.end(), joinRelation->thisIsTheHeader.attributes.begin(), joinRelation->thisIsTheHeader.attributes.end());
        std::vector<unsigned int> headerDupesInitial;
        std::vector<unsigned int> headerDupesFinal;
        for (unsigned int i = 0; i < newAttributes.size(); i++) {
            for (unsigned int j = 0; j < i; j++) {
                if (newAttributes.at(j) == newAttributes.at(i)) {
                    headerDupesInitial.push_back(j);
                    headerDupesFinal.push_back(i);
                    break;
                }
            }
        }
        for (int i = static_cast<int>(headerDupesFinal.size() - 1); i >= 0; i--) {
            newAttributes.erase(newAttributes.begin() + headerDupesFinal.at(i));
        }
        Header newHeader(newAttributes);
        std::set<Tuple> newTupleSet;
        //no overlap between headers
        if (newAttributes.size() == thisIsTheHeader.attributes.size() + joinRelation->thisIsTheHeader.attributes.size()) {
            for (std::set<Tuple>::iterator it = tupleSet.begin(); it != tupleSet.end(); it++) {
                for (std::set<Tuple>::iterator jt = joinRelation->tupleSet.begin(); jt != joinRelation->tupleSet.end(); jt++) {
                    std::vector<std::string> newTupleVals;
                    newTupleVals.insert(newTupleVals.end(), it->tupolev.begin(), it->tupolev.end());
                    newTupleVals.insert(newTupleVals.end(), jt->tupolev.begin(), jt->tupolev.end());
                    Tuple newTuple(newTupleVals);
                    newTupleSet.insert(newTuple);
                }
            }
        }
        //headers overlap
        else {
            for (std::set<Tuple>::iterator it = tupleSet.begin(); it != tupleSet.end(); it++) {
                for (std::set<Tuple>::iterator jt = joinRelation->tupleSet.begin(); jt != joinRelation->tupleSet.end(); jt++) {
                    bool allDupesMatch = true;
                    for (unsigned int i = 0; i < headerDupesInitial.size(); i++) {
                        if (it->tupolev.at(headerDupesInitial.at(i)) != jt->tupolev.at(headerDupesFinal.at(i) - thisIsTheHeader.attributes.size())) {
                            allDupesMatch = false;
                            break;
                        }
                    }
                    if (allDupesMatch) {
                        std::vector<std::string> newTupleVals;
                        newTupleVals.insert(newTupleVals.end(), it->tupolev.begin(), it->tupolev.end());
                        newTupleVals.insert(newTupleVals.end(), jt->tupolev.begin(), jt->tupolev.end());
                        for (int i = static_cast<int>(headerDupesFinal.size()) - 1; i >= 0; i--) {
                            newTupleVals.erase(newTupleVals.begin() + headerDupesFinal.at(i));
                        }
                        Tuple newTuple(newTupleVals);
                        newTupleSet.insert(newTuple);
                    }
                }
            }
        }
        Relation* newRelation = new Relation("intermediary", newHeader, newTupleSet);
        return newRelation;
    }

    std::vector<bool> onion(Relation* onionRelation) {
        std::vector<bool> danielKron;
        if (relationName != onionRelation->relationName) return danielKron;
        for (std::set<Tuple>::iterator it = onionRelation->tupleSet.begin(); it != onionRelation->tupleSet.end(); it++) {
            if (tupleSet.insert(*it).second == true) {
                danielKron.push_back(true);
            }
            else danielKron.push_back(false);
        }
        return danielKron;
    }

    Relation* rename(std::vector<std::string> newHeaderValues) {
        Header newHeader(newHeaderValues);
        Relation* returnThis = new Relation(relationName, newHeader, tupleSet);
        return returnThis;
    }
};

class Database {
private:
public:
    std::map<std::string, Relation*> nameToRelation;
    Database() {}
    Database(std::vector<std::string> names, std::vector<Relation*> relations) {
        for (unsigned int i = 0; i < names.size(); i++) {
            nameToRelation[names.at(i)] = relations.at(i);
        }
    }
    ~Database() {}
};

#endif