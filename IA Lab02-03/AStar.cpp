#include "AStar.h"

/*
	https://stackoverflow.com/questions/15179481/how-to-calculate-distance-between-2-points-in-a-2d-matrix
	https://uk.mathworks.com/matlabcentral/answers/413437-how-calculate-distance-between-points-in-matrices
*/
State* AStar::Pick(State* source, std::vector<State*>* neighbors)
{
	if (neighbors->size() == 0)
	{
		throw "Invalid call on A* Pick()";
	}

	int maxMissionaries = 0;
	int maxCannibals = 0;
	   
	/*
		sqrt((x2 - x1) ^ 2 + (y2 - y1) ^ 2);
		where x1 = 0 missionaries and y1 = 0 cannibals
		also called max distance from *initial* source to target
	*/
	const double maxDist =
		std::sqrt((double)source->nm * source->nm + (double)source->nc * source->nc);

	double dist = maxDist;

	State* closestState = nullptr;

	for (const auto& child : *neighbors)
	{
		maxMissionaries = 
			std::max(child->GetMissionariesFromCurrentShore(), maxMissionaries);
		maxCannibals = 
			std::max(child->GetCannibalsFromCurrentShore(), maxCannibals);
			   
		maxMissionaries = std::min(source->bc, maxMissionaries);
		maxCannibals = std::min(source->bc, maxCannibals);
			   
		const double x1 = maxMissionaries;
		const double y1 = maxCannibals;
		const double x2 = source->nm;
		const double y2 = source->nc;

		/*
			Tratam si cazul cand flip matricea pentru ca suntem pe malul drept.
			Pe o pereche 30|30|4 reduce de la 61 tranzitii la 60.
		*/
		const double tempDist = ((source->bp == BoatPosition::LEFT) ?
			std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) :
			maxDist - std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

		if (tempDist <= dist)
		{
			dist = tempDist;
			closestState = child;
		}
	}

	if (closestState == nullptr)
	{
		throw "A* next state not picked!";
	}

	return closestState;
}
