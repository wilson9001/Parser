#include "token.h"

token::token(tokenType type, string literalValue, unsigned int lineNumber)
{
	this->type = type;
	this->literalValue = literalValue;
	this->lineNumber = lineNumber;
}


token::~token()
{
}
