#include "graph.h"

graph::graph()
{
	cout << "Warning! Default graph constructor is not usable!";
}

graph::~graph()
{
}

void graph::DFSForest()
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		DFS(i);
	}
}

void graph::DFS(int startingNode)
{
	if (nodes[startingNode].checkVisited())
	{
		return;
	}

	nodes[startingNode].markVisited();

	set<int> nodeDependencies = nodes[startingNode].getDependencies();
	//for (int i = 0; i < nodeDependencies.size(); i++)
	for(int x : nodeDependencies)
	{
		DFS(x);
	}
	postOrderStack.push(startingNode);
}

void graph::populateGraph(size_t nodesToMake)
{
	for (size_t i = 0; i < nodesToMake; i++)
	{
		nodes.insert(pair<int, node>(i, node()));
	}
}

void graph::addEdge(size_t from,size_t to)
{
	nodes[from].addDependency(to);
}
