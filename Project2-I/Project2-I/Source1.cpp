#include <iostream>
#include <vector>
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include <list>

using namespace std;

int main(/*int size , char* argv[]*/)
{//							argv[1]

	string filename = "otest4.txt";
	//not self dependent size 1
	scanner tokenScanner(filename);

	tokenScanner.scan();

	parser tokenParser(tokenScanner.getTokens());

	tokenParser.parse();

	//tokenScanner.printTokens();

	interpreter myInterpreter;

	datalogProgram myData = tokenParser.getDatalog();
	myInterpreter.createDataBase(myData);

	myInterpreter.evaluateQueries();

	system("pause");

	return 0;
}