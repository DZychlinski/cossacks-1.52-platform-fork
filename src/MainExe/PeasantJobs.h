// PeasantJobs.h - what a nation's peasants are doing right now (read-only, no game state is changed).
// Used by the resource bar (ShowRMap in Nature.cpp) and the AI activity log (AILog.cpp).
#pragma once

struct PeasantJobCounts
{
	int Food;        // gathering food (fields, farms), including carrying it to the base
	int Wood;
	int Stone;
	int Gold;        // peasants inside the finished gold mines
	int Iron;
	int Coal;
	int ToMine;      // walking to a mine to go inside (which mine is not known)
	int Build;       // constructing / repairing a building
	int Idle;        // no order at all
	int Other;       // any other order, plus peasants hidden somewhere other than a mine
	int Peasants;    // all peasants of the nation
	int OtherMineInside;  // peasants inside mines that are not gold/iron/coal (already part of Peasants)
};

// Walks the whole object list: do not call every frame.
void CountPeasantJobs(int nation, PeasantJobCounts* out);

// Counts for the local player (NatRefTBL[MyNation]), recomputed at most every ~500 ms of real time.
const PeasantJobCounts& GetLocalPeasantJobsCached();
