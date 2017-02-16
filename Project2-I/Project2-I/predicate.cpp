#include "predicate.h"
#include <list>
#include "token.h"
#include "parser.h"
#include <exception>
#include "parameter.h"

/*
predicate::predicate() :predicateName(UNDEFINED, "This should never show up", 0)
{
	//predicateName = (UNDEFINED, "THIS SHOULD NEVER SHOW UP", 0);
}
*/

predicate::predicate()
{
}

predicate::predicate(string predicateName, list<parameter> ParameterList)
{
	this->predicateName = predicateName;
	this->ParameterList = ParameterList;
}

//alternate constructor here for when creating objects to store.

predicate::~predicate()
{
}

list<predicate> predicate::makePredicate(list<token>& tokenList)
{
	list<predicate> predicateList;

	switch (tokenList.front().getType())
	{
	case SCHEMES:
		predicateList = schemeList(tokenList);
		break;
	case FACTS:
		predicateList = factList(tokenList);
		break;
	case QUERIES:
		predicateList = queryList(tokenList);
		break;
	default: error(tokenList.front());
	}

	return predicateList;
}

list<predicate> predicate::predicateList(list<token>& tokenList, list<predicate> addPredicateList)
{
	predicate tempPredicate;

	addPredicateList.push_back(tempPredicate.makeRulePredicate(tokenList));

	checkComment(tokenList);

	if (tokenList.front().getType() == COMMA)
	{
		match(tokenList, COMMA);
		checkComment(tokenList);

		return predicateList(tokenList, addPredicateList);
	}

	else
	{
		return addPredicateList;
	}
}

predicate predicate::headPredicate(list<token>& tokenList)
{
	string predicateName = tokenList.front().getLiteralValue();

	match(tokenList, tokenType::ID);

	checkComment(tokenList);

	match(tokenList, LEFT_PAREN);
	//
	list<parameter> parameters;

	parameter ruleParameter;

	//checkComment(tokenList);

	parameters = ruleParameter.parameterList(tokenList, SCHEMES);

	predicate ruleHeadPredicate(predicateName, parameters);

	return ruleHeadPredicate;
}

//*This function represents the SCHEMES grammar.
//*If the tokens don't follow COLON, LEFT_PAREN, ID, idlist,RIGHT_PAREN, then an error is thrown.
//*Else, a predicate object is created.
predicate predicate::scheme(list<token> &tokenList)
{
	token PredicateName = tokenList.front();
	//predicateName = tokenList.front();

	match(tokenList, tokenType::ID);

	checkComment(tokenList);

	match(tokenList, LEFT_PAREN);

	checkComment(tokenList);

	parameter tokenParameter;

	list<parameter> parameterList;

	//make list of ID's
	parameterList = tokenParameter.parameterList(tokenList, SCHEMES);

	checkComment(tokenList);

	match(tokenList, RIGHT_PAREN);

	checkComment(tokenList);
	//predicate newScheme(predicateName.getLiteralValue(), parameterList)

	//return newScheme;

	/*
	if (tokenToParse.getType() == COLON)
	{
		tokenToParse = getToken();
		token schemename = tokenToParse;
		match(ID);
		match(LEFT_PAREN);
		token param1 = tokenToParse;
		match(ID);

		list<token> tokenList;
		tokenList.push_back(param1);

		tokenList = idList(tokenList);

		//recursively call schemes if needed.
	}
	//////////////////make list of id's.



	else if (tokenToParse.getType() == COMMENT)
	{
		match(COMMENT);
		return scheme();
	}

	else
	{
		error(tokenToParse);
	}
	/*Goes to:
	ID LEFT_PAREN ID idList RIGHT_PAREN
	*/
}

list<predicate> predicate::schemeList(list<token>& tokenList)
{
	list<predicate> predicateList;

	match(tokenList, SCHEMES);

	checkComment(tokenList);

	match(tokenList, COLON);

	checkComment(tokenList);

	predicateList.push_back(scheme(tokenList));

	while (tokenList.front().getType() == ID)
	{
		predicateList.push_back(scheme(tokenList));
	}

	return predicateList;
}

predicate predicate::fact(list<token>& tokenList)
{
	token predicateName = tokenList.front();

	match(tokenList,tokenType::ID);

	checkComment(tokenList);

	match(tokenList,LEFT_PAREN);

	checkComment(tokenList);

	parameter tokenParameter;

	list<parameter> parameterList;

	parameterList = tokenParameter.parameterList(tokenList, FACTS);

	//predicate predicateFact(predicateName.literalvalue(),parameterList)
	predicate predicateFact(predicateName.getLiteralValue(), parameterList);

	return predicateFact;
}

list<predicate> predicate::factList(list<token>& tokenList)
{
	list<predicate> predicateList;

	match(tokenList, FACTS);

	checkComment(tokenList);

	match(tokenList, COLON);

	checkComment(tokenList);

	while (tokenList.front().getType() == ID)
	{
		predicateList.push_back(fact(tokenList));
	}

	return predicateList;
}

predicate predicate::query(list<token>& tokenList)
{
	token predicateName = tokenList.front();

	match(tokenList, tokenType::ID);

	checkComment(tokenList);

	match(tokenList, LEFT_PAREN);

	parameter tokenParameter;

	list<parameter> parameterList;

	parameterList = tokenParameter.parameterList(tokenList, QUERIES);

	checkComment(tokenList);

	predicate predicateQuery(predicateName.getLiteralValue(), parameterList);

	return predicateQuery;
}

list<predicate> predicate::queryList(list<token>& tokenList)
{
	list<predicate> predicateList;

	match(tokenList, QUERIES);

	checkComment(tokenList);

	match(tokenList,COLON);

	checkComment(tokenList);

	predicateList.push_back(query(tokenList));

	while (tokenList.front().getType() == ID)
	{
		predicateList.push_back(query(tokenList));
	}

	return predicateList;
}

//*Compares the current token's tokentype to the tokentype passed in.
//*If they are a match, then the tokenlist has it's front member popped. This means that if the value needs to be stored it should be done before the list is passed into the function.
//*Else the token is thrown.
void predicate::match(list<token> &tokenToTest,tokenType testTokenType)
{
	tokenToTest.front().getType() == testTokenType ? tokenToTest.pop_front() : error(tokenToTest.front());
}

void predicate::checkComment(list<token> &tokenToTest)
{
	while (tokenToTest.front().getType() == COMMENT)
	{
		tokenToTest.pop_front();
	}
}

predicate predicate::makeRulePredicate(list<token> &tokenList)
{
	token predicateName = tokenList.front();

	match(tokenList, tokenType::ID);

	checkComment(tokenList);

	match(tokenList, LEFT_PAREN);

	parameter tokenParameter;

	list<parameter> parameterList;

	parameterList = tokenParameter.parameterList(tokenList, RULES);

	checkComment(tokenList);

	predicate predicateQuery(predicateName.getLiteralValue(), parameterList);

	return predicateQuery;
	return predicate();
}

void predicate::error(token BadToken)
{
	throw BadToken;
}