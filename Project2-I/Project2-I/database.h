#ifndef DATABASE_H_
#define DATABASE_H_

#include <map>
#include "relation.h"

using namespace std;

class database
{
public:
	database();
	~database();

private:

	//This map will hold all the relation objects with relation name, relation object.
	map<string, relation> relationMap;
};

#endif