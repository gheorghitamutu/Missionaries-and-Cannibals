#include "Algorithm.h"

void Algorithm::FindSolution(State* source)
{
	path.push_back(source);
	Search(source);
}

bool Algorithm::Search(State* source, int limit)
{
	//std::cout << "Algorithm::Search Limit: " << limit << std::endl;

	transitionsCount++;

	if (source->IsFinal())	return true;

	if (source->IsLost())	return false;
		
	if (limit == 0)	maxDepthReached = true;
	
	if (maxDepthReached) return false;	

	auto neighbors = source->GetNextPossibleStates();
	if (neighbors.size() == 0)
	{
		source->GenerateNextStates();
		neighbors = source->GetNextPossibleStates();
	}

	std::vector<State*> validNeighbors;
	validNeighbors.reserve(neighbors.size());

	for (const auto& node : neighbors)
	{
		bool found = false;

		for (const auto& n : path) if ((found = (*n == *node)))	break;	

		if (found) continue;
		
		for (const auto& n : validNeighbors) if ((found = (*n == *node))) break;			

		if (found) continue;

		for (const auto& n : visited) if ((found = (*n == *node))) break;		

		if (found) continue;

		validNeighbors.emplace_back(node);
	}

	//std::cout << "Next valid neighbors for: ";
	//source->Print();
	//std::cout << std::endl;
	//
	//for (const auto& nb : validNeighbors)
	//{
	//	nb->Print();
	//	std::cout << std::endl;
	//
	//	if (nb->bp == source->bp)
	//	{
	//		std::cout << "WTF" << std::endl;
	//	}
	//}

	Sort(source, &validNeighbors);

	for (const auto& neighbor : validNeighbors)
	{				
		//std::cout << "For: "; source->Print(); std::cout << " -> ";
		//std::cout << "Picked: "; neighbor->Print(); std::cout << std::endl;

		path.push_back(neighbor);

		const int nextLevel = limit > -1 ? limit - 1 : -1;
		targetReached = Algorithm::Search(neighbor, nextLevel);
		
		if (targetReached) return true;

		visited.push_back(neighbor);
		path.pop_back();
	}

	return false;
}

void Algorithm::PrintSolutionPath()
{
	if (targetReached)
	{
		for (const auto& state : path)
		{
			state->Print();
			std::cout << " -> ";
		}

		std::cout << "Cost: [" << transitionsCount << "]!" << std::endl;
	}
	else
	{
		std::cout << "NONE -> Cost: [" << transitionsCount << "]!" << std::endl;
	}
}

int Algorithm::GetTransitionsCount()
{
	return transitionsCount;
}

bool Algorithm::TargetHasBeenReached()
{
	return targetReached;
}
