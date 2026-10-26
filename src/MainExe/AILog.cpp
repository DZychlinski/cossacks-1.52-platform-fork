// AILog.cpp - opt-in AI activity log, see AILog.h. The whole body is compiled only for MSVC builds
// with COS_AI_LOG, so the WASM/GCC build (build_dmcr.sh) is unaffected.
#if defined(_MSC_VER) && defined(COS_AI_LOG)
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "PlayerInfo.h"
#include "AILog.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <map>
#include <string>

extern int GameTime;       // game seconds, 0 at game start (mapa.cpp)
extern PlayerInfo PINFO[8];
extern char** NatNames;
extern int NNations;

static const int kSnapshotPeriodSec = 60;

// Mine kind = Nat->Mon index offset from Nat->UID_MINE (see Megapolis.cpp UpgradeMines: NMinGL, NMinFE, NMinCO).
struct MineKindDef
{
	int Offset;
	const char* Label;
};
static const MineKindDef kMineKindDefs[] = {
	{ 0, "gold" },  // UID_MINE + 0
	{ 1, "iron" },  // UID_MINE + 1
	{ 2, "coal" },  // UID_MINE + 2
};
static const int kMineKinds = sizeof(kMineKindDefs) / sizeof(kMineKindDefs[0]);

// Upgrade that starts the 18th century: NewUpgrade::Name is "MAIN" + two-letter nation code (e.g. MAINAU),
// registered as XVIII by the AI DLLs.
static const char* const kCenturyUpgradePrefix = "MAIN";
static const size_t kCenturyUpgradeNameLen = 6;

// Usage values of key buildings counted individually (everything else is listed by name, because
// 17c/18c barracks, stable, academy, market and diplomatic centre have no distinct Usage).
struct KeyBuildingDef
{
	int Usage;
	const char* Label;
};
static const KeyBuildingDef kKeyBuildings[] = {
	{ CenterID,  "TownCentre" },  // main building
	{ SkladID,   "Storehouse" },  // resource drop-off
	{ MelnicaID, "Mill" },        // food
	{ PortID,    "Port" },        // ships / sea trade
	{ TowerID,   "Towers" },      // defence
};
static const int kNKeyBuildings = sizeof(kKeyBuildings) / sizeof(kKeyBuildings[0]);

static FILE* g_file = NULL;
static int g_lastGameTime = -1;
static int g_nextSnapshot = 0;
static char g_exeDir[MAX_PATH];

static void CloseLog()
{
	if (g_file)
	{
		fclose(g_file);
		g_file = NULL;
	}
	g_nextSnapshot = 0;
}

const char* AILog_ResName(int ResID)
{
	if (ResID == FoodID) return "food";
	if (ResID == TreeID) return "wood";
	if (ResID == StoneID) return "stone";
	if (ResID == GoldID) return "gold";
	if (ResID == IronID) return "iron";
	if (ResID == CoalID) return "coal";
	return "?";
}

static PlayerInfo* PlayerOf(int NI)
{
	byte color = NatRefTBL[NI];
	for (int j = 0; j < 8; j++)
	{
		if (PINFO[j].ColorID == color) return &PINFO[j];
	}
	return NULL;
}

static const char* NationName(int NI)
{
	PlayerInfo* P = PlayerOf(NI);
	if (P && NatNames && P->NationID < NNations && NatNames[P->NationID]) return NatNames[P->NationID];
	return "?";
}

static const char* PlayerName(int NI)
{
	PlayerInfo* P = PlayerOf(NI);
	return P ? P->name : "?";
}

static bool NationInGame(int NI)
{
	return NATIONS[NI].AI_Enabled || NtNUnits[NI] > 0;
}

static void OpenLog()
{
	if (!g_exeDir[0])
	{
		GetModuleFileNameA(NULL, g_exeDir, MAX_PATH);
		char* slash = strrchr(g_exeDir, '\\');
		if (slash) *slash = 0;
	}
	char path[MAX_PATH * 2];
	snprintf(path, sizeof path, "%s\\ai_logs", g_exeDir);
	CreateDirectoryA(path, NULL);
	SYSTEMTIME st;
	GetLocalTime(&st);
	snprintf(path, sizeof path, "%s\\ai_logs\\ai_%04d%02d%02d-%02d%02d%02d.log", g_exeDir,
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	g_file = fopen(path, "w");
	if (!g_file) return;
	fprintf(g_file, "# Cossacks AI log, started %04d-%02d-%02d %02d:%02d:%02d\n",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	for (int ni = 0; ni < 8; ni++)
	{
		if (NationInGame(ni))
		{
			fprintf(g_file, "# P%d %s nation=%s name=%s\n", ni, NATIONS[ni].AI_Enabled ? "AI" : "HUMAN",
				NationName(ni), PlayerName(ni));
		}
	}
	fflush(g_file);
}

// Starts a new file when a game starts: game time going back means the map was (re)loaded.
static bool EnsureLog()
{
	if (g_lastGameTime >= 0 && GameTime < g_lastGameTime) CloseLog();
	g_lastGameTime = GameTime;
	if (!g_file) OpenLog();
	return g_file != NULL;
}

static void WriteStamp(int NI, const char* who)
{
	fprintf(g_file, "%02d:%02d P%d %-5s ", GameTime / 60, GameTime % 60, NI, who);
}

void AILog_Event(int NI, const char* fmt, ...)
{
	if (NI < 0 || NI > 7 || !NATIONS[NI].AI_Enabled) return;
	if (!EnsureLog()) return;
	WriteStamp(NI, "AI");
	va_list ap;
	va_start(ap, fmt);
	vfprintf(g_file, fmt, ap);
	va_end(ap);
	fputc('\n', g_file);
	fflush(g_file);
}

struct MineStat
{
	int Count;
	int Building;      // foundation, not finished yet
	int Inside;
	int Capacity;
	int Levels[8];
};

struct NatStat
{
	int Peasants;
	int Military;
	MineStat Mines[3];
	MineStat OtherMines;
	int Key[8];
	std::map<std::string, int> Buildings;  // finished buildings by name (barracks etc. differ per nation)
};

static void AddMine(MineStat& M, OneObject* OB)
{
	if (!OB->Ready)
	{
		M.Building++;
		return;
	}
	M.Count++;
	M.Inside += OB->NInside;
	M.Capacity += OB->AddInside + OB->Ref.General->MoreCharacter->MaxInside;
	int lev = OB->SingleUpgLevel - 2;
	if (lev < 0) lev = 0;
	if (lev > 7) lev = 7;
	M.Levels[lev]++;
}

static void PrintMine(const char* label, const MineStat& M)
{
	fprintf(g_file, " %s: mines=%d building=%d inside=%d/%d levels=", label, M.Count, M.Building, M.Inside, M.Capacity);
	for (int l = 0; l < 8; l++) fprintf(g_file, "%d%s", M.Levels[l], l < 7 ? "," : "");
}

static bool CenturyDone(Nation* NT)
{
	for (int i = 0; i < NT->NUpgrades; i++)
	{
		NewUpgrade* NU = NT->UPGRADE[i];
		if (NU && NU->Name && strlen(NU->Name) == kCenturyUpgradeNameLen
			&& !strncmp(NU->Name, kCenturyUpgradePrefix, strlen(kCenturyUpgradePrefix)))
		{
			return NU->Done;
		}
	}
	return false;
}

static void Snapshot()
{
	NatStat* S = new NatStat[8];
	for (int i = 0; i < 8; i++)
	{
		S[i].Peasants = 0;
		S[i].Military = 0;
		memset(S[i].Mines, 0, sizeof(S[i].Mines));
		memset(&S[i].OtherMines, 0, sizeof(S[i].OtherMines));
		memset(S[i].Key, 0, sizeof(S[i].Key));
	}
	for (int i = 0; i < MAXOBJECT; i++)
	{
		OneObject* OB = Group[i];
		if (!OB || OB->Sdoxlo || OB->NNUM > 7) continue;
		NatStat& N = S[OB->NNUM];
		NewMonster* NM = OB->newMons;
		if (NM->Building)
		{
			byte use = NM->Usage;
			if (use == MineID)
			{
				int kind = int(OB->NIndex) - int(OB->Nat->UID_MINE);
				bool known = false;
				for (int k = 0; k < kMineKinds; k++)
				{
					if (kind == kMineKindDefs[k].Offset)
					{
						AddMine(N.Mines[k], OB);
						known = true;
					}
				}
				if (!known) AddMine(N.OtherMines, OB);
			}
			else if (OB->Ready)
			{
				for (int k = 0; k < kNKeyBuildings; k++)
				{
					if (use == kKeyBuildings[k].Usage) N.Key[k]++;
				}
				const char* nm = OB->Ref.General->Message;
				if (nm && use != FarmID) N.Buildings[nm]++;
				if (use == FarmID) N.Key[7]++;
			}
		}
		else if (NM->Peasant)
		{
			N.Peasants++;
		}
		else
		{
			N.Military++;
		}
	}
	for (int ni = 0; ni < 8; ni++)
	{
		if (!NationInGame(ni)) continue;
		Nation* NT = NATIONS + ni;
		NatStat& N = S[ni];
		const char* who = NT->AI_Enabled ? "AI" : "HUMAN";
		WriteStamp(ni, who);
		fprintf(g_file, "SNAP nation=%s name=%s\n", NationName(ni), PlayerName(ni));
		WriteStamp(ni, who);
		fprintf(g_file, "RES  food=%d wood=%d stone=%d gold=%d iron=%d coal=%d\n",
			XRESRC(ni, FoodID), XRESRC(ni, TreeID), XRESRC(ni, StoneID),
			XRESRC(ni, GoldID), XRESRC(ni, IronID), XRESRC(ni, CoalID));
		WriteStamp(ni, who);
		fprintf(g_file, "GATH food=%d wood=%d stone=%d gold=%d iron=%d coal=%d (gathered so far)\n",
			NT->ResTotal[FoodID], NT->ResTotal[TreeID], NT->ResTotal[StoneID],
			NT->ResTotal[GoldID], NT->ResTotal[IronID], NT->ResTotal[CoalID]);
		WriteStamp(ni, who);
		fprintf(g_file, "POP  pop=%d cap=%d peasants=%d military=%d xviii=%s\n",
			NT->NGidot, NT->NFarms, N.Peasants, N.Military, CenturyDone(NT) ? "yes" : "no");
		WriteStamp(ni, who);
		fprintf(g_file, "MINE");
		for (int k = 0; k < kMineKinds; k++) PrintMine(kMineKindDefs[k].Label, N.Mines[k]);
		if (N.OtherMines.Count || N.OtherMines.Building) PrintMine("other", N.OtherMines);
		fputc('\n', g_file);
		WriteStamp(ni, who);
		fprintf(g_file, "BLD ");
		for (int k = 0; k < kNKeyBuildings; k++) fprintf(g_file, " %s=%d", kKeyBuildings[k].Label, N.Key[k]);
		fprintf(g_file, " Houses=%d\n", N.Key[7]);
		WriteStamp(ni, who);
		fprintf(g_file, "BLDN");
		for (std::map<std::string, int>::iterator it = N.Buildings.begin(); it != N.Buildings.end(); ++it)
		{
			fprintf(g_file, " %s=%d", it->first.c_str(), it->second);
		}
		fputc('\n', g_file);
	}
	fflush(g_file);
	delete[] S;
}

void AILog_Tick()
{
	if (GameTime < g_lastGameTime) CloseLog();
	g_lastGameTime = GameTime;
	if (GameTime < g_nextSnapshot) return;
	bool anyAI = false;
	for (int ni = 0; ni < 8; ni++) anyAI = anyAI || NATIONS[ni].AI_Enabled;
	if (!anyAI) return;
	if (!EnsureLog()) return;
	g_nextSnapshot = GameTime - GameTime % kSnapshotPeriodSec + kSnapshotPeriodSec;
	Snapshot();
}
#endif
