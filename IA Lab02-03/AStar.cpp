#include "AStar.h"

/*
	https://stackoverflow.com/questions/15179481/how-to-calculate-distance-between-2-points-in-a-2d-matrix
	https://uk.mathworks.com/matlabcentral/answers/413437-how-calculate-distance-between-points-in-matrices
*/
void AStar::Sort(State* source, std::vector<State*>* neighbors)
{
	/*
		sqrt((x2 - x1) ^ 2 + (y2 - y1) ^ 2);
		where x1 = 0 missionaries and y1 = 0 cannibals
		also called max distance from *initial* source to target
			   			
		Tratam si cazul cand flip matricea pentru ca suntem pe malul drept.
		Pe o pereche 30|30|4 reduce de la 61 tranzitii la 60.			
	*/

	const double maxDist =
		std::sqrt((double)source->nm * source->nm + (double)source->nc * source->nc);

	//std::cout << "AStar::Sort Elems: " << neighbors->size() << std::endl;
	std::sort(neighbors->begin(), neighbors->end(),
		[=](const State* a, const State* b) -> bool {

			//std::cout << "sort" << std::endl;

			//if (source->mmal1 != source->cmal1)
			//{
			//	return (a->mmal1 + a->cmal1) / a->bc <
			//		(b->mmal1 + b->cmal1) / b->bc;
			//}

			int aMaxMis =
				std::min(source->bc, std::max(a->GetMissionariesFromCurrentShore(), aMaxMis));
			int aMaxCan =
				std::min(source->bc, std::max(a->GetCannibalsFromCurrentShore(), aMaxCan));
			
			const double ax1 = aMaxMis;
			const double ay1 = aMaxCan;
			const double ax2 = source->nm;
			const double ay2 = source->nc;
			
			const double aDist = ((source->bp == BoatPosition::LEFT) ?
				std::sqrt((ax2 - ax1) * (ax2 - ax1) + (ay2 - ay1) * (ay2 - ay1)) :
				maxDist - std::sqrt((ax2 - ax1) * (ax2 - ax1) + (ay2 - ay1) * (ay2 - ay1)));
			
			int bMaxMis =
				std::min(source->bc, std::max(b->GetMissionariesFromCurrentShore(), bMaxMis));
			int bMaxCan =
				std::min(source->bc, std::max(b->GetCannibalsFromCurrentShore(), bMaxCan));
			
			const double bx1 = bMaxMis;
			const double by1 = bMaxCan;
			const double bx2 = source->nm;
			const double by2 = source->nc;
			
			const double bDist = ((source->bp == BoatPosition::LEFT) ?
				std::sqrt((bx2 - bx1) * (bx2 - bx1) + (by2 - by1) * (by2 - by1)) :
				maxDist - std::sqrt((bx2 - bx1) * (bx2 - bx1) + (by2 - by1) * (by2 - by1)));
			
			return aDist < bDist;
		}
	);
}
