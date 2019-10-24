#include "Algorithm.h"

void Algorithm::FindSolution(State* source)
{
	//source->cachedStates.clear();
	path.push_back(source);
	Search(source);
}

bool Algorithm::Search(State* source, int limit)
{
	//std::cout << "Algorithm::Search Limit: " << limit << std::endl;

	transitionsCount++;
	if (source->IsFinal()) // is target
	{
		return true;
	}

	if (source->IsLost()) // avoid dead ends
	{
		return false;
	}	
	
	if (limit > -1 && limit == 0) // max depth reached
	{
		maxDepthReached = true;
		return false;
	}

	auto neighbors = source->GetNextPossibleStates();
	if (neighbors.size() == 0)
	{
		source->GenerateNextStates();
		neighbors = source->GetNextPossibleStates();
	}

	std::vector<State*> validNeighbors;
	validNeighbors.reserve(neighbors.size());

	for (auto node : neighbors)
	{
		//if (std::find(path.begin(), path.end(), node) == path.end() &&
		//	std::find(validNeighbors.begin(), validNeighbors.end(), node) == validNeighbors.end() &&
		//	std::find(visited.begin(), visited.end(), node) == visited.end())
		//{

		bool found = false;
		for (const auto& n : path)
		{
			if (*n == *node)
			{
				found = true;
				break;
			}
		}

		if (found == false)
		{
			for (const auto& n : validNeighbors)
			{
				if (*n == *node)
				{
					found = true;
					break;
				}
			}
		}

		if (found == false)
		{
			for (const auto& n : visited)
			{
				if (*n == *node)
				{
					found = true;
					break;
				}
			}
		}

		//if (found == false)
		//{
		//	for (const auto& n : toVisit)
		//	{
		//		if (*n == *node)
		//		{
		//			found = true;
		//			break;
		//		}
		//	}
		//}

		if (found == false)
		{
			if (node->IsLost() == false)
			{
				//if (node->level > source->level)
				//{
					validNeighbors.emplace_back(node);
					toVisit.emplace_back(node);
				//}
			}
		}
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

	while (validNeighbors.empty() == false)
	{
		const auto pickedNeighbor = Pick(source, &validNeighbors);
				
		//std::cout << "For: "; source->Print(); std::cout << " -> ";
		//std::cout << "Picked: "; pickedNeighbor->Print(); std::cout << std::endl;

		path.push_back(pickedNeighbor);
		if (Algorithm::Search(pickedNeighbor, limit > -1 ? limit - 1 : -1))
		{
			targetReached = true;
			return true;
		}

		toVisit.erase(
			std::remove(toVisit.begin(), toVisit.end(), pickedNeighbor),
			toVisit.end());

		visited.push_back(pickedNeighbor);
		path.pop_back();

		validNeighbors.erase(
			std::remove(validNeighbors.begin(), validNeighbors.end(), pickedNeighbor),
			validNeighbors.end());
	}

	return false;
}

void Algorithm::PrintSolutionPath()
{
	for (const auto& state : path)
	{
		state->Print();
		std::cout << " -> ";
	}

	if (path.size() > 1)
	{
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
