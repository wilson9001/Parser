#ifndef DATALOGPROGRAM_H_
#define DATALOGPROGRAM_H_

#include "parameter.h"
#include "predicate.h"
#include "rule.h"
#include <list>
#include "token.h"
#include <set>

class datalogProgram
{
public:
	datalogProgram();
	~datalogProgram();

	void addFacts(list<predicate> addFacts);

	void addSchemes(list<predicate> addSchemes);

	void addQueries(list<predicate> addQueries);

	void addRules(list<rule> addRules);

	list<predicate> getFacts() { return factList; }

	list<predicate> getSchemes() { return schemeList; }

	list<predicate> getQueries() { return queryList; }

	list<rule> getRules() { return ruleList; }

private:
	list<predicate> factList;
	list<predicate> schemeList;
	list<predicate> queryList;
	list<rule> ruleList;

	set<string> domain;
};

#endif