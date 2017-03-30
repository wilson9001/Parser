#ifndef RELATION_H_
#define RELATION_H_

#include "scheme.h"
#include <set>
#include "Tuple.h"
#include <list>
#include "parameter.h"
#include "predicate.h"

class relation
{
public:
	relation(string relationName, list<parameter> parameterList);
	relation();
	~relation();

	void addTuple(Tuple &newTuple) { relationTuples.insert(newTuple); }

	//*Used on internal data in temp relations to answer queries.
	//*Needs to redo set of tuples to only have ones that qualify with this query.

	//relation selectAbsolute(string AbsoluteString, int positionInTuple);
	void selectAbsolute(string AbsoluteString, int positionInTuple);
	//*Used on internal data in temp relations to answer queries.
	//*May only add to project vector if ID is new. If it isn't new, then it will select
	//for where the column numbers with this ID equal each other.

	//relation selectID(string IdString, int positionInTuple);
	void selectID(string IdString, int positionInTuple);

	void findSharedColumns(int position1, int position2);

	//*Will take existing vector of columns to project and reduce tuple vectors down to the appropriate indexes/columns.
	//*If the entire query was absolutes, then there should be no numbers in the projection vector, and since there can only be one answer that's K.

	//relation project();
	void project();

	Tuple removeTupleColumns(Tuple &oldTuple);

	//*Will take vector of projectNames and rename the still existing columns after project to be the appropriate name.
	void rename();

	set<Tuple> getTuples() { return relationTuples; }

	scheme getScheme() { return relationScheme; }

	/*void? join(relation1, relation2)
	should make new schema and then make a new relation.
	Then, have a double for-loop 
		for(rel 1 iterate through tuples in relation 1)
			for(rel2 iterate through tuples in relation 2)
				if( joinable(tup1, tup2)
					returns tuple -- join(tup1, tup2)
					addjoin -- adds to new relation

		return new relation
	*/
	//relation1 can be existing tuples in this relation, newScheme is new addition to address rescheming of incoming relationToJoin.
	void join(relation relationToJoin, list<parameter> & newScheme);

	/*NOTES: Rules can have string, supposed to reuse query function?
	ALso, first tuple needs two spaces, all tuples after that need one.*/
	
	
	
	
	/*
	tuple join (not same as other function, this takes tuples instead of relations like the other one)
	
	join tuples t1 and t2
	for each name n2 and value v2 in scheme s2 and tuple t2 if the name n2 is not found in scheme s1 add the value v2 to the new tuple

	this will make sure to eliminate duplicate columns before inserting tuples into set 1 of tuples
	*/
	
	Tuple joinTuple(Tuple &t1, Tuple &t2, vector<pair<int, int>> &matchingIndex);

	/*joinable if tuple names are same but values are not then they are not joinable, iterate through each if scheme names are same if tuples aren't the same then they aren't joinable */
	bool joinable(Tuple &tup1, Tuple &tup2, vector<pair<int, int>> &matchingIndex);


	/*
	void addJoin (tuple) add to new relation
	*/
	void addJoin(relation &newRelation, Tuple &tupleToAdd);

	/*union(rel1, rel2) 
	for tuples rel2
		insert into set of tup1
	*/
	void Union(relation &rel1, relation &rel2);

	//Will need to separate the return pair out for use in the next functions in the join.
	//Scheme1 can be existing scheme of relation object
	vector<pair<int, int>> makeScheme(scheme &scheme2);
	//pair<scheme,

	void reScheme(list<parameter> &scheme);

	void fixColumns(predicate &headPredicate);

	void reorderColumns(vector<size_t> &relevantColumnPosition);

private:
	//*Name of this relation
	//*Will correspond to name of facts that will become tuples
	string relationName;
	//*Blueprint of tuples that will populate this relation
	scheme relationScheme;
	//*Temporary scheme used to reorder scheme when rearranging columns
	scheme tempScheme;
	//*Actual instantiations of the relation outlined in the scheme
	set<Tuple> relationTuples;
	//*temporary storage for tuples that will replace existing tuples when doing a join or reorder function.
	set<Tuple> tempTuples;
	//*Used in temporary relations to answer queries. Used to rename relationScheme.
	vector<string> projectNames;
	//*Used in temporary relations to answer queries. Used to reduce Tuples down to specified indexes corresponding with specified columns.
	vector<int> columnsToProject;
};

#endif