#pragma once

#include "Algorithm.h"

class BKT : public Algorithm
{
public:
	State* Pick(State* source, std::vector<State*>* neighbors) override;
};

