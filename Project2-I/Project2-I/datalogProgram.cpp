#include "datalogProgram.h"

datalogProgram::datalogProgram()
{
}

datalogProgram::~datalogProgram()
{
}

void datalogProgram::addFacts(list<predicate> factList)
{
	for (auto x : factList)
	{
		this->factList.push_back(x);

		list<parameter> predicateParameters = x.getPredicateParameters();

		for (auto y : predicateParameters)
		{
			domain.insert(y.getParameterValue());
		}
	}
}

void datalogProgram::addSchemes(list<predicate> schemeList)
{
	for (auto x : schemeList)
	{
		this->schemeList.push_back(x);
	}
}

void datalogProgram::addQueries(list<predicate> queryList)
{
	for (auto x : queryList)
	{
		this->queryList.push_back(x);
	}
}

void datalogProgram::addRules(list<rule> addRules)
{
	for (auto x : addRules)
	{
		this->ruleList.push_back(x);
	}
}
