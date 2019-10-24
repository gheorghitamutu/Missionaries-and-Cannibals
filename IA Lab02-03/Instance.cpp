#include "Instance.h"

Instance::~Instance()
{
	if (algorithm != nullptr)
	{
		//delete algorithm;
	}
}

Instance::Instance(int nc, int nm, int bc)
{
	state.Initialize(nc, nm, bc);
}

void Instance::SetAlgorithm(Algorithms algUsed)
{
	switch (algUsed)
	{
	case Algorithms::Random:
		algorithm = new Random();
		break;
	case Algorithms::BKT:
		algorithm = new BKT();
		break;
	case Algorithms::IDDFS:
		algorithm = new IDDFS();
		break;
	case Algorithms::AStar:
		algorithm = new AStar();
		break;
	default:
		/* error? */
		break;
	}
}

void Instance::Solve()
{
	algorithm->FindSolution(&state);
}

void Instance::PrintSolutionPath()
{
	algorithm->PrintSolutionPath();
}

int Instance::GetTransitionsCount()
{
	return algorithm->GetTransitionsCount();
}

bool Instance::TargetHasBeenReached()
{
	return algorithm->TargetHasBeenReached();
}
