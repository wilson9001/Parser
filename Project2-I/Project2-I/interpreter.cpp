#include "interpreter.h"
#include <iostream>

using namespace std;

interpreter::interpreter()
{
}

interpreter::~interpreter()
{
}

void interpreter::createDataBase(datalogProgram & dataLog)
{
	const list<predicate> schemeList = dataLog.getSchemes();
	//create relations
	for (predicate x : schemeList)
	{
		myDatabase.addRelation(x.getPredicateName(), x.getPredicateParameters());
	}

	const list<predicate> factList = dataLog.getFacts();
	//create tuples
	for (predicate x : factList)
	{

	}

	queryList = dataLog.getQueries();

	ruleList = dataLog.getRules();
}

