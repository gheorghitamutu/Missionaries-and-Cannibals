#pragma once

#include "BKT.h"

class IDDFS : public BKT
{
private:
	int level = -1;		
	int maxDepth = 2000;
public:
	/* Initial call for IDDFS algorithm. Overwrites base class Search(). */
	bool Search(State* source, int limit = -1) override;

	/* Need some extra stuff here. */
	void PrintSolutionPath() override;
};

