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
		const int boatCapacity = boat(generator);
		const int peopleCount = people(generator);

		// there are some invalid combinations -> rule them out
		bool invalid = false;
		for (const auto& alg : algs)
		{
			Instance instance(peopleCount, peopleCount, boatCapacity);
			instance.SetAlgorithm(alg);
			instance.Solve();

			if (instance.TargetHasBeenReached() == false)
			{
				invalid = true;
				break;
			}
		}

		if (invalid == false)
		{
			pairs.push_back({ boatCapacity,peopleCount });
		}
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

		Instance iRandom(peopleCount, peopleCount, boatCapacity);
		iRandom.SetAlgorithm(Algorithms::Random);
		
		Instance iBKT(peopleCount, peopleCount, boatCapacity);
		iBKT.SetAlgorithm(Algorithms::BKT);

		Instance iIDDFS(peopleCount, peopleCount, boatCapacity);
		iIDDFS.SetAlgorithm(Algorithms::IDDFS);
		
		Instance iAStar(peopleCount, peopleCount, boatCapacity);
		iAStar.SetAlgorithm(Algorithms::AStar);
			  
		// run and count

		// -------------------- Start Random ------------------------------

		{
			const auto begin = std::chrono::steady_clock::now();

			iRandom.Solve();

			randomDuration.push_back(
				std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::steady_clock::now() - begin).count());

			randomTransitionsCount.push_back(iRandom.GetTransitionsCount());
		}

		// -------------------- Start BKT ---------------------------------

		{
			const auto begin = std::chrono::steady_clock::now();

			iBKT.Solve();

			BKTDuration.push_back(
				std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::steady_clock::now() - begin).count());

			BKTTransitionsCount.push_back(iBKT.GetTransitionsCount());
		}

		// -------------------- Start IDDFS --------------------------------

		{
			const auto begin = std::chrono::steady_clock::now();

			iIDDFS.Solve();

			IDDFSDuration.push_back(
				std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::steady_clock::now() - begin).count());

			IDDFSTransitionsCount.push_back(iIDDFS.GetTransitionsCount());
		}

		// -------------------- Start A* ----------------------------------

		{
			const auto begin = std::chrono::steady_clock::now();

			iAStar.Solve();

			aStarDuration.push_back(
				std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::steady_clock::now() - begin).count());

			aStarTransitionsCount.push_back(iAStar.GetTransitionsCount());
		}

		//std::cout << std::endl << "Random solution: ";
		//iRandom.PrintSolutionPath();
		//std::cout << std::endl;
		//
		//std::cout << std::endl << "BKT solution: ";
		//iBKT.PrintSolutionPath();
		//std::cout << std::endl;
		//
		//std::cout << std::endl << "IDDFS solution: ";
		//iIDDFS.PrintSolutionPath();
		//std::cout << std::endl;
		//
		//std::cout << std::endl << "A* solution: ";
		//iAStar.PrintSolutionPath();
		//std::cout << std::endl;
	}

	std::cout << " -> All done!" << std::endl;

	const int pr = 6;	// padding right
	const int pl = 12;	// padding left

	std::cout << "Stats" << std::endl;
	for (int i = 0; i < instanceCount; i++)
	{
		std::cout << "Instance: ["	<< std::setw(2) << i << "] -> [" << std::setw(2) << pairs.at(i).first << "bc|" << std::setw(2) << pairs.at(i).second << "nm|nc] ";
		
		std::cout << "Random ["		<< std::setw(pl) << randomTransitionsCount.at(i)	<< "t|" << std::setw(pl) << randomDuration.at(i)	<< "ns] ";
		std::cout << "BKT ["		<< std::setw(pl) << BKTTransitionsCount.at(i)		<< "t|" << std::setw(pl) << BKTDuration.at(i)		<< "ns] ";
		std::cout << "IDDFS ["		<< std::setw(pl) << IDDFSTransitionsCount.at(i)		<< "t|" << std::setw(pl) << IDDFSDuration.at(i)		<< "ns] ";
		std::cout << "A* ["			<< std::setw(pl) << aStarTransitionsCount.at(i)		<< "t|" << std::setw(pl) << aStarDuration.at(i)		<< "ns] ";
		
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Average transitions|time (in nanosecons)" << std::endl;

	std::cout << std::left << std::setw(pr) << "Random" << " [" << std::right << std::setw(pl) <<
		std::accumulate(randomTransitionsCount.begin(), randomTransitionsCount.end(), 0) / instanceCount << "t|" <<
		std::setw(pl) << std::accumulate(randomDuration.begin(), randomDuration.end(), 0) / instanceCount << "ns] " <<
		std::endl;

	std::cout << std::left << std::setw(pr) << "BKT" << " [" << std::right << std::setw(pl) <<
		std::accumulate(BKTTransitionsCount.begin(), BKTTransitionsCount.end(), 0) / instanceCount << "t|" <<
		std::setw(pl) << std::accumulate(BKTDuration.begin(), BKTDuration.end(), 0) / instanceCount << "ns] " <<
		std::endl;

	std::cout << std::left << std::setw(pr) << "IDDFS" << " [" << std::right << std::setw(pl) <<
		std::accumulate(IDDFSTransitionsCount.begin(), IDDFSTransitionsCount.end(), 0) / instanceCount << "t|" <<
		std::setw(pl) << std::accumulate(IDDFSDuration.begin(), IDDFSDuration.end(), 0) / instanceCount << "ns] " <<
		std::endl;

	std::cout << std::left << std::setw(pr) << "A*" << " [" << std::right << std::setw(pl) <<
		std::accumulate(aStarTransitionsCount.begin(), aStarTransitionsCount.end(), 0) / instanceCount << "t|" <<
		std::setw(pl) << std::accumulate(aStarDuration.begin(), aStarDuration.end(), 0) / instanceCount << "ns] " <<
		std::endl;

	std::cout << std::endl;

	return 0;
}
