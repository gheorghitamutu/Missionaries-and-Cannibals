#pragma once

#include <algorithm>
#include <random>

#include "Algorithm.h"

class Random : public Algorithm
{
private:
	std::default_random_engine generator{};

public:
	void Sort(State* source, std::vector<State*>* neighbors) override;
};

