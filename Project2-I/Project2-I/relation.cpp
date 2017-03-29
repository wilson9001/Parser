#include "relation.h"
#include <map>

relation::relation(string relationName, list<parameter> parameterList)
{
	this->relationName = relationName;

	vector<string> schemeValueList;

	for (parameter p : parameterList)
	{
		relationScheme.push_back(p.getParameterValue());
	}
}

relation::relation()
{
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

	for (size_t i = 0; i < projectNames.size(); i++)
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

Tuple relation::removeTupleColumns(Tuple &oldTuple)
{
	Tuple newTuple;

	for (int x : columnsToProject)
	{
		newTuple.push_back(oldTuple[x]);
	}

	//Need to remove scheme columns here...
	//scheme newScheme
	return newTuple;
}

void relation::rename()
{
	/*
	//create scheme outside of rename function, set equal here.
	for (size_t i = 0; i < relationScheme.size(); i++)
	{
		relationScheme[i] = projectNames[i];
	}
	*/
	scheme newScheme;

	for (auto x : projectNames)
	{
		newScheme.push_back(x);
	}

	relationScheme = newScheme;
}

void relation::join(relation & relationToJoin)
{
	/*
	Create new schema vector, iterate through schema vectors of relations 1 and then 2. If an ID is already in the new schema, don't add it again. Add a pair of ints corresponding to the index of the ID in the first scheme (which will match the one in the new scheme) and the index of the same ID in the second scheme, into a vector of pairs. If the two relations have no shared ID's, the new vector will hold all of the ID's for the schemes of both 1 and 2; the vector of pairs will be empty. If the schemes have all of the same ID's then the scheme will hold the ID's in the order of the first scheme, and the vector of pairs will be the same size as the scheme, with each relation in a pair.
	
	The joinable function will determine if different tuples are joinable. I will need to pass in both the tuples and the scheme of the new relation, and then check to make sure each indexed value matches in both tuples. If this is the case, then it will return true. Otherwise, it will return false.

	Create a new relation with the finished scheme. THIS IS THE NEW RELATION. MODIFY INTERNAL DATA
	*/

	//pair<scheme, vector<pair<int,int>>> newSchemeInfo =
	vector<pair<int,int>> sharedColumns = makeScheme(relationToJoin.getScheme());

	//relation newRelation(newSchemeInfo.first);

	//rel1.getTuples()
	for (Tuple x : relationTuples)
	{
		for (Tuple y : relationToJoin.getTuples())
		{
			//places that now say charedColumns were newSchemeInfo.second
			if (joinable(x, y,sharedColumns))
			{
				//will probably need to modify the addTuple function so it is called from the interpreter object, and this will reorder the columns and then union. NO
				//addTuple(joinTuple(x, y, sharedColumns));

				tempTuples.insert(joinTuple(x, y, sharedColumns));
			}
		}
	}

	relationTuples = tempTuples;

	tempTuples.clear();
	//WORK HERE
}


Tuple relation::joinTuple(Tuple & t1, Tuple & t2, vector<pair<int, int>> &matchingIndex)
{
	map<int, char> columnsToExclude;
	
	for (pair<int, int> x : matchingIndex)
	{
		columnsToExclude.insert(pair<int, char>(x.second, 'Y'));
	}

	Tuple newTuple = t1;

	for (size_t i = 0; i < t2.size(); i++)
	{
		if (!columnsToExclude.count(i))
		{
			newTuple.push_back(t2[i]);
		}
	}

	//WORK HERE - CAN'T INSERT YET. HAVE TO REORDER and remove COLUMNS
	//relationTuples.insert(newTuple);
	return newTuple;
}

bool relation::joinable(Tuple & tup1, Tuple & tup2, vector<pair<int, int>> &matchingIndex)
{
	/*size_t i = 0; i < matchingIndex.size(); i++*/
	for (pair<int, int> x : matchingIndex)
	{
		if (tup1[x.first] != tup2[x.second])
		{
			return false;
		}
	}

	return true;
}


void relation::addJoin(relation & newRelation, Tuple & tupleToAdd)
{
	newRelation.addTuple(tupleToAdd);
}


void relation::Union(relation & rel1, relation & rel2)
{
	if (rel1.getScheme().size() != rel2.getScheme().size())
	{
		cout << "Error! The schemes aren't the same size for this union!\n";
	}

	for (Tuple x : rel2.getTuples())
	{
		rel1.addTuple(x);
	}
}

//pair<scheme,
vector<pair<int,int>> relation::makeScheme(scheme &scheme2)
{
	//scheme newScheme = scheme1;
	//use relationScheme
	vector<pair<int, int>> sharedColumnIndexes;
	bool uniqueColumn;

	for (size_t i = 0; i < scheme2.size(); i++)
	{
		uniqueColumn = true;
		for (size_t j = 0; i < relationScheme.size(); i++)
		{
			if (scheme2[i] == relationScheme[j])
			{
				sharedColumnIndexes.push_back(pair<int, int>(j, i));
				uniqueColumn = false;
			}
		}

		if (uniqueColumn)
		{
			//newScheme
			relationScheme.push_back(scheme2[i]);
		}
	}

	return sharedColumnIndexes;
}

void relation::reScheme(list<parameter> &scheme)
{
	relationScheme.clear();

	for (parameter x : scheme)
	{
		relationScheme.push_back(x.getParameterValue());
	}
}

void relation::fixColumns(predicate & headPredicate)
{
	vector<size_t> relevantColumnPosition;

	for (parameter x : headPredicate.getPredicateParameters())
	{
		for (size_t i = 0; i < relationScheme.size(); i++)
		{
			if (x.getParameterValue() == relationScheme[i])
			{
				relevantColumnPosition.push_back(i);
			}
		}
	}

	reorderColumns(relevantColumnPosition);
}

void relation::reorderColumns(vector<size_t> &relevantColumnPosition)
{
	Tuple tempTuple;

	for (Tuple x : relationTuples)
	{
		for (int y : relevantColumnPosition)
		{
			//tempScheme.push_back(relationScheme[y]);
			tempTuple.push_back(x[y]);
		}
		tempTuples.insert(tempTuple);
		tempTuple.clear();
	}

	relationTuples = tempTuples;

	tempTuples.clear();

	for (int x : relevantColumnPosition)
	{
		tempScheme.push_back(relationScheme[x]);
	}

	relationScheme = tempScheme;

	tempScheme.clear();
}
