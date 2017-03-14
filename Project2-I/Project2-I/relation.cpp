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
