#include "database.h"



database::database()
{
}


database::~database()
{
}

//*Used to insert a single relation into the Database object.
//@relationName(string): name of relation
//@parameterList(list<parameter> list of parameter objects, which will be stripped down to just the value, since they should all be ID's.
void database::addRelation(string relationName, list<parameter> parameterList)
{
	relationMap.insert(pair<string,relation>(relationName, relation(relationName, parameterList)));
}

void database::addTuples(string relationName, list<parameter> tupleValues)
{
	Tuple tempTuple;
	for (parameter x : tupleValues)
	{
		tempTuple.push_back(x.getParameterValue());
	}

	relationMap[relationName].addTuple(tempTuple);
}

void database::Union(string relationName, set<Tuple> tuplesToAdd)
{
	for (Tuple x : tuplesToAdd)
	{
		relationMap[relationName].addTuple(x);
	}
}

size_t database::size()
{
	size_t sizeOfDatabase = 0;

	for (auto x : relationMap)
	{
		sizeOfDatabase += (x.second.getTuples()).size();
	}

	return sizeOfDatabase;
}
