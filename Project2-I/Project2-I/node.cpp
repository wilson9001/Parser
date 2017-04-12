#include "node.h"

node::node()
{
	visited = false;
	passes = 0;
}


node::~node()
{
}

bool node::isDependency(int dependencyToSeek)
{
	bool isFound;

	dependencyEdges.find(dependencyToSeek) == dependencyEdges.end() ? isFound = false: isFound = true;

	return isFound;
}
