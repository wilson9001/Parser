#ifndef GRAPH_H_
#define GRAPH_H_

#include "rule.h"
#include <vector>
#include <set>
#include <map>
#include <stack>
#include "node.h"
#include <queue>
#include <sstream>

using namespace std;

/*This class supports the rule optimization algorithm. The graph can:
*Compute post-order traversal numbers from a given starting node
*Report all nodes reachable from a depth-first search started on a given node
*Reverse all the edges in the graph
*Compute the set of all strongly connected components*/
class graph
{
public:
	graph();
	~graph();

	//*basically a loop to run DFS on every node in the graph.
	void DFSForest();
	//^ Going to have to create a similar function for SCC's which will call find SCC (similar to DFS) and run rule evaluation on each component it returns.

	//This is similar to DFSForest; it will run makeComponent in a loop, each time checking if there is a new component by evaluating the size of the set afterwards. If the set is empty, then it skips to the next iteration. Check before each call of create component to see if next node has already been visited in order to distinguish between trivial components and no component.<--Will not work because rules are in interpreter. Need to return all components.
	//Push component onto stack of components each iteration, return stack.
	void createComponents();

	queue<set<int>> getComponents() { return componentQueue; }

	//*Checks to see if node has been visited, if not then it does a depth first-search through the node dependencies and marks them visited, affixing a post order number on the way out or pushing them on a stack.
	//for each node dependency: if not visited, DFS, then push on stack/assign number and return.
	void DFS(int startingNode);
	//^Create similar function for find SCC, which will return the component so that the rules can be evaluated it in.
	
	void findSCC(int startingNode);
	//Similar to DFS, except populates the component member variable. Rule evaluations should be done on component after each call of this, as this will clear the set to repopulate it.
	void makeComponent(int startingNode);
	//vector<rule> reportReachableNodes(rule startingNode);

	void populateGraph(size_t nodesToMake);
	//void reverseEdges(graph forwardGraph);

	//*Used to add a dependency to a rule.
	//*If building the reverse graph, simply swap the inputs when you call the function.
	//@from (int): the node that is having a dependency added to it.
	//@to (int): the node that is being added as a dependency to another node/rule.
	void addEdge(size_t from, size_t to);

	//*Should only be used by reverse graph to give to forward graph.
	stack<int> getPostOrderStack() { return postOrderStack; }

	//*Should only be used in forward graph to receive post order from reverse graph.
	void setPostOrderStack(stack<int> &postOrderStack) { this->postOrderStack = postOrderStack; }

	bool selfDependent(set<int> &nodeToTest);

	string createGraphPrint();

private:

	//*This map actually holds the rules/nodes of the graph.
	//*The int key corresponds to the number of the rule (which is represented in the node).
	map<int, node> nodes;
	//Used in place of post-order numbers. Filled in reverse graph, emptied in forward graph to create components.
	stack<int> postOrderStack;
	//Used to store each component as it's created, then evaluated.
	set<int> component;

	//Used in forward graphs to hold created components.
	queue<set<int>> componentQueue;

	stringstream graphPrintout;
};

#endif