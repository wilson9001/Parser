#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "database.h"
#include <list>
#include "predicate.h"
#include "rule.h"
#include "datalogProgram.h"
#include <list>
#include <sstream>

class interpreter
{
public:
	interpreter();
	~interpreter();

	//void function(vector<Reltion> const &r)

	void createDataBase(datalogProgram &dataLog);

	void evaluateQueries();

	void answerQuery(predicate queryToAnswer);

private:
	database myDatabase;
	list<predicate> queryList;
	list<rule> ruleList;
	stringstream queryResults;
};

#endif