#include "IDDFS.h"

bool IDDFS::Search(State* source, int limit)
{
	//std::cout << "IDDFS::Search Limit: " << limit << std::endl;
	do
	{
		path.clear();
		path.push_back(source);

		visited.clear();

		level++;

		maxDepthReached = false;

		//std::cout << "\rSearching solution on level: " << level << " ... ";

		/*
			Just call base class Search() to do the rest.
		*/
		if (Algorithm::Search(source, level))
		{
			return true;
		}
	} while (maxDepthReached && level < maxDepth);

	return false;
}

void IDDFS::PrintSolutionPath()
{
	if (targetReached == true)
	{
		std::cout << "Solution found on level: " << level << std::endl;
		Algorithm::PrintSolutionPath();
	}
	else
	{
		std::cout << "Explored the entire tree (" << level << " depth levels). ";
		std::cout << "There's no solution!" << std::endl;
	}
}
