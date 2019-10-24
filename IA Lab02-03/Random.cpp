#include "Random.h"

State* Random::Pick(State* source, std::vector<State*>* neighbors)
{
	std::uniform_int_distribution<int> distribution(0, neighbors->size() - 1);

	int offset = distribution(generator);

	return neighbors->at(offset);
}
