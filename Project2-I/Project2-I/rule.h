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

	predicate getNextPredicate() { return predicateList.front(); }

	void popPredicate() { predicateList.pop_front(); }

	size_t predicatesRemaining() { return predicateList.size(); }

	//void incrementPasses() { passes++; }

	//size_t returnPasses() { return passes; }

private:

	//*First predicate is headpredicate.
	//*Name of headPredicate is name of rule.
	list<predicate> predicateList;
	//size_t passes;
};

#endif