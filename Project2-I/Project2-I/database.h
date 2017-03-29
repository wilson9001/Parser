#ifndef DATABASE_H_
#define DATABASE_H_

#include <map>
#include "relation.h"
#include <list>
#include "parameter.h"

using namespace std;

class database
{
public:
	database();
	~database();

	void addRelation(string relationName, list<parameter> parameterList);

	void addTuples(string relationName, list<parameter> tupleValues);

	void Union(string relationName, set<Tuple> tuplesToAdd);

	relation getRelation(string relationName) { return relationMap[relationName]; }

	size_t size();

private:

	//This map will hold all the relation objects with relation name, relation object.
	map<string, relation> relationMap;
};

#endif