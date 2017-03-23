#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "database.h"
#include <list>
#include "predicate.h"
#include "rule.h"
#include "datalogProgram.h"
#include <list>
#include <sstream>
#include <unordered_map>
#include "relation.h"

class interpreter
{
public:
	interpreter();
	~interpreter();

	//void function(vector<Reltion> const &r)

	void createDataBase(datalogProgram dataLog);

	/*
	void evaluateRules()
	  will need to evaluate each relation in a rule as a query, join results together, rename columns to match existing schema, then union with existing facts in their appropriate schemes. 
	*/
	void evaluateRules();

	void evaluateQueries();

	void answerQuery(predicate &queryToAnswer);

	string queryMapKeyGenerator(predicate &queryToStringify);

	void printQueryResults();

	void addQueryParameters(list<parameter> const &queryParameters);

	void printTuples(predicate &QueryToFetchResults);

	void printTuple(Tuple tupleToPrint, scheme schemeToPrint);

private:
	database myDatabase;
	list<predicate> queryList;
	list<rule> ruleList;
	stringstream queryResults;

	//Will store relation results in here to make sure all the results are organized properly. Will use existing queryList to order the printing out of them. Key will have to be entire query, since the scheme can be queried in multiple ways, and each way will need their own result relation mapped.
	unordered_map<string, relation> relationResults;
};

#endif