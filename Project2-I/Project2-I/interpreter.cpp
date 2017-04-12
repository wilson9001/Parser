#include "interpreter.h"

using namespace std;

interpreter::interpreter()
{
	passesThroughRules = 0;
}

interpreter::~interpreter()
{
}

void interpreter::createDataBase(datalogProgram &dataLog)
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

	doRules();
	//passThroughRules();
}

void interpreter::passThroughRules(set<int> &componentToEvaluate)
{
	size_t previousDBSize;

	//this fix point algorithm will need to be moved elsewhere...
	do
	{
		previousDBSize = myDatabase.size();
		//passesThroughRules++;

		//This will need to be replaced by the rule optimization and then running the queries
		evaluateRules(componentToEvaluate);
	} while (previousDBSize != myDatabase.size());
	
}

//previously had no input, used ruleList
void interpreter::evaluateRules(set<int> &componentToEvaluate)
{
	/*
	createGraphs();

	reverseGraph.DFSForest();

	stack<int> postOrderNumbers = reverseGraph.getPostOrderStack();
	forwardGraph.setPostOrderStack(postOrderNumbers);
	forwardGraph.createComponents();

	queue<set<int>> components = forwardGraph.getComponents();
	
	//loop
	while (!components.empty())
	{
		set<int> component = components.front();
		components.pop();
		*/
		/*If the set component only has size 1, check to see if the node dependencies includes itself by checking in the dependency set for that node. If yes, then */
		/*if (component.size() == 1)
		{
			if (forwardGraph.selfDependent(component))
			{
				evaluateTrivialComponent(*component.begin());
				continue;
			}
		}
		evaluateComponent(component);
	}*/

	//endloop
	//forwardGraph.
	//This will need to be changed to only evaluate rules in a certain subset of the rules...
	
	//for (rule x : ruleList)
	auto iter = ruleList.begin();

	for(int x : componentToEvaluate)
	{
		iter = ruleList.begin();
		advance(iter, x);

		evaluateRule(*iter);
	}
}

void interpreter::evaluateRule(rule &ruleToEvaluate)
{//Previously passed by reference.
	ruleToEvaluate.incrementPasses();
	/*Repeatedly join relations until there are no more relations to join, then switch columns to match the one in the head predicate. Then after the completed relation is finished, add to existing relation object that matches the scheme.*/
	predicate headPredicate = ruleToEvaluate.getNextPredicate();

	ruleToEvaluate.popPredicate();

	predicate tempPredicate = ruleToEvaluate.getNextPredicate();
	answerQuery(tempPredicate);

	//predicate tempPredicate = ruleToEvaluate.getNextPredicate();
	string queryKey = queryMapKeyGenerator(tempPredicate);
	relation newRelation = relationResults[queryKey];
	relationResults.clear();

	ruleToEvaluate.popPredicate();
	//Can change this to set newRelation equal to existing relation, then update join function to keep joining with new predicate until predicates are empty by performing internal operations. I updated the make new scheme class, but I will need to update the others as well.

	while (ruleToEvaluate.predicatesRemaining() > 0)
	{
		predicate tempPredicate = ruleToEvaluate.getNextPredicate();
		answerQuery(tempPredicate);

		string mapQuery = queryMapKeyGenerator(tempPredicate);
		relation relationToJoin = relationResults[mapQuery];

		relationResults.clear();

		newRelation.join(relationToJoin);

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
//added CONST for experiment
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
	queryResults << forwardGraph.createGraphPrint() << endl << "Rule Evaluation";



	//DOn't need this anymore...
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
		/*
		if (it == tuplesToPrint.begin())
		{
			//not here. Needs to go in printTuple...
			queryResults << "2";
		}
		*/

		auto a = *it;
		printTuple(a, tempScheme);

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

void interpreter::printTuple(Tuple &tupleToPrint, scheme &schemeToPrint)
{
	//queryResults << endl;
	for (size_t i = 0; i < schemeToPrint.size(); i++)
	{
		if (i == 0)
		{
			queryResults << " ";
		}
		//Needs to be two spaces for first tuple, one space for each tuple after that.
		queryResults << " " << schemeToPrint[i] << "=" << tupleToPrint[i];

		if (i != (schemeToPrint.size() - 1))
		{
			queryResults << ",";
		}
	}
}

void interpreter::createGraphs()
{
	forwardGraph.populateGraph(ruleList.size());
	reverseGraph.populateGraph(ruleList.size());

	//rule x : ruleList
	size_t iIndex = 0;
	size_t zIndex = 0;

	for (auto i = ruleList.begin(); i != ruleList.end(); i++)
	{
		//list<predicate> tempList = x.getRulePredicates(); <-- may need this
		for (predicate y : (*i).getRulePredicates()/*x.getRulePredicates()*/)
		{
			zIndex = 0;
			//rule z : ruleList
			for (auto z = ruleList.begin(); z != ruleList.end(); z++)
			{
				if (y.getPredicateName() == (*z).getNextPredicate().getPredicateName())
				{
					//Going to have to create an iterator to go through the list of rules later...
					forwardGraph.addEdge(iIndex, zIndex);
					reverseGraph.addEdge(zIndex, iIndex);
				}
				zIndex++;
			}
		}
		iIndex++;
	}
}

/*void interpreter::evaluateTrivialComponent(int trivialComponent)
{
	auto iter = ruleList.begin();
	advance(iter, trivialComponent);
	/////////////////////////
}*/

/*void interpreter::evaluateComponent(set<int> component)
{
	auto iter = ruleList.begin();

	for (int x : component)
	{
		iter = ruleList.begin();
		advance(iter, x);
		evaluateRule(*iter);
	}
}*/

void interpreter::doRules()
{
	createGraphs();

	reverseGraph.DFSForest();

	stack<int> postOrderNumbers = reverseGraph.getPostOrderStack();
	forwardGraph.setPostOrderStack(postOrderNumbers);
	forwardGraph.createComponents();

	queue<set<int>> components = forwardGraph.getComponents();

	//loop
	while (!components.empty())
	{
		passesThroughRules = 0;
		set<int> component = components.front();
		components.pop();

		/*If the set component only has size 1, check to see if the node dependencies includes itself by checking in the dependency set for that node. If yes, then */
		if (component.size() == 1)
		{
			if (forwardGraph.selfDependent(component))
			{
				auto iter = ruleList.begin();
				advance(iter, *component.begin());
				
				evaluateRule(*iter);
				passesThroughRules++;

				continue;
			}
		}
		passThroughRules(component);
		//evaluateComponent(component);
	}
}
