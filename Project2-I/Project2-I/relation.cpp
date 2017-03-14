#include "relation.h"

relation::relation(string relationName, list<parameter> parameterList)
{
	this->relationName = relationName;

	vector<string> schemeValueList;

	for (parameter p : parameterList)
	{
		relationScheme.push_back(p.getParameterValue());
	}
}

relation::~relation()
{
}

//relation relation::selectAbsolute(string AbsoluteString, int positionInTuple)
void relation::selectAbsolute(string AbsoluteString, int positionInTuple)
{
	set<Tuple> newSet;

	for (Tuple x : relationTuples)
	{
		if (x[positionInTuple] == AbsoluteString)
		{
			newSet.insert(x);
		}
	}

	relationTuples = newSet;
}

//relation relation::selectID(string IdString, int positionInTuple)
void relation::selectID(string IdString, int positionInTuple)
{
	bool newID = true;

	for (int i = 0; i < projectNames.size(); i++)
	{
		if (projectNames[i] == IdString)
		{
			findSharedColumns(i, positionInTuple);
			newID = false;
		}
	}

	if (newID)
	{
		projectNames.push_back(IdString);
		columnsToProject.push_back(positionInTuple);
	}
}

void relation::findSharedColumns(int position1, int position2)
{
	set<Tuple> newSet;

	for (Tuple x : relationTuples)
	{
		if (x[position1] == x[position2])
		{
			newSet.insert(x);
		}
	}

	relationTuples = newSet;
}

//relation relation::project()
void relation::project()
{
	set<Tuple> newSet;

	//iterate through each element in set, then iterate through the vector of ints and pull out the indexes from the vector of values(tuple) that correspond to the values in the int vector. Insert into new tuple, insert tuple into new set.

	for (Tuple x : relationTuples)
	{
		newSet.insert(removeTupleColumns(x));
	}

	relationTuples = newSet;
}

Tuple relation::removeTupleColumns(Tuple oldTuple)
{
	Tuple newTuple;

	for (int x : columnsToProject)
	{
		newTuple.push_back(oldTuple[x]);
	}

	return newTuple;
}

void relation::rename()
{
	for (size_t i = 0; i < relationScheme.size(); i++)
	{
		relationScheme[i] = projectNames[i];
	}
}
