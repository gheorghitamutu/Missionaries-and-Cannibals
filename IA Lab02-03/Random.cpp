#include "Random.h"

void Random::Sort(State* source, std::vector<State*>* neighbors)
{
	std::shuffle(neighbors->begin(), neighbors->end(), generator);
}
