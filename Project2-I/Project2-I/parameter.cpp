#include "parameter.h"
#include <list>
#include "token.h"
#include <sstream>

using namespace std;

parameter::parameter()
{
}

parameter::parameter(string parameterValue, dataType parameterType)
{
	this->parameterType = parameterType;
	this->parameterValue = parameterValue;
}

parameter::~parameter()
{
}


//list<parameter> parameter::makeParameter(list<token>& tokenList,tokenType typeOfPredicate)

//Redirects parameter finding process to appropriate function based on predicate type.
list<parameter> parameter::parameterList(list<token>& tokenList, tokenType typeOfPredicate)
{
	list<parameter> parameterList;
	switch (typeOfPredicate)
	{
	case SCHEMES:
		parameterList = idList(tokenList, parameterList);
		break;
	case FACTS:
		parameterList = stringList(tokenList, parameterList);
		break;
	case RULES:
		//parameterList = makeParameters(tokenList, parameterList, RULES);
		checkparameterlength(tokenList);
		parameterList = ruleList(tokenList, parameterList);
		break;
	case QUERIES:
		//parameterList = makeParameters(tokenList, parameterList, QUERIES);
		checkparameterlength(tokenList);
		parameterList = QueryParameterList(tokenList, parameterList);
		break;
	default:
		error(tokenList.front());
	}

	return parameterList;
	/*
	token firstParameter(UNDEFINED, "YOU SHOULD NOT SEE THIS", 0);
	list<parameter> parameterList;

	switch (typeOfPredicate)
	{
	case SCHEMES:
		firstParameter = tokenList.front();
		match(tokenList, tokenType::ID);
		////////////////
	case FACTS:

	case QUERIES:

	}

	return parameter();*/
}

//list<parameter> parameter::parameterList(list<token>& tokenList)
/*
list<parameter> parameter::makeParameters(list<token> &tokenList, list<parameter> parameterList, tokenType typeOfPredicate)
{

	switch (typeOfPredicate)
	{
	case QUERIES:

	case FACTS:

	default: error(tokenList.front());
	}

}
*/

void parameter::Operator(list<token>& tokenList)
{
	switch (tokenList.front().getType())
	{
	case ADD:
	case MULTIPLY:
		return;
	default: error(tokenList.front());
	}
}

list<parameter> parameter::idList(list<token>& tokenList, list<parameter> idParameterList)
{
	checkComment(tokenList);

	parameter idParameter(tokenList.front().getLiteralValue(), iD);

	idParameterList.push_back(idParameter);

	match(tokenList, ID);

	checkComment(tokenList);

	if (tokenList.front().getType() == COMMA)
	{
		match(tokenList, COMMA);
		checkComment(tokenList);
		return idList(tokenList, idParameterList);
	}

	else
	{
		match(tokenList, RIGHT_PAREN);
		checkComment(tokenList);

		return idParameterList;
	}
	/*--comment
	check for id, add ID to list of ID's
	--comment
	check for comma, if yes match comma and call IDlist again
	else match right paren and return IDlist
	*/
	/*list<token> predicate::idList(list<token> &tokenList)
{
	switch (tokenToParse.getType())
	{
	case COMMA:
		match(COMMA);
		tokenList.push_back(tokenToParse);
		match(ID);
		return idList(tokenList);
	case RIGHT_PAREN:
		match(RIGHT_PAREN);
		return tokenList;
	case COMMENT:
		match(COMMENT);
		return idList(tokenList);
	default:
		error(tokenToParse);
	}
	//goes to: COMMA ID idList | lambda
}*/
}

list<parameter> parameter::stringList(list<token>& tokenList, list<parameter> stringParameterList)
{
	checkComment(tokenList);

	parameter stringParameter(tokenList.front().getLiteralValue(), sTRING);

	stringParameterList.push_back(stringParameter);

	match(tokenList, STRING);

	checkComment(tokenList);

	if (tokenList.front().getType() == COMMA)
	{
		match(tokenList,COMMA);
		return stringList(tokenList, stringParameterList);
	}

	else
	{
		match(tokenList,RIGHT_PAREN);
		checkComment(tokenList);
		match(tokenList, PERIOD);
		checkComment(tokenList);

		return stringParameterList;
	}
}

void parameter::match(list<token>& tokens, tokenType tokenToMatch)
{
	tokens.front().getType() == tokenToMatch ? tokens.pop_front() : error(tokens.front());
}

void parameter::checkComment(list<token>& tokenList)
{
	while (tokenList.front().getType() == COMMENT)
	{
		tokenList.pop_front();
	}
}

list<parameter> parameter::QueryParameterList(list<token>& tokenList, list<parameter> queryParameterList)
{
	expression queryExpression;
	string parameterName;
	dataType parameterType;

	checkComment(tokenList);

	switch (tokenList.front().getType())
	{
	case LEFT_PAREN:
		match(tokenList, LEFT_PAREN);
		queryParameterList.push_back(queryExpression.makeExpression(tokenList));//<be sure to check in here if the next tokens are comma or right paren.
		return QueryParameterList(tokenList, queryParameterList);
		/*match(tokenList, LEFT_PAREN);
		queryParameterList.push_back(queryExpression.makeExpression(tokenList));
		return queryParameters(tokenList, queryParameterList);
		*/
	case STRING:
		queryParameterList.push_back(queryParameter(tokenList, sTRING));//<be sure to check in here if the next tokens are comma or right paren.
		/*parameterName = tokenList.front().getLiteralValue();
		match(tokenList, tokenType::STRING);
		parameterType = dataType::STRING;
		break;
		*/
		break;
	case ID:
		queryParameterList.push_back(queryParameter(tokenList, iD));//<be sure to check in here if the next tokens are comma or right paren.
		/*parameterName = tokenList.front().getLiteralValue();
		match(tokenList, tokenType::ID);
		parameterType = dataType::ID;
		break;
		*/
		break;
	case COMMA:
		checkComma(tokenList);
		return QueryParameterList(tokenList, queryParameterList);

	case RIGHT_PAREN:
		match(tokenList, RIGHT_PAREN);
		checkComment(tokenList);
		match(tokenList, Q_MARK);
		checkComment(tokenList);
		return queryParameterList;

	default: error(tokenList.front());
	}

	//parameter queryParameter(parameterName, parameterType);
	//queryParameterList.push_back(queryParameter);

	return QueryParameterList(tokenList, queryParameterList);
}

parameter parameter::queryParameter(list<token>& tokenList, dataType typeOfParameter)
{
	string parameterValue;
	dataType parameterType;

	switch (typeOfParameter)
	{
	case iD:
		parameterValue = tokenList.front().getLiteralValue();
		parameterType = iD;
		match(tokenList, ID);
		checkComment(tokenList);
		break;

	case sTRING:
		parameterValue = tokenList.front().getLiteralValue();
		parameterType = sTRING;
		match(tokenList, STRING);
		checkComment(tokenList);
		break;

	default:error(tokenList.front());
	}

	checkComment(tokenList);

	if (tokenList.front().getType() != COMMA && tokenList.front().getType() != RIGHT_PAREN)
	{
		error(tokenList.front());
	}

	parameter tempParameter(parameterValue, parameterType);
	return tempParameter;
}


list<parameter> parameter::ruleList(list<token>& tokenList, list<parameter> ruleParameterList)
{////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//on first parameter of predicate
	expression ruleExpression;
	string parameterName;
	dataType parameterType;

	checkComment(tokenList);

	switch (tokenList.front().getType())
	{
	case LEFT_PAREN:
		match(tokenList, LEFT_PAREN);
		ruleParameterList.push_back(ruleExpression.makeExpression(tokenList));//<be sure to check in here if the next tokens are comma or right paren.
		return ruleList(tokenList, ruleParameterList);

	case STRING:
		ruleParameterList.push_back(queryParameter(tokenList, sTRING));//<be sure to check in here if the next tokens are comma or right paren.
		break;
	case ID:
		ruleParameterList.push_back(queryParameter(tokenList, iD));//<be sure to check in here if the next tokens are comma or right paren.
		break;

	case COMMA:
		checkComma(tokenList);
		return /*QueryParameterList*/ruleList(tokenList, ruleParameterList);

	case RIGHT_PAREN:
		match(tokenList, RIGHT_PAREN);
		checkComment(tokenList);
		return ruleParameterList;

	default: error(tokenList.front());
	}

	return ruleList(tokenList, ruleParameterList);
}

void parameter::checkComma(list<token>& tokenList)
{
	match(tokenList, COMMA);

	checkComment(tokenList);

	if (tokenList.front().getType() != ID && tokenList.front().getType() != STRING && tokenList.front().getType() != LEFT_PAREN)
	{
		error(tokenList.front());
	}

	checkComment(tokenList);
}

void parameter::checkExpressionEnd(list<token>& tokenList)
{
	checkComment(tokenList);

	if (tokenList.front().getType() != COMMA && tokenList.front().getType() != RIGHT_PAREN && tokenList.front().getType() != ADD && tokenList.front().getType() != MULTIPLY)
	{
		error(tokenList.front());
	}
}

void parameter::checkExpressionParameter(list<token>& tokenList)
{
	switch (tokenList.front().getType())
	{
	case ID:
	case STRING:
		return;
	default: error(tokenList.front());
	}
}

void parameter::checkparameterlength(list<token>& tokenList)
{
	switch (tokenList.front().getType())
	{
	case ID:
	case STRING:
	case LEFT_PAREN:
		return;
	default: error(tokenList.front());
	}
}


//---------------------------------------Expression Functions Below----------------------------

expression::expression()
{
}

expression::expression(string expressionString):parameter(expressionString, eXPRESSION)
{
}

expression::~expression()
{
}

/*string expression::getParameterValue()
{
	return parameter::getParameterValue();
	//return expression(leftParameter, operatorType, rightParameter);
}*/

expression expression::makeExpression(list<token>& tokenList)
{
	stringstream expressionString;

	expressionString << "(";//////////////

	expression tempExpression;

	//validateExpression(tokenList);

	//while (tokenList.front().getType() != RIGHT_PAREN)
	//{
		if (tokenList.front().getType() == LEFT_PAREN)
		{
			checkComment(tokenList);
			match(tokenList, LEFT_PAREN);
			checkComment(tokenList);

			//expressionString << "(";

			expression tempExpression2 = tempExpression.makeExpression(tokenList);

			expressionString << tempExpression2.getParameterValue();
		}

		else
		{
			checkExpressionParameter(tokenList);
			expressionString << tokenList.front().getLiteralValue();
			tokenList.pop_front();
		}

		checkComment(tokenList);

		Operator(tokenList);
		expressionString << tokenList.front().getLiteralValue();
		tokenList.pop_front();

		checkComment(tokenList);

		if (tokenList.front().getType() == LEFT_PAREN)
		{
			checkComment(tokenList);
			match(tokenList, LEFT_PAREN);
			checkComment(tokenList);

			//expressionString << "(";

			expression tempExpression2 = tempExpression.makeExpression(tokenList);

			expressionString << tempExpression2.getParameterValue();
		}

		else
		{
			checkExpressionParameter(tokenList);
			expressionString << tokenList.front().getLiteralValue();
			tokenList.pop_front();
		}
	//}
		checkComment(tokenList);
		match(tokenList, RIGHT_PAREN);
		expressionString << ")";
		checkComment(tokenList);
		checkExpressionEnd(tokenList);

	return expression(expressionString.str());
}