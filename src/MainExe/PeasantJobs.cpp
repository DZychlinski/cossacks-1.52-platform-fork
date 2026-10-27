// PeasantJobs.cpp - peasant job classification shared by the resource bar and the AI log, see PeasantJobs.h.
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "PeasantJobs.h"
#include <string.h>

// Peasant jobs: engine order handlers (DoLink) that mean "working" for a peasant.
extern void TakeResLink(OneObject* OBJ);    // gathering food/wood/stone (also carrying it to the base)
extern void BuildObjLink(OneObject* OBJ);   // constructing / repairing a building
extern void GoToMineLink(OneObject* OB);    // walking to a mine to go inside

static const int kBarRefreshMs = 500;

enum WorkSlot { W_FOOD, W_WOOD, W_STONE, W_TOMINE, W_BUILD, W_IDLE, W_OTHER, W_COUNT };

// Resources gathered outside mines and the job slot of the peasants gathering them.
struct GatherDef
{
	int* ResID;     // pointer: the engine resource ids are globals filled at load time
	WorkSlot Slot;
};
static const GatherDef kGatherDefs[] = {
	{ &FoodID,  W_FOOD },   // fields, farms
	{ &TreeID,  W_WOOD },
	{ &StoneID, W_STONE },
};
static const int kNGather = sizeof(kGatherDefs) / sizeof(kGatherDefs[0]);

// Mine kind = the resource the mine produces (OB->RType), as the "I" statistics screen counts it (Mine.cpp NInGold/NInIron/NInCoal).
struct MineKindDef
{
	int* ResID;     // pointer: the engine resource ids are globals filled at load time
	int PeasantJobCounts::* Field;
};
static const MineKindDef kMineKindDefs[] = {
	{ &GoldID, &PeasantJobCounts::Gold },
	{ &IronID, &PeasantJobCounts::Iron },
	{ &CoalID, &PeasantJobCounts::Coal },
};
static const int kMineKinds = sizeof(kMineKindDefs) / sizeof(kMineKindDefs[0]);

// Current job of a free (not hidden) peasant, from its first order and the resource it carries.
static WorkSlot PeasantJob(OneObject* OB)
{
	Order1* OR1 = OB->LocalOrder;
	if (!OR1) return W_IDLE;
	byte res = OB->RType;
	if (OR1->DoLink == &TakeResLink) res = OR1->info.TakeRes.ResID;
	else if (OR1->DoLink == &BuildObjLink) return W_BUILD;
	else if (OR1->DoLink == &GoToMineLink) return W_TOMINE;
	for (int k = 0; k < kNGather; k++)
	{
		if (res == *kGatherDefs[k].ResID) return kGatherDefs[k].Slot;
	}
	return W_OTHER;
}

void CountPeasantJobs(int nation, PeasantJobCounts* out)
{
	memset(out, 0, sizeof(*out));
	if (nation < 0 || nation > 7) return;
	int work[W_COUNT];
	memset(work, 0, sizeof(work));
	int inside = 0;
	for (int i = 0; i < MAXOBJECT; i++)
	{
		OneObject* OB = Group[i];
		if (!OB || OB->Sdoxlo || OB->NNUM != nation) continue;
		NewMonster* NM = OB->newMons;
		if (NM->Building)
		{
			if (NM->Usage != MineID || !OB->Ready) continue;
			bool known = false;
			for (int k = 0; k < kMineKinds; k++)
			{
				if (OB->RType == *kMineKindDefs[k].ResID)
				{
					out->*kMineKindDefs[k].Field += OB->NInside;
					known = true;
				}
			}
			if (!known) out->OtherMineInside += OB->NInside;
			inside += OB->NInside;
		}
		else if (NM->Peasant)
		{
			out->Peasants++;
			if (!OB->Hidden) work[PeasantJob(OB)]++;  // hidden ones are inside mines/buildings, see the mines' NInside
		}
	}
	out->Food = work[W_FOOD];
	out->Wood = work[W_WOOD];
	out->Stone = work[W_STONE];
	out->ToMine = work[W_TOMINE];
	out->Build = work[W_BUILD];
	out->Idle = work[W_IDLE];
	int hiddenElsewhere = out->Peasants - work[W_FOOD] - work[W_WOOD] - work[W_STONE] - work[W_TOMINE]
		- work[W_BUILD] - work[W_IDLE] - work[W_OTHER] - inside;
	if (hiddenElsewhere < 0) hiddenElsewhere = 0;
	out->Other = work[W_OTHER] + hiddenElsewhere;
}

const PeasantJobCounts& GetLocalPeasantJobsCached()
{
	static PeasantJobCounts cache;
	static int cachedNation = -1;
	static DWORD cachedAt = 0;
	int nation = NatRefTBL[MyNation];
	DWORD now = GetTickCount();
	if (nation != cachedNation || now - cachedAt >= DWORD(kBarRefreshMs))
	{
		CountPeasantJobs(nation, &cache);
		cachedNation = nation;
		cachedAt = now;
	}
	return cache;
}
