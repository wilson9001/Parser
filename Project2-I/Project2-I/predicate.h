#ifndef PREDICATE_H_
#define PREDICATE_H_

#include "token.h"
#include "parameter.h"

class predicate
{
public:
	predicate();
	predicate(string predicateName, list<parameter> ParameterList);
	~predicate();

	list<predicate> makePredicate(list<token> &tokenList);

	list<predicate> predicateList(list<token> &tokenList, list<predicate> addPredicateList);

	predicate headPredicate(list<token> &tokenList);

	predicate scheme(list<token> &tokenList);

	list<predicate> schemeList(list<token> &tokenList);

	predicate fact(list<token> &tokenList);

	list<predicate> factList(list<token> &tokenList);

	predicate query(list<token> &tokenList);

	list<predicate> queryList(list<token> &tokenList);

	void error(token BadToken);

	void match(list<token> &tokenToTest, tokenType testTokenType);

	void checkComment(list<token> &tokenToTest);

	string getPredicateName() { return predicateName; }

	list<parameter> getPredicateParameters() { return ParameterList; }

	predicate makeRulePredicate(list<token> &tokenList);

private:
	string predicateName;
	list<parameter> ParameterList;
};

#endif