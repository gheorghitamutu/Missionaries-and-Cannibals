#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <list>
#include <iterator>

enum class BoatPosition
{
	LEFT = 0,
	RIGHT = 1
};

class State
{
public:
	int nc; // numbers of cannibals
	int nm; // numbers of missionaries
	int bc; // capacity of the boat

	int mmal1; // missionaries on left shore
	int cmal1; // missionaries on right shore
	int mmal2; // missionaries on right shore
	int cmal2; // cannibals on right shore

	BoatPosition bp; // boat position (left/right - 0/1)

	std::vector<State*> nextStates; // all the possible next states (initialized when requested)

	// cache of all states shared by all states
	inline static std::list<State> cachedStates{};

	// the level where we encountered this state | (-1) -> undefined
	int level = -1;

	/* Is this the final state that we are looking for? */
	bool IsFinal();

	/* Set initial state. */
	void Initialize(int nc, int nm, int bc);

	/* Is this a valid state? */
	bool IsValid();

	/* 
		Is this a valid transition?

		mm - missionaries moved
		cm - cannibals moved
		
		Valid transitions:
			nm1 > 0 -> nm1 > nc1
			nm2 > 0 -> nm2 > nc2
			mm >= 0 && cm >= 0 && mm + cm > 0 -> bp = 1 | optional - we are not required to use negative values
			mm <= 0 && cm <= 0 && mm + cm < 0 -> bp = 2	| optional - we are not required to use negative values
			|mm + cm| <= bc
	*/
	bool IsValidTransition(int mm, int cm);

	/*
		Go to the next valid state.
		We assume that you that the input is already validated.
	*/
	void NextValidState(int mm, int cm, int level);

	/*
		The number of cannibals on any shore exceeds 
		the number of missionaries among them;
	*/
	bool IsLost();

	/*
		All the states possible from the current state taking into account
		capacity & position of the boat for the current missionaries and cannibals
		on that shore.
	*/
	void GenerateNextStates(int level = -1);

	std::vector<State*> GetNextPossibleStates();
	std::vector<State*> GetNextPossibleStatesIndexes() { return nextStates; };
	static std::list<State>* GetCachedStates() { return &cachedStates; };

	/*
		Are 2 states equal? Used on comparison for unique list of states.
	*/
	bool operator==(const State& rhs) const;

	int GetMissionariesFromCurrentShore() const;
	int GetCannibalsFromCurrentShore() const;

	/*
		Index of cached state.
	*/
	State* CachedState(State* state);

	/*
		Set current level in graph/tree.
	*/
	void SetLevel(int level);

	int GetLevel();

	void Print() const;
};

