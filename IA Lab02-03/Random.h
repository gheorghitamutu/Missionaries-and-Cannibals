#pragma once

#include <random>

#include "Algorithm.h"

class Random : public Algorithm
{
private:
	std::random_device generator;

public:
	State* Pick(State* source, std::vector<State*>* neighbors) override;
};

