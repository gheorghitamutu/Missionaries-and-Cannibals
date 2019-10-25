#pragma once

#include "Algorithm.h"

class AStar : public Algorithm
{
public:
	/* Filtered neighbors. */
	void Sort(State* source, std::vector<State*>* neighbors) override;
};

