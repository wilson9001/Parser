#include "interpreter.h"

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
		myDatabase.addTuples(x.getPredicateName(), x.getPredicateParameters());
	}

	queryList = dataLog.getQueries();

	ruleList = dataLog.getRules();
}

void interpreter::evaluateQueries()
{
	for (predicate x : queryList)
	{

	}
}

void interpreter::answerQuery(predicate queryToAnswer)
{
}

