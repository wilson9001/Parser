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

	printQueryResults();
}

void interpreter::answerQuery(predicate &queryToAnswer)
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
	//PROBLEM HERE ^^^

	//we now have the final relation result for this query. We need to do an insert into the unordered map with the key as the whole query string and the relational result as the mapped value. Since running the same query twice should produce the same results, it doesn't matter if an insert is ignored if run again.
	relationResults.insert(pair<string, relation>(queryMapKeyGenerator(queryToAnswer), tempRelation));
}

//*Turns entire query statement into a string to be used in the map of result relations, and in printing queries in the printResults function.
string interpreter::queryMapKeyGenerator(predicate &queryToStringify)
{
	stringstream keyBuilder;

	keyBuilder << queryToStringify.getPredicateName() << "(";

	for (parameter p : queryToStringify.getPredicateParameters())
	{
		switch (p.getParameterType())
		{
		case iD:
			keyBuilder << p.getParameterValue() << ",";
			break;

			//Must be string...
		default:
			keyBuilder << "'" << p.getParameterValue() << "',";
		}
	}

	string finalKey = keyBuilder.str().substr(0, keyBuilder.str().length() - 1);

	finalKey += ")?";

	return finalKey;
}

//*Iterates through the query vector and pulls out the result relation from the map for each query,
//get the size of the set of tuples, and then iterate through the set.
void interpreter::printQueryResults()
{
	//for (predicate x : queryList)
	for(auto iter = queryList.begin(); iter != queryList.end(); iter++)
	{
		//queryResults << x.getPredicateName() << "(";
		queryResults << (*iter).getPredicateName() << "(";
		//addQueryParameters(x.getPredicateParameters());
		addQueryParameters((*iter).getPredicateParameters());

		queryResults << ")? ";

		//printTuples(x);
		printTuples(*iter);

		//queryResults << endl;
		if (++iter != queryList.end())
		{
			queryResults << endl;
		}
		iter--;
	}

	//cout queryResults after final formatting (remove last endl) here.
}

void interpreter::addQueryParameters(list<parameter> &queryParameters)
{
	stringstream tempStream;

	for (auto x : queryParameters)
	{
		switch (x.getParameterType())
		{
		case iD:
			tempStream << x.getParameterValue() << ",";
			break;

			//must be string
		default:
			tempStream << "'" << x.getParameterValue() << "',";
		}
	}
	string tempString = tempStream.str();

	 queryResults << tempString.substr(0, tempString.size() - 1);
}

void interpreter::printTuples(predicate &QueryToFetchResults)
{
	relation tempRelation = relationResults[queryMapKeyGenerator(QueryToFetchResults)];

	set<Tuple> tuplesToPrint = tempRelation.getTuples();

	if (tuplesToPrint.size() == 0)
	{
		queryResults << "No" << endl;
		return;
	}

	queryResults << "Yes(" << tuplesToPrint.size() << ")" << endl;

	scheme tempScheme = tempRelation.getScheme();

	//for (Tuple x : tuplesToPrint)

	//set<Tuple>::iterator iter = tuplesToPrint.begin();

	//for(int i = 0; i < tuplesToPrint.size(); i ++)
	for(auto it = tuplesToPrint.begin(); it != tuplesToPrint.end(); it++)
	{
		//printTuple(x, tempScheme);

		printTuple(*it, tempScheme);

		if (++it != (tuplesToPrint.end()))
		{
			queryResults << endl;
		}
		it--;
	}
	//insert newline here except on last iteration?

	/*
	for (int i = 0; i < tuplesToPrint.size(); i++)
	{

	}
	*/
}

void interpreter::printTuple(Tuple tupleToPrint, scheme schemeToPrint)
{
	for (size_t i = 0; i < schemeToPrint.size(); i++)
	{
		queryResults << "  " << schemeToPrint[i] << "='" << tupleToPrint[i] << "'";

		if (i != (schemeToPrint.size() - 1))
		{
			queryResults << ", ";
		}
	}
}