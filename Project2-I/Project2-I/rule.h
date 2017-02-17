#ifndef RULE_H_
#define RULE_H_

#include <list>
#include "token.h"
#include "predicate.h"

using namespace std;

class rule
{
public:
	rule();
	rule(list<predicate> predicateList);
	~rule();

	rule makeRule(list<token> &tokenList);

	list<rule> ruleList(list<token> &tokenList);

	void match(list<token> &tokenList, tokenType tokenToMatch);

	void error(token badToken);

	void checkComment(list<token> &tokenList);

	list<predicate> getRulePredicates() { return predicateList; }

private:

	//*First predicate is headpredicate.
	//*Name of headPredicate is name of rule.
	list<predicate> predicateList;
};

#endif