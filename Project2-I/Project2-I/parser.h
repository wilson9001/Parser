#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"
#include "token.h"
#include <list>
#include "datalogProgram.h"

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

private:

	list<token> parsingTokens;
	token tokenToParse;
	datalogProgram myProgram;
};

#endif