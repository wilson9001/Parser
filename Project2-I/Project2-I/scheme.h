#ifndef SCHEME_H_
#define SCHEME_H_

#include <vector>
#include <string>

using namespace std;

class scheme :
	public vector<string>
{
public:
	scheme();
	scheme(string schemeName);
	~scheme();

private:
	string schemeName;
};

#endif