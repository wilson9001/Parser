#ifndef NODE_H_
#define NODE_H_

#include <set>

using namespace std;

class node
{
public:
	/*What exactly is a node? How is an edge implemented? What exactly is a component and how is it implemented?*/
	node();
	~node();

	//*Adds an int corresponding to a rule number (or node) that is a dependency for this rule.
	void addDependency(int nodeToAdd) { dependencyEdges.insert(nodeToAdd); }

	//void affixNumber(int postFixNumber) { this->postFixNumber = postFixNumber; }

	void markVisited() { visited = true; }

	bool checkVisited() { return visited; }

	set<int> getDependencies() { return dependencyEdges; }

	//int getPostFixNumber() { return postFixNumber; }

private:
	set<int> dependencyEdges;
	bool visited;
	//int postFixNumber;
};

#endif