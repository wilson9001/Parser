#ifndef TUPLE_H_
#define TUPLE_H_

#include <vector>

using namespace std;

class Tuple :
	public vector<string>
{
public:
	Tuple();
	~Tuple();
};

#endif