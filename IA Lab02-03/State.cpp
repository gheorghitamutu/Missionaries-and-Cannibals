#include "State.h"

bool State::IsFinal()
{
	return 
		nc == cmal2 && 
		nm == mmal2;
}

void State::Initialize(int nm, int nc, int bc)
{
	this->nc = nc;
	this->nm = nm;
	this->bc = bc;

	mmal1 = nm;
	cmal1 = nc;

	mmal2 = 0;
	cmal2 = 0;

	bp = BoatPosition::LEFT;

	level = 0; // default level in tree

	CachedState(this);
}

bool State::IsValid()
{
	return
		mmal1 >= 0 && mmal1 <= nm &&
		cmal1 >= 0 && cmal1 <= nm &&
		mmal1 + mmal2 == nm &&
		cmal1 + cmal2 == nc;
}

bool State::IsValidTransition(int mm, int cm)
{
	bool result = true;

	if (mm + cm <= bc && // we can only move so many as the boat capacity allows it
		mm + cm > 0) // but there's no point in having an empty boat
	{
		switch (bp)
		{
		case BoatPosition::LEFT:
			result =
				(mm <= mmal1) && // we can take no more than the number of missionaries from the left shore
				(cm <= cmal1); // we can take no more than the number of cannibals from the left shore
			break;
		case BoatPosition::RIGHT:
			result =
				(mm <= mmal2) && // we can take no more than the number of missionaries from the right shore
				(cm <= cmal2); // we can take no more than the number of cannibals from the right shore
			break;
		default:
			/* nothing */
			result = false;
			break;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

void State::NextValidState(int mm, int cm, int level = -1)
{
	switch (bp)
	{
	case BoatPosition::LEFT:
		this->mmal1 -= mm;
		this->cmal1 -= cm;
		this->mmal2 += mm;
		this->cmal2 += cm;
		this->bp = BoatPosition::RIGHT;
		break;
	case BoatPosition::RIGHT:
		this->mmal1 += mm;
		this->cmal1 += cm;
		this->mmal2 -= mm;
		this->cmal2 -= cm;
		this->bp = BoatPosition::LEFT;
		break;
	default:
		/* nothing */
		break;
	}

	nextStates.clear();

	this->level = level;
}

bool State::IsLost()
{
	bool IsLost = 
		mmal1 != 0 && mmal1 < cmal1 || 
		mmal2 != 0 && mmal2 < cmal2;

	return IsLost;
}

/*
	Generates all the permutations of missionaries and cannibals from the current shore (LEFT|RIGHT).
	It's slow and add several times some permutations (which are cleared at the end of the function)
	but it seems that it does the job.
*/
void State::GenerateNextStates(int level)
{
	nextStates.clear();
	nextStates.reserve(bc * bc);

	std::vector<std::pair<int, int>> statePairs;
	statePairs.reserve(bc * bc);

	if (level == -1)
	{
		if (this->level != -1)
		{
			level = this->level + 1;
		}
	}

	for (int i = 1; i <= bc; i++)
	{
		for (int j = i; j <= std::min(bc, this->GetMissionariesFromCurrentShore()); j++)
		{
			if (IsValidTransition(j, 0))
			{				
				statePairs.emplace_back(j, 0);
			}
		}

		for (int j = i; j <= std::min(bc, this->GetCannibalsFromCurrentShore()); j++)
		{
			if (IsValidTransition(0, j))
			{
				statePairs.emplace_back(0, j);
			}
		}

		for (int j = 1; j < std::min(i, this->GetMissionariesFromCurrentShore()); j++)
		{
			if (j > this->GetMissionariesFromCurrentShore() ||
				i - j > this->GetCannibalsFromCurrentShore())
			{
				continue;
			}

			if (IsValidTransition(j, i - j))
			{
				statePairs.emplace_back(j ,i - j);
			}
		}

		for (int j = 1; j < std::min(i, this->GetCannibalsFromCurrentShore()); j++)
		{
			if (j > this->GetCannibalsFromCurrentShore() ||
				i - j > this->GetMissionariesFromCurrentShore())
			{
				continue;
			}

			if (IsValidTransition(i - j, j))
			{
				statePairs.emplace_back(i - j, j);
			}
		}
	}

	std::sort(statePairs.begin(), statePairs.end(), 
		[](const std::pair<int, int>& a, const std::pair<int, int>& b) -> bool
		{
			return
				a.first < b.first ||
				(a.first == b.first &&
					a.second < b.second);
		});
	auto last = std::unique(statePairs.begin(), statePairs.end());
	statePairs.erase(last, statePairs.end());
	
	for (const auto& transitionPair : statePairs)
	{
		State state = *this;
		state.NextValidState(transitionPair.first, transitionPair.second, level);

		if (state.IsLost() == false)
		{
			nextStates.push_back(CachedState(&state));
		}
	}

	//std::cout << "Next possible states for: ";
	//this->Print(); 
	//std::cout << std::endl;
	//
	//for (const auto& state : nextStates)
	//{	
	//	state->Print();
	//	std::cout << std::endl;
	//
	//	if (state->bp == this->bp)
	//	{
	//			std::cout << "WTF" << std::endl;
	//	}
	//}
}

std::vector<State*> State::GetNextPossibleStates()
{
	return nextStates;
}

/* Compare all its *important* elements and check if the states are equal. */
bool State::operator==(const State& rhs) const
{
	return
		this->nc == rhs.nc &&
		this->nm == rhs.nm &&
		this->bc == rhs.bc &&
		this->mmal1 == rhs.mmal1 &&
		this->cmal1 == rhs.cmal1 &&
		this->mmal2 == rhs.mmal2 &&
		this->cmal2 == rhs.cmal2 &&
		this->bp == rhs.bp;
}

int State::GetMissionariesFromCurrentShore() const
{
	return (bp == BoatPosition::LEFT ? mmal1 : mmal2);
}

int State::GetCannibalsFromCurrentShore() const
{
	return (bp == BoatPosition::LEFT ? cmal1 : cmal2);
}

State* State::CachedState(State* state)
{
	const unsigned int size = cachedStates.size();
	const auto stateSearched = *state;
	
	const auto start = cachedStates.begin();
	const auto end = cachedStates.end();

	auto wanted = std::find(start, end, stateSearched);
	
	State* address;
	if (wanted != end)
	{
		address = &(*wanted);
	}
	else
	{
		cachedStates.push_back(stateSearched);
		address = &(*(++cachedStates.rbegin()).base());
	}

	return address;
}

void State::SetLevel(int level)
{
	this->level = level;
}

int State::GetLevel()
{
	return level;
}

void State::Print() const
{
	std::cout << "[" << mmal1 << "|" << cmal1 << "]" << 
		"[" << mmal2 << "|" << cmal2 << "][" <<
		(bp == BoatPosition::LEFT ? "L]" : "R]");
}
