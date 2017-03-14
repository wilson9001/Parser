#ifndef TUPLE_H_
#define TUPLE_H_

#include <vector>

using namespace std;

//*Vector of strings used to hold instantiated schemes.
class Tuple :
	public vector<string>
{
public:
	Tuple();
	~Tuple();
};

#endif