#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"
#include "token.h"
#include <list>
#include "datalogProgram.h"
#include "parameter.h"

class parser
{
public:
	parser(list<token> parsingTokens);
	~parser();

	void parse();

	void match(tokenType testTokenType);

	void makeDatalogProgram();

	token getToken();

	//list<token> returnList(tokenType tokenToList);

	void error(token badToken);

	void printDatalogProgram();

	string printRulePredicateParameters(list<parameter> predicateParameter);

	void printFacts(list<predicate> predicateList);

	void printFactParameters(list<parameter> parameterList);

	void printSchemes(list<predicate> predicateList);

	void checkComment(list<token> &tokenList);

	void purgeComments();

	datalogProgram getDatalog() { return myProgram; }

private:

	list<token> parsingTokens;
	//token tokenToParse;
	datalogProgram myProgram;
};

#endif