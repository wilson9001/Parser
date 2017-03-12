#include "parser.h"
#include "token.h"
#include "scanner.h"
#include <iostream>
#include <map>

typedef map<tokenType, string> tokenMap;

using namespace std;
using std::cout;

parser::parser(list<token> parsingTokens):parsingTokens(parsingTokens)
{
}

parser::~parser()
{
}

void parser::parse()
{


	try
	{
		while (parsingTokens.front().getType() != MYEOF)
		{
			makeDatalogProgram();
		}
	}

	catch(token badToken)
	{
		tokenMap DEEZMAPS;

		DEEZMAPS = { { COMMA, "COMMA" },{ PERIOD ,"PERIOD" },{ Q_MARK,"Q_MARK" },{ LEFT_PAREN,"LEFT_PAREN" },{ RIGHT_PAREN,"RIGHT_PAREN" },{ COLON, "COLON" },{ COLON_DASH,"COLON_DASH" },{ MULTIPLY,"MULTIPLY" },{ ADD,"ADD" },{ SCHEMES,"SCHEMES" },{ FACTS,"FACTS" },{ RULES,"RULES" },{ QUERIES,"QUERIES" },{ ID,"ID" },{ STRING,"STRING" },{ COMMENT,"COMMENT" },{ WHITESPACE,"WHITESPACE" },{ UNDEFINED,"UNDEFINED" },{ MYEOF,"EOF" } };

		cout << "Failure!\n  (" << DEEZMAPS[badToken.getType()] << ",\"" << badToken.getLiteralValue() << "\"," << badToken.getLineNumber() << ")";

		return;
	}

	//printDatalogProgram();
	//wrap datalogProgram in try-catch block.
}

//*Compares the current token's tokentype to the tokentype passed in.
//*If they are a match, then the token is advanced to the next token in the list.
//*Else the token is thrown.
void parser::match(tokenType testTokenType)
{
	parsingTokens.front().getType() == testTokenType ? parsingTokens.pop_front() : error(parsingTokens.front());
}

//*This function represents the start position for all grammars.
//*If the next token is not FACTS, RULES, SCHEMES, QUERIES, then the token is thrown.
void parser::makeDatalogProgram()
{
	predicate myPredicate;
	rule myRule;

	//check for comments

	checkComment(parsingTokens);

	myProgram.addSchemes(myPredicate.makePredicate(parsingTokens));

	checkComment(parsingTokens);

	myProgram.addFacts(myPredicate.makePredicate(parsingTokens));

	checkComment(parsingTokens);

	myProgram.addRules(myRule.ruleList(parsingTokens));

	checkComment(parsingTokens);

	myProgram.addQueries(myPredicate.makePredicate(parsingTokens));

	checkComment(parsingTokens);

	if (parsingTokens.front().getType() != MYEOF)
	{
		error(parsingTokens.front());
	}
	/*
	switch (parsingTokens.front().getType())
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
		parsingTokens.pop_front();
		return;
	default:
		error(parsingTokens.front());
	}
	*/
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

void parser::printDatalogProgram()
{
	list<predicate> predicateList = myProgram.getSchemes();
	cout << "Success!\nSchemes(" << predicateList.size() << "):";

	printSchemes(predicateList);
	//
	/*
	for (auto x : predicateList)
	{
		cout << "\n  " << x.getPredicateName() << "(";
		list<parameter> parameterList = x.getPredicateParameters();

		stringstream parameters;

		for (auto y : parameterList)
		{
			parameters << y.getParameterValue() << ",";
		}

		string parameterString = parameters.str();

		parameterString = parameterString.substr(0, parameterString.length() - 1);

		cout << parameterString << ")";
	}
	*/
	//

	predicateList = myProgram.getFacts();

	printFacts(predicateList);
	//
	/*
	cout << "\nFacts(" << predicateList.size() << "):";

	for (auto x : predicateList)
	{
		cout << "\n  " << x.getPredicateName() << "(";
		list<parameter> parameterList = x.getPredicateParameters();
		//
		stringstream parameters;

		for (auto y : parameterList)
		{
			parameters << y.getParameterValue() << ",";
		}

		string parameterString = parameters.str();

		parameterString = parameterString.substr(0, parameterString.length() - 1);

		cout << parameterString << ").";
		//
	}
	*/
	//
	list<rule> ruleList;
	ruleList = myProgram.getRules();

	cout << "\nRules(" << ruleList.size() << "):";

	for (auto x : ruleList)
	{
		cout << "\n  ";

		list<predicate> rulePredicates = x.getRulePredicates();

		cout << rulePredicates.front().getPredicateName() << "(";

		list<parameter> predicateParameters = rulePredicates.front().getPredicateParameters();

		rulePredicates.pop_front();

		stringstream parameterString;

		for (auto y : predicateParameters)
		{
			parameterString << y.getParameterValue() << ",";
		}

		string parameterstr = parameterString.str();

		parameterstr = parameterstr.substr(0, parameterstr.length() - 1);

		cout << parameterstr << ") :- ";

		stringstream predicateString;

		for (auto y : rulePredicates)
		{
			predicateString << y.getPredicateName() << "(";

			list<parameter> predicateParameters = y.getPredicateParameters();

			predicateString << printRulePredicateParameters(predicateParameters);
			//stringstream parameterString;
			/*
			for (auto z : predicateParameters)
			{
				parameterString << z.getParameterValue() << ",";
			}

			string parameterStr = parameterString.str();

			parameterStr = parameterStr.substr(0, parameterStr.length() - 1);
			*/
			//predicateString << parameterStr << "),";

		}

		string predicatestr = predicateString.str();

		predicatestr = predicatestr.substr(0, predicatestr.length() - 1);

		cout << predicatestr << ".";
	}

	predicateList = myProgram.getQueries();

	cout << "\nQueries(" << predicateList.size() << "):";

	for (auto x : predicateList)
	{
		cout << "\n  " << x.getPredicateName() << "(";

		list<parameter>predicateParameters = x.getPredicateParameters();

		stringstream parameterString;

		for (auto y : predicateParameters)
		{
			parameterString << y.getParameterValue() << ",";
		}

		string parameterStr = parameterString.str();

		parameterStr = parameterStr.substr(0, parameterStr.length() - 1);

		cout << parameterStr << ")?";
	}

	set<string> domainList = myProgram.getDomain();

	cout << "\nDomain(" << domainList.size() << "):";

	for (auto x : domainList)
	{
		cout << "\n  " << x;
	}
}

string parser::printRulePredicateParameters(list<parameter> predicateParameter)
{
	stringstream parameterString;

	for (auto z : predicateParameter)
	{
		parameterString << z.getParameterValue() << ",";
	}

	//return parameterString.str().substr(0, parameterString.str().size() - 1);

	string parameterStr = parameterString.str().substr(0, parameterString.str().size() - 1);

	parameterStr += "),";

	return parameterStr;
	//parameterStr = parameterStr.substr(0, parameterStr.length() - 1);

	//predicateString << parameterStr << "),";

}

void parser::printFacts(list<predicate> predicateList)
{
	cout << "\nFacts(" << predicateList.size() << "):";

	for (auto x : predicateList)
	{
		cout << "\n  " << x.getPredicateName() << "(";
		list<parameter> parameterList = x.getPredicateParameters();

		printFactParameters(parameterList);
		//
		/*
		stringstream parameters;

		for (auto y : parameterList)
		{
			parameters << y.getParameterValue() << ",";
		}

		string parameterString = parameters.str();

		parameterString = parameterString.substr(0, parameterString.length() - 1);

		cout << parameterString << ").";
		*/
	}
}

void parser::printFactParameters(list<parameter> parameterList)
{
	stringstream parameters;

	for (auto y : parameterList)
	{
		parameters << y.getParameterValue() << ",";
	}

	string parameterString = parameters.str();

	parameterString = parameterString.substr(0, parameterString.length() - 1);

	cout << parameterString << ").";
}

void parser::printSchemes(list<predicate> predicateList)
{
	for (auto x : predicateList)
	{
		cout << "\n  " << x.getPredicateName() << "(";
		list<parameter> parameterList = x.getPredicateParameters();

		stringstream parameters;

		for (auto y : parameterList)
		{
			parameters << y.getParameterValue() << ",";
		}

		string parameterString = parameters.str();

		parameterString = parameterString.substr(0, parameterString.length() - 1);

		cout << parameterString << ")";
	}
}

void parser::checkComment(list<token>& tokenList)
{
	while (tokenList.front().getType() == COMMENT)
	{
		tokenList.pop_front();
	}
}

void parser::purgeComments()
{
	list<token>::iterator it;

	for (auto i : parsingTokens)
	{
		//i.getType() == COMMENT ? parsingTokens.erase(parsingTokens.begin() + i, parsingTokens.begin() + i )
	}
}
