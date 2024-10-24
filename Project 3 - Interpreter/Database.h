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
    Header() {}
    Header(std::vector<std::string> headTuper) {
        for (unsigned int i = 0; i < headTuper.size(); i++) {
            attributes.push_back(headTuper.at(i));
        }
    }
    ~Header() {}
    std::vector<std::string> attributes;
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
        std::sort(columns.begin(), columns.end());
        std::set<Tuple> newpolev;
        std::vector<std::string> headerValues;
        unsigned int j = 0;
        for (unsigned int i = 0; j < columns.size(); i++) {
            if (columns.at(j) == i) {
                headerValues.push_back(thisIsTheHeader.attributes.at(i));
                j++;
            }
        }
        Header newHeader(headerValues);
        std::set<Tuple>::iterator it = tupleSet.begin();
        while (it != tupleSet.end()) {
            std::vector<std::string> newpolevValues;
            j = 0;
            for (unsigned int i = 0; j < columns.size(); i++) {
                if (columns.at(j) == i) {
                    newpolevValues.push_back(it->tupolev.at(i));
                    j++;
                }
            }
            newpolev.insert(Tuple(newpolevValues));
            it++;
        }
        Relation* returnThis = new Relation(relationName, thisIsTheHeader, newpolev);
        return returnThis;
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