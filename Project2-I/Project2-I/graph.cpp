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

void graph::createComponents()
{
	//create component and push on stack repeatedly
	while(!postOrderStack.empty())
	{
		if (nodes[postOrderStack.top()].checkVisited())
		{
			postOrderStack.pop();
			return;
		}

		makeComponent(postOrderStack.top());
		componentQueue.push(component);
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

void graph::makeComponent(int startingNode)
{
	component.clear();

	if (nodes[startingNode].checkVisited())
	{
		return;
	}

	nodes[startingNode].markVisited();

	set<int> nodeDependencies = nodes[startingNode].getDependencies();
	//for (int i = 0; i < nodeDependencies.size(); i++)
	for (int x : nodeDependencies)
	{
		DFS(x);
	}
	component.insert(startingNode);
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

bool graph::selfDependent(set<int> &componentToTest)
{
	//auto x = componentToTest.begin();

	if (nodes[*componentToTest.begin()].isDependency(*componentToTest.begin()))
	{
		return true;
	}

	return false;
}

string graph::createGraphPrint()
{
	int ruleNumber = 0;

	graphPrintout << "Dependency Graph";

	for (auto x : nodes)
	{
		graphPrintout << "\nR" << ruleNumber++ << ":";

		set<int> dependencies = x.second.getDependencies();
		
		for(auto y = dependencies.begin(); y != dependencies.end(); y++)
		{
			graphPrintout << "R" << *y;
			if (++y != dependencies.end())
			{
				graphPrintout << ",";
			}
			--y;
		}
		graphPrintout << endl;
	}

	return graphPrintout.str();
}
