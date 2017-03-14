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

	void addTuple(Tuple newTuple) { relationTuples.insert(newTuple); }

	//*Used on internal data in temp relations to answer queries.
	//*Needs to redo set of tuples to only have ones that qualify with this query.
	relation selectAbsolute(string AbsoluteString, int positionInTuple);
	//*Used on internal data in temp relations to answer queries.
	//*May only add to project vector if ID is new. If it isn't new, then it will select
	//for where the column numbers with this ID equal each other.
	relation selectID(string IdString, int positionInTuple);

	//*Will take existing vector of columns to project and reduce tuple vectors down to the appropriate indexes/columns.
	relation project();

	//*Will take vector of projectNames and rename the still existing columns after project to be the appropriate name.
	void rename();

private:
	//*Name of this relation
	//*Will correspond to name of facts that will become tuples
	string relationName;
	//*Blueprint of tuples that will populate this relation
	scheme relationScheme;
	//*Actual instantiations of the relation outlined in the scheme
	set<Tuple> relationTuples;
	//*Used in temporary relations to answer queries. Used to rename relationScheme.
	vector<string> projectNames;
	//*Used in temporary relations to answer queries. Used to reduce Tuples down to specified indexes corresponding with specified columns.
	vector<int> columnsToProject;
};

#endif