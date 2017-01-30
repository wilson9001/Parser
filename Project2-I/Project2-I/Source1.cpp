#include <iostream>
#include <vector>
#include "scanner.h"

using namespace std;

int main(/*int size , char* argv[]*/)
{//							argv[1]

	string filename = "test1.txt";

	scanner tokenScanner(filename);

	tokenScanner.scan();

	//

	tokenScanner.printTokens();

	system("pause");

	return 0;
}