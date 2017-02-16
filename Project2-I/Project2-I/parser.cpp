#include "parser.h"
#include "token.h"
#include "scanner.h"
#include <iostream>

parser::parser(list<token> parsingTokens):parsingTokens(parsingTokens),tokenToParse(getToken())
{
}

parser::~parser()
{
}

void parser::parse()
{
	try
	{
		/*for (auto x : parsingTokens)
		{
			datalogProgram(getToken());
		}
		*/

		while (!parsingTokens.front().getType() != MYEOF)
		{
			makeDatalogProgram();
		}
	}

	catch(token badToken)
	{
		cout << "Failure!\n\t(" << badToken.getType() << ", \"" << badToken.getLiteralValue() << "\"," << badToken.getLineNumber() << ")";
	}
	//wrap datalogProgram in try-catch block.
}

//*Compares the current token's tokentype to the tokentype passed in.
//*If they are a match, then the token is advanced to the next token in the list.
//*Else the token is thrown.
void parser::match(tokenType testTokenType)
{
	tokenToParse.getType() == testTokenType ? tokenToParse = getToken() : error(tokenToParse);
	/*if (token == t)
		token = getToken();

	else
		error();*/
}

//*This function represents the start position for all grammars.
//*If the next token is not FACTS, RULES, SCHEMES, QUERIES, then the token is thrown.
void parser::makeDatalogProgram()
{
	predicate myPredicate;
	rule myRule;

	//check for comments

	switch (tokenToParse.getType())
	{
	case SCHEMES:
		myProgram.addSchemes(myPredicate.makePredicate(parsingTokens));
		break;
	case FACTS:
		myProgram.addFacts(myPredicate.makePredicate(parsingTokens));
		break;
	case RULES:
		myProgram.addRules(myRule.ruleList(parsingTokens));
		break;
	case QUERIES:
		myProgram.addQueries(myPredicate.makePredicate(parsingTokens));
		break;
	case COMMENT:
		tokenToParse = getToken();
		return;
	default:
		error(tokenToParse);
	}
	/* Goes to:
	SCHEMES COLON scheme schemeList
	FACTS COLON factList
	RULES COLON ruleList
	QUERIES COLON query queryList
	*/
}

//Returns the token at the front of the list and pops it off.
token parser::getToken()
{
	token tempToken = parsingTokens.front();
	parsingTokens.pop_front();
	return tempToken;
}

void parser::error(token badToken)
{
	throw badToken;
}