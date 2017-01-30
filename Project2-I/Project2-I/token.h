//#pragma once
#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

using namespace std;

enum tokenType { COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, MYEOF };

class token
{
public:
	token(tokenType type, string literalValue, unsigned int lineNumber);
	~token();

	tokenType getType() { return type; }

	string getLiteralValue() { return literalValue; }

	unsigned int getLineNumber() { return lineNumber; }

private:
	tokenType type;
	string literalValue;
	unsigned int lineNumber;
};

#endif