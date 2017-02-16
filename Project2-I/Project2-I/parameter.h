#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

enum dataType{STRING,ID,EXPRESSION};

//class expression;

class parameter
{
public:
	parameter();

	parameter(string parameterValue, dataType parameterType);

	virtual ~parameter();

	//IDLIst, stringlist, querylist need to go here...

	list<parameter> makeParameters(list<token> &tokenList, list<parameter> parameterList, tokenType typeOfPredicate);
	//list<parameter> makeParameter(list<token> &tokenList, tokenType typeOfPredicate);

	list<parameter> parameterList(list<token> &tokenList, tokenType typeOfPredicate);
	//list<parameter> parameterList(list<token> &tokenList);

	parameter Operator(list<token> &tokenList);

	list<parameter> idList(list<token> &tokenList, list<parameter> idParameterList);

	list<parameter> stringList(list<token> &tokenList, list<parameter> stringParameterList);

	void match(list<token> &tokens, tokenType tokenToMatch);

	void error(token badToken) { throw badToken; }

	void checkComment(list<token> &tokenList);

	list<parameter> QueryParameterList(list<token> &tokenList, list<parameter> queryParameterList);

	parameter queryParameter(list<token> &tokenList, dataType typeOfParameter);

	list<parameter> ruleList(list<token> &tokenList, list<parameter> ruleParameterList);

	string getParameterValue() { return parameterValue; }

	dataType getParameterType() { return parameterType; }

	void checkComma(list<token> &tokenList);

private:
	string parameterValue;
	dataType parameterType;
};

class expression : public parameter
{
public:
	expression();
	expression(string expressionString);

	~expression();

	//string getParameterValue();

	expression makeExpression(list<token> &tokenList);

	void validateExpression(list<token> tokenList);
};
#endif