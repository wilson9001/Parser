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
		answerQuery(x);
	}

	//In the print results function, we will be able to go through the query vector and pull out the result relation from the map for each result, get the size of the set of tuples, and then iterate through the set.
}

void interpreter::answerQuery(predicate queryToAnswer)
{
	relation tempRelation = myDatabase.getRelation(queryToAnswer.getPredicateName());

	//create two vectors, one of rename names and the other of ints corresponding to columns to include in new, reduced relation. These will need to be passed into the select function for ID's, as well as the project function. The list of names will need to be passed to the rename function.

	vector<string> renameNames;
	vector<int> columnsToProject;

	//go through each parameter in query, use appropriate select function for type of parameter.

	size_t parameterIndex = 0;

	for (parameter x : queryToAnswer.getPredicateParameters())
	{
		switch (x.getParameterType())
		{
		case iD:
			//If ID, add to rename and project list. If name already exists in rename, do not add to project again but instead do a select where the columns equal each other. Then move to next parameter.
			tempRelation.selectID(x.getParameterValue(), parameterIndex);
			break;

			//Must be a string
		default:
			tempRelation.selectAbsolute(x.getParameterValue(), parameterIndex);
			//If string, immediately perform select where column number of parameter index = the string value.
		}

		parameterIndex++;
	}

	tempRelation.project();
	//using project function, reduce columns in indexes specified
	//rename columns to list of project variables
	tempRelation.rename();
	//we now have the final relation result for this query. We need to do an insert into the unordered map with the key as the whole query string and the relational result as the mapped value. Since running the same query twice should produce the same results, it doesn't matter if an insert is ignored if run again.
}