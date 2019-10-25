#pragma once

#include "Algorithm.h"

class BKT : public Algorithm
{
public:
	void Sort(State* source, std::vector<State*>* neighbors) override;
};

