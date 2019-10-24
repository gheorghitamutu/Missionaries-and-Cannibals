#pragma once

#include "Algorithm.h"

class AStar : public Algorithm
{
public:
	/* Filtered neighbors. */
	State* Pick(State* source, std::vector<State*>* neighbors) override;
};

