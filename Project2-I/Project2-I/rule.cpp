#include "rule.h"
#include <exception>
#include "predicate.h"

using namespace std;

rule::rule()
{
}

rule::rule(list<predicate> predicateList)
{
	this->predicateList = predicateList;
}


rule::~rule()
{
}

rule rule::makeRule(list<token>& tokenList)
{
	predicate rulePredicate;

	list<predicate> rulePredicateList;

	rulePredicateList.push_back(rulePredicate.headPredicate(tokenList));

	checkComment(tokenList);

	match(tokenList, COLON_DASH);

	checkComment(tokenList);

	rulePredicateList = rulePredicate.predicateList(tokenList, rulePredicateList);

	checkComment(tokenList);

	match(tokenList, PERIOD);

	checkComment(tokenList);

	rule tempRule(rulePredicateList);

	return tempRule;
}

list<rule> rule::ruleList(list<token>& tokenList)
{
	checkComment(tokenList);

	match(tokenList,RULES);

	checkComment(tokenList);

	match(tokenList,COLON);

	checkComment(tokenList);

	list<rule> ruleList;
	//have to check for lambda! here and facts...

	while (tokenList.front().getType() == ID)
	{
		ruleList.push_back(makeRule(tokenList));
	}

	return ruleList;
}

void rule::match(list<token>& tokenList, tokenType tokenToMatch)
{
	tokenList.front().getType() == tokenToMatch ? tokenList.pop_front() : error(tokenList.front());
}

void rule::error(token badToken)
{
	throw badToken;
}

void rule::checkComment(list<token>& tokenList)
{
	while (tokenList.front().getType() == COMMENT)
	{
		tokenList.pop_front();
	}
}