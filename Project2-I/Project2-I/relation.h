#ifndef RELATION_H_
#define RELATION_H_

#include "scheme.h"
#include <set>
#include "Tuple.h"
#include <list>
#include "parameter.h"

class relation
{
public:
	relation(string relationName, list<parameter> parameterList);
	~relation();

private:
	//*Name of this relation
	//*Will correspond to name of facts that will become tuples
	string relationName;
	//*Blueprint of tuples that will populate this relation
	scheme relationScheme;
	//*Actual instantiations of the relation outlined in the scheme
	set<Tuple> relationTuples;
};

#endif