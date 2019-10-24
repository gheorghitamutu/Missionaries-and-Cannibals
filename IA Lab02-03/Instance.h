#pragma once

#include "State.h"
#include "Random.h"
#include "IDDFS.h"
#include "BKT.h"
#include "AStar.h"

class Instance
{
public:
	Instance(int nm, int nc, int bc);
	Instance() = delete; // we don't want an instance without initializing its State
	~Instance();

	void SetAlgorithm(Algorithms algUsed);
	void Solve();
	void PrintSolutionPath();
	int GetTransitionsCount();
	bool TargetHasBeenReached();
private:
	State state; // this state should always be valid before usage
	Algorithm* algorithm = nullptr; // you don't actually care which one is it, you just call DoTransition and it does the rest
};

