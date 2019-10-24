#pragma once

#include <vector>
#include <utility>
#include <iostream>

#include "State.h"

enum class Algorithms
{
	Random,
	BKT,
	IDDFS,
	AStar
};

class Algorithm
{
protected:
	std::vector<State*> path;
	std::vector<State*> visited;
	std::vector<State*> toVisit;

	int transitionsCount = 0; // a call on Search function = 1 transition

	bool maxDepthReached = false; // IDDFS sanity check (no path to solution)

	bool targetReached = false;
public:
	virtual void FindSolution(State* source);	
	virtual State* Pick(State* source, std::vector<State*>* neighbors) = 0;
	virtual bool Search(State* source, int limit = -1);
	virtual void PrintSolutionPath();
	int GetTransitionsCount();
	bool TargetHasBeenReached();
};
