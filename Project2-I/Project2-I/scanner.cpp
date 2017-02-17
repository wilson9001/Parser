#include "scanner.h"
#include <fstream>
#include <ctype.h>
#include <iostream>
#include <map>
#include <sstream>
#include <cctype>

using namespace std;

typedef map<tokenType, string> tokenMap;

scanner::scanner(string filename)
{
	this->filename = filename;
	lineNumber = 1;
	startLine = 1;
	isID = false;
	word.str("");
	endOfFile = false;
	isUndefined = false;
	fileOpened = false;
}

scanner::~scanner()
{
}

void scanner::scan()
{
	fileStream.open(filename);

	if (fileStream.is_open())
	{
		fileOpened = true;

		tokenType nextToken = determineToken();

		while (nextToken != MYEOF)
		{
			token test(nextToken, word.str(), startLine);
			//
			if(test.getType() != COMMENT)
			{
				tokenList.push_back(test);
			}
			//tokenList.push_back(test);
			nextToken = determineToken();
		}

		startLine = lineNumber;
		token test(nextToken, word.str(), startLine);
		tokenList.push_back(test);

		fileStream.close();
	}

	else
	{
		cout << "File failed to open!\n";
	}
}

void scanner::skipSpace()
{
	char inputChar;

	fileStream.get(inputChar);

	if (fileStream.eof() || endOfFile)
	{
		endOfFile = true;
		return;
	}
	//check for spaces and advance over them in file.
	while (isspace(inputChar))
	{
		track_line_number(inputChar);
		fileStream.get(inputChar);
		if (fileStream.eof())
		{
			endOfFile = true;
			return;
		}
	}
	startLine = lineNumber;
	fileStream.unget();
	return;
}

void scanner::printTokens()
{
	if (!fileOpened)
	{
		return;
	}

	tokenMap DEEZMAPS;

	DEEZMAPS = { { COMMA, "COMMA" },{ PERIOD ,"PERIOD" },{ Q_MARK,"Q_MARK" },{ LEFT_PAREN,"LEFT_PAREN" },{ RIGHT_PAREN,"RIGHT_PAREN" },{ COLON, "COLON" },{ COLON_DASH,"COLON_DASH" },{ MULTIPLY,"MULTIPLY" },{ ADD,"ADD" },{ SCHEMES,"SCHEMES" },{ FACTS,"FACTS" },{ RULES,"RULES" },{ QUERIES,"QUERIES" },{ ID,"ID" },{ STRING,"STRING" },{ COMMENT,"COMMENT" },{ WHITESPACE,"WHITESPACE" },{ UNDEFINED,"UNDEFINED" },{ MYEOF,"EOF" } };

	for (auto iterator : tokenList)
	{
		cout << "(" << DEEZMAPS[iterator.getType()] << ",\"" << iterator.getLiteralValue() << "\"," << iterator.getLineNumber() << ")" << endl;
	}

	cout << "Total Tokens = " << tokenList.size();
}

tokenType scanner::determineToken()
{
	skipSpace();

	if (endOfFile)
	{
		word.str("");
		word.clear();
		return MYEOF;
	}

	word.str("");
	word.clear();

	/*stringstream word;

	char nextChar;
	fileStream.get(nextChar);
	while (!isspace(nextChar))
	{
	word << nextChar;
	fileStream.get(nextChar);
	}
	*/

	char nextChar;

	fileStream.get(nextChar);

	if (isdigit(nextChar))
	{
		word << nextChar;
		return UNDEFINED;
	}

	switch (nextChar)
	{
	case ',': word << nextChar; return COMMA;
	case '.': word << nextChar; return PERIOD;
	case '?': word << nextChar; return Q_MARK;
	case '(': word << nextChar; return LEFT_PAREN;
	case ')': word << nextChar; return RIGHT_PAREN;
	case '*': word << nextChar; return MULTIPLY;
	case '+': word << nextChar; return ADD;
	case EOF: word << nextChar; return MYEOF;
	default:
		fileStream.unget();
		return otherTokens();
		//call function (make first) that checks for other kinds of tokens
		//return other_function() --returns tokenType
	}
	//should never get here.
}

tokenType scanner::otherTokens()
{
	word.str("");
	word.clear();

	if (fileStream.peek() == ':')
	{
		char nextChar;
		fileStream.get(nextChar);
		word << nextChar;

		if (fileStream.peek() == '-')
		{
			fileStream.get(nextChar);
			word << nextChar;
			return COLON_DASH;
		}

		else
		{
			return COLON;
		}
	}
	//tokens to detect are: colon-dash, colon, schemes, facts, rules, queries, id, string, comment, undefined?
	//need to make detect token function();
	else
	{
		return detectToken();
	}
}

tokenType scanner::detectToken()
{
	word.str("");
	word.clear();
	char nextChar = fileStream.peek();
	switch (nextChar)
	{
	case '\'': getString();
		if (!isUndefined) { return STRING; }
		else { return UNDEFINED; }
	case '#': getComment();
		if (!isUndefined) { return COMMENT; }
		else { return UNDEFINED; }
	}

	if (isalpha(nextChar))
	{
		return getID_or_keyword();
	}

	fileStream.get(nextChar);
	word << nextChar;
	return UNDEFINED;
}

void scanner::getComment()
{
	word.str("");
	word.clear();

	char nextChar;
	fileStream.get(nextChar);

	//begin isCommentBlock()
	/*
	if (fileStream.peek() == '|')
	{
	while ((nextChar != '|') || ((nextChar == '|') && (fileStream.peek() != '#')))
	{//check for end of file
	//previous check was here
	if (fileStream.eof() && nextChar != '|')
	{
	isUndefined = true;
	endOfFile = true;
	return;
	}

	track_line_number(nextChar);
	word << nextChar;

	//added second
	if (fileStream.eof())
	{
	isUndefined = true;
	endOfFile = true;
	return;
	}

	fileStream.get(nextChar);
	}
	word << nextChar;
	fileStream.get(nextChar);
	word << nextChar;
	return;
	}
	*/
	//}
	if (!isCommentBlock(nextChar))
	{
		while (nextChar != '\n')
		{//EOF check?
			word << nextChar;
			fileStream.get(nextChar);
			track_line_number(nextChar);
			if (fileStream.eof())
			{
				endOfFile = true;
				return;
			}
		}
		fileStream.unget();
		lineNumber--;
	}
}

void scanner::getString()
{
	char nextChar;
	fileStream.get(nextChar);
	word << nextChar;
	fileStream.get(nextChar);

	int comma_count = 1;

	while ((nextChar != '\'') || ((nextChar == '\'') && ((fileStream.peek() == '\'') || (comma_count % 2 == 0))))
	{

		if (fileStream.eof())
		{
			fileStream.unget();
			isUndefined = true;
			endOfFile = true;
			return;
		}

		track_line_number(nextChar);

		word << nextChar;

		if (nextChar == '\'')
		{
			comma_count++;
		}

		fileStream.get(nextChar);
	}
	word << nextChar;
}

tokenType scanner::getID_or_keyword()
{
	word.str("");
	word.clear();

	//tokenType checkkeywords(){
	/*isID = false;

	if (is_schemes())
	{
	return SCHEMES;
	}

	//Else get identifier() return ID

	else if (!isID && is_facts())
	{
	return FACTS;
	}

	else if (!isID && is_rules())
	{
	return RULES;
	}

	else if (!isID && is_queries())
	{
	return QUERIES;
	}*/

	// else {return undefined;}}

	tokenType test = checkKeywords();
	if (test == UNDEFINED)
	{
		/*else*/
		//{
		//must be an identifier
		char nextChar;
		fileStream.get(nextChar);

		while (isalnum(nextChar))
		{
			word << nextChar;
			fileStream.get(nextChar);
		}
		fileStream.unget();
		return ID;
	}

	else
	{
		return test;
	}
}

bool scanner::is_schemes()
{
	return S();
}

bool scanner::is_facts()
{
	return F();
}

bool scanner::is_rules()
{
	return R();
}

bool scanner::is_queries()
{
	return Q();
}

bool scanner::F()
{

	if (fileStream.peek() == 'F')
	{
		char nextChar;
		fileStream.get(nextChar);
		word << nextChar;
		isID = true;
		return a();
	}
	//fileStream.unget();
	return false;
}

bool scanner::a()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'a')
	{
		fileStream.get(nextChar);
		word << nextChar;

		return c(nextChar);
	}
	return false;
}

bool scanner::c(char prev)
{
	char nextChar = fileStream.peek();

	if (nextChar != 'c')
	{
		return false;
	}

	else
	{
		fileStream.get(nextChar);
		word << nextChar;
		switch (prev)
		{
		case 'a': return t();
		case 'S': return h();
		}
	}
}

bool scanner::t()
{
	char nextChar = fileStream.peek();

	if (nextChar == 't')
	{
		fileStream.get(nextChar);
		word << nextChar;
		return s();
	}

	else
	{
		return false;
	}
}

bool scanner::s()
{
	char nextChar = fileStream.peek();

	if (nextChar != 's')
	{
		return false;
	}

	else
	{
		fileStream.get(nextChar);
		word << nextChar;

		if (isalnum(fileStream.peek()))
		{
			return false;
		}

		else
		{
			return true;
		}
	}
}

bool scanner::S()
{
	if (fileStream.peek() == 'S')
	{
		char nextChar;
		fileStream.get(nextChar);
		word << nextChar;
		isID = true;
		return c(nextChar);
	}

	return false;
}

bool scanner::h()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'h')
	{
		fileStream.get(nextChar);
		word << nextChar;
		return e(nextChar);
	}

	else
	{
		return false;
	}
}

bool scanner::e(char prev)
{
	char nextChar = fileStream.peek();

	if (nextChar != 'e')
	{
		return false;
	}

	else
	{
		fileStream.get(nextChar);
		word << nextChar;
		switch (prev)
		{
		case 'h': return m();
		case 'u': return r();
		case 'l':
		case 'i':
		case 'm': return s();
		}
	}
}

bool scanner::m()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'm')
	{
		fileStream.get(nextChar);
		word << nextChar;
		return e(nextChar);
	}
	return false;
}

bool scanner::R()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'R')
	{
		fileStream.get(nextChar);
		word << nextChar;
		isID = true;
		return u(nextChar);
	}

	else
	{
		return false;
	}
}

bool scanner::u(char prev)
{
	char nextChar = fileStream.peek();

	if (nextChar != 'u')
	{
		return false;
	}

	else
	{
		fileStream.get(nextChar);
		word << nextChar;

		switch (prev)
		{
		case 'R': return l();
		case 'Q': return e(nextChar);
		}
	}
}

bool scanner::l()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'l')
	{
		fileStream.get(nextChar);
		word << nextChar;

		return e(nextChar);
	}

	else
	{
		return false;
	}
}

bool scanner::Q()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'Q')
	{
		fileStream.get(nextChar);
		word << nextChar;
		isID = true;
		return u(nextChar);
	}

	else
	{
		return false;
	}
}

bool scanner::r()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'r')
	{
		fileStream.get(nextChar);
		word << nextChar;
		return i();
	}

	else
	{
		return false;
	}
}

bool scanner::i()
{
	char nextChar = fileStream.peek();

	if (nextChar == 'i')
	{
		fileStream.get(nextChar);
		word << nextChar;
		return e(nextChar);
	}

	else
	{
		return false;
	}
}

void scanner::track_line_number(char test)
{
	if (test == '\n')
	{
		lineNumber++;
	}
}

bool scanner::isCommentBlock(char passChar)
{
	char nextChar = passChar;

	if (fileStream.peek() == '|')
	{
		while ((nextChar != '|') || ((nextChar == '|') && (fileStream.peek() != '#')))
		{//check for end of file
		 //previous check was here
			if (fileStream.eof() && nextChar != '|')
			{
				isUndefined = true;
				endOfFile = true;
				return true;
			}

			track_line_number(nextChar);
			word << nextChar;

			//added second
			if (fileStream.eof())
			{
				isUndefined = true;
				endOfFile = true;
				return true;
			}

			fileStream.get(nextChar);
		}
		word << nextChar;
		fileStream.get(nextChar);
		word << nextChar;
		return true;
	}

	else
	{
		return false;
	}
}

tokenType scanner::checkKeywords()
{
	//tokenType checkkeywords();

	isID = false;

	if (is_schemes())
	{
		return SCHEMES;
	}

	//Else get identifier() return ID

	else if (!isID && is_facts())
	{
		return FACTS;
	}

	else if (!isID && is_rules())
	{
		return RULES;
	}

	else if (!isID && is_queries())
	{
		return QUERIES;
	}

	else
	{
		return UNDEFINED;
	}
}

list<token> scanner::getTokens()
{
	return tokenList;
}