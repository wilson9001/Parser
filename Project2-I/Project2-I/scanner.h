//#pragma once

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>
#include <sstream>
#include "token.h"
#include <list>
#include <fstream>

using namespace std;

class scanner
{
public:
	scanner(string filename);
	~scanner();

	void scan();

	void skipSpace();

	void printTokens();

	tokenType determineToken();

	tokenType otherTokens();

	tokenType detectToken();

	void getComment();

	void getString();

	tokenType getID_or_keyword();

	bool is_schemes();

	bool is_facts();

	bool is_rules();

	bool is_queries();

	bool F();

	bool a();

	bool c(char prev);

	bool t();

	bool s();

	bool S();

	bool h();

	bool e(char prev);

	bool m();

	bool R();

	bool u(char prev);

	bool l();

	bool Q();

	bool r();

	bool i();

	void track_line_number(char test);

	bool isCommentBlock(char passChar);

	tokenType checkKeywords();

private:

	string filename;

	bool endOfFile;

	bool isUndefined;

	bool fileOpened;

	ifstream fileStream;

	stringstream word;

	size_t lineNumber;

	size_t startLine;

	list<token> tokenList;

	bool isID;
};

#endif