#include <iostream>
#include <vector>
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include <list>

using namespace std;

int main(/*int size , char* argv[]*/)
{//							argv[1]

	string filename = "rtest3.txt";

	scanner tokenScanner(filename);

	tokenScanner.scan();

	parser tokenParser(tokenScanner.getTokens());

	tokenParser.parse();

	//tokenScanner.printTokens();

	interpreter myInterpreter;

	myInterpreter.createDataBase(tokenParser.getDatalog());

	myInterpreter.evaluateQueries();

	//system("pause");

	return 0;
}