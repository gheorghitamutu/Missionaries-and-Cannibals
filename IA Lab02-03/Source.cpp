/*
	Laborator 2: Modelarea unei probleme de decizie
		https://docs.google.com/document/d/111h5QikSMdrSNJmmBke1Nvt1rSR5c2ltvsZSG7uzNi4
	Laborator 3: A* si compararea strategiilor
		https://docs.google.com/document/d/1vCyBEFCA_PyQvm8CZlXqKVPPQTAdzyZ1St5KYFUYR4Q
*/

#include <chrono>	// time
#include <numeric>	// accumulate
#include <iomanip>	// std::cout padding

#include "Instance.h"

int main()
{
	/* 
		missionaries and cannibals -> [3-15] each
		boat capacity -> [2-5]
		instance count -> [10]
	*/

	std::random_device generator;

	std::uniform_int_distribution<int> people(3, 15);
	std::uniform_int_distribution<int> boat(2, 5);

	const int instanceCount = 10;
	
	std::vector<Algorithms> algs =
	{
		Algorithms::Random,
		Algorithms::BKT,
		Algorithms::IDDFS,
		Algorithms::AStar,
	};

	// create instances input

	// bc, nm, nc
	std::vector<std::pair<int, int>> pairs;
	while (pairs.size() < instanceCount)
	{
		const int peopleCount = people(generator);
		const int boatCapacity = boat(generator);

		// there are some invalid combinations -> rule them out
		bool targetReached = false;
		for (const auto& alg : algs)
		{
			Instance instance(peopleCount, peopleCount, boatCapacity);
			instance.SetAlgorithm(alg);
			instance.Solve();

			targetReached = instance.TargetHasBeenReached();

			if (targetReached == false) break;
		}

		if (targetReached) pairs.push_back({ boatCapacity,peopleCount });
	}

	std::vector<int> randomTransitionsCount;
	std::vector<long long> randomDuration;
	
	std::vector<int> BKTTransitionsCount;
	std::vector<long long> BKTDuration;
	
	std::vector<int> IDDFSTransitionsCount;
	std::vector<long long> IDDFSDuration;
	
	std::vector<int> aStarTransitionsCount;
	std::vector<long long> aStarDuration;

	for (int i = 0; i < instanceCount; i++)
	{
		// setup
		const int boatCapacity = pairs.at(i).first;
		const int peopleCount = pairs.at(i).second;

		std::cout << "\rSolving instance #" << i + 1;// << " [" << boatCapacity << "|" << peopleCount << "]!";

		for (const auto& alg : algs)
		{
			Instance instance(peopleCount, peopleCount, boatCapacity);
			instance.SetAlgorithm(alg);

			const auto begin = std::chrono::steady_clock::now(); 
			instance.Solve();
			const auto end = std::chrono::steady_clock::now();

			const auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

			switch (alg)
			{
			case Algorithms::Random:
				//std::cout << std::endl << "Random solution: ";
				randomDuration.push_back(delta);
				randomTransitionsCount.push_back(instance.GetTransitionsCount());
				break;
			case Algorithms::BKT:
				//std::cout << std::endl << "BKT solution: ";
				BKTDuration.push_back(delta);
				BKTTransitionsCount.push_back(instance.GetTransitionsCount());
				break;
			case Algorithms::IDDFS:
				//std::cout << std::endl << "IDDFS solution: ";
				IDDFSDuration.push_back(delta);
				IDDFSTransitionsCount.push_back(instance.GetTransitionsCount());
				break;
			case Algorithms::AStar:
				//std::cout << std::endl << "A* solution: ";
				aStarDuration.push_back(delta);
				aStarTransitionsCount.push_back(instance.GetTransitionsCount());
				break;
			default:
				// nothing
				break;
			}
		}
	}

	std::cout << " -> All done!" << std::endl;

	const int pr = 6;	// padding right
	const int pl = 12;	// padding left

	std::vector<std::pair<const char*, std::pair<std::vector<int>*, std::vector<long long>*>>> stats = {
		{"Random [" ,	{ &randomTransitionsCount,	&randomDuration	} },
		{"BKT [" ,		{ &BKTTransitionsCount,		&BKTDuration	} },
		{"IDDFS [" ,	{ &IDDFSTransitionsCount,	&IDDFSDuration	} },
		{"A* [" ,		{ &aStarTransitionsCount,	&aStarDuration	} },
	};

	std::cout << std::endl << "Stats" << std::endl;
	for (int i = 0; i < instanceCount; i++)
	{
		std::cout << "Instance: ["	<< std::setw(2) << i << "] -> [" << std::setw(2) << pairs.at(i).first << "bc|" << std::setw(2) << pairs.at(i).second << "nm|nc] ";
		
		for (const auto& stat : stats)
		{
			std::cout << stat.first << std::setw(pl) << 
				stat.second.first->at(i) << "t|" << std::setw(pl) <<
				stat.second.second->at(i) << "ns] ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Average transitions|time (in nanosecons)" << std::endl;

	stats.at(0).first = "Random";
	stats.at(1).first = "BKT";
	stats.at(2).first = "IDDFS";
	stats.at(3).first = "A*";

	for (const auto& stat : stats)
	{
		std::cout << std::left << std::setw(pr) << stat.first << " [" << std::right << std::setw(pl) <<
			std::accumulate(stat.second.first->begin(), stat.second.first->end(), 0) / instanceCount << "t|" <<
			std::setw(pl) << std::accumulate(stat.second.second->begin(), stat.second.second->end(), 0LL) / instanceCount << "ns] " <<
			std::endl;
	}

	std::cout << std::endl;

	return 0;
}
