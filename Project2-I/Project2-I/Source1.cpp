#include <iostream>
#include <vector>
#include "scanner.h"
#include "parser.h"
#include <list>

using namespace std;

int main(/*int size , char* argv[]*/)
{//							argv[1]

	string filename = "test1.txt";

	scanner tokenScanner(filename);

	tokenScanner.scan();

	parser tokenParser(tokenScanner.getTokens());

	tokenParser.parse();

	tokenScanner.printTokens();

	system("pause");

	return 0;
}