#include "interpreter.h"

using namespace std;

interpreter::interpreter()
{
	passesThroughRules = 0;
}

interpreter::~interpreter()
{
}

void interpreter::createDataBase(datalogProgram dataLog)
{//may need to pass by reference...

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

	passThroughRules();
}

void interpreter::passThroughRules()
{
	size_t previousDBSize;

	do
	{
		previousDBSize = myDatabase.size();
		passesThroughRules++;

		evaluateRules();
	} while (previousDBSize != myDatabase.size());
}

void interpreter::evaluateRules()
{
	//cout << "evaluateRules called.";

	for (rule x : ruleList)
	{
		evaluateRule(x);
	}
}

void interpreter::evaluateRule(rule ruleToEvaluate)
{//Previously passed by reference.

	/*Repeatedly join relations until there are no more relations to join, then switch columns to match the one in the head predicate. Then after the completed relation is finished, add to existing relation object that matches the scheme.*/
	predicate headPredicate = ruleToEvaluate.getNextPredicate();

	ruleToEvaluate.popPredicate();

	relation newRelation = myDatabase.getRelation((ruleToEvaluate.getNextPredicate()).getPredicateName());

	//WORK HERE: NEED TO REDO SCHEME SO THAT THE EXISTING RELATION'S SCHEME IS OVERWRITTEN
	predicate tempPredicate = ruleToEvaluate.getNextPredicate();

	list<parameter> parameterList = tempPredicate.getPredicateParameters();
	newRelation.reScheme(parameterList);

	ruleToEvaluate.popPredicate();
	//Can change this to set newRelation equal to existing relation, then update join function to keep joining with new predicate until predicates are empty by performing internal operations. I updated the make new scheme class, but I will need to update the others as well.

	while (ruleToEvaluate.predicatesRemaining() > 0)
	{//Need to rescheme incoming predicate before join or first thing in join.
		tempPredicate = ruleToEvaluate.getNextPredicate();
		list<parameter> tempParameters = tempPredicate.getPredicateParameters();
		newRelation.join(myDatabase.getRelation(tempPredicate.getPredicateName()), tempParameters);//Parameterlist added to rescheme incoming relation.
			//Only need to add 1 relation to join with itself;

		ruleToEvaluate.popPredicate();
	}

	newRelation.fixColumns(headPredicate); //<--this will reorder columns and remove columns not in the head predicate.
	myDatabase.Union(headPredicate.getPredicateName(), newRelation.getTuples());
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
	queryResults << "Schemes populated after " << passesThroughRules << " passes through the Rules.\n";

	//for (predicate x : queryList)
	for(auto iter = queryList.begin(); iter != queryList.end(); iter++)
	{
		//queryResults << x.getPredicateName() << "(";

		if (iter != queryList.begin())
		{
			queryResults << endl;
		}

		predicate tempPredicate = *iter;

		queryResults << tempPredicate.getPredicateName() << "(";
		//addQueryParameters(x.getPredicateParameters());
		addQueryParameters(tempPredicate.getPredicateParameters());

		queryResults << ")? ";

		//printTuples(x);
		printTuples(*iter);

		//queryResults << endl;
		/////////////////////////////////////////////////////////////
		/*if (++iter != queryList.end())
		{
			queryResults << "3\n";
						//^may need to fix this
		}
		iter--;*/
		///////////////////////////////////////////////////////////
		//cout << queryResults.str();
	}

	//cout queryResults after final formatting (remove last endl) here.
	cout << queryResults.str();
}

void interpreter::addQueryParameters(list<parameter> const &queryParameters)
{
	stringstream tempStream;

	for (auto x : queryParameters)
	{
		/*
		switch (x.getParameterType())
		{
		case iD:
			tempStream << x.getParameterValue() << ",";
			break;

			//must be string
		default:
			tempStream << "'" << x.getParameterValue() << "',";
		}
		*/
		tempStream << x.getParameterValue() << ",";
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
		queryResults << "No" /*<< endl*/;
		return;
	}

	queryResults << "Yes(" << tuplesToPrint.size() << ")" /*<< "2\n2"*/;
	//														//^^ may need to comment this out
	scheme tempScheme = tempRelation.getScheme();
	if (!tempScheme.empty())
	{
		queryResults << endl;
	}
	//for (Tuple x : tuplesToPrint)

	//set<Tuple>::iterator iter = tuplesToPrint.begin();
	//queryResults << "2\n2";
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
	//queryResults << endl;
	for (size_t i = 0; i < schemeToPrint.size(); i++)
	{
		//Needs to be two spaces for first tuple, one space for each tuple after that.
		queryResults << "  " << schemeToPrint[i] << "=" << tupleToPrint[i];

		if (i != (schemeToPrint.size() - 1))
		{
			queryResults << ",";
		}
	}
}