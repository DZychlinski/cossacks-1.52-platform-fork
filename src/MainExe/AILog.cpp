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
#include "Megapolis.h"
#include "PlayerInfo.h"
#include "AILog.h"
#include "PeasantJobs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <map>
#include <string>

extern int GameTime;       // game seconds, 0 at game start (mapa.cpp)
extern bool g_AutoPilot;   // Interface.cpp: [Debug] AutoPilot, the AI plays the local player
extern City CITY[8];       // Account = the score the game shows
extern PlayerInfo PINFO[8];
extern char** NatNames;
extern int NNations;

static const int kSnapshotPeriodSec = 60;
static const int kMaxUnitTypes = 2048;    // Nation::NKilled / NProduced / Mon size
static const int kMaxUpgrades = 4096;     // same bound as AIReportProject
static const int kNNations = 8;
static const int kMineRulesSize = 27;     // SetMinesUpgradeRules table: 3 levels x gold/iron/coal x 3 values
static const byte kNoReportedOutcome = 0xFF;

// Mine kind = the resource the mine produces (OB->RType), as the "I" statistics screen counts it.
struct MineKindDef
{
	int* ResID;     // pointer: the engine resource ids are globals filled at load time
	const char* Label;
};
static const MineKindDef kMineKindDefs[] = {
	{ &GoldID, "gold" },
	{ &IronID, "iron" },
	{ &CoalID, "coal" },
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
static int g_playersMask = 0;            // nations listed in the header
static bool g_lastTeamLogged = false;
static char g_exeDir[MAX_PATH];

// Change detection for AI proposals / settings, and baselines for the per-minute unit counters.
static byte g_lastWant[kNNations][2][kMaxUpgrades];
static int g_lastPDist[kNNations][3];
static bool g_hasPDist[kNNations];
static int g_lastMineRules[kNNations][kMineRulesSize];
static bool g_hasMineRules[kNNations];
static word g_prevProduced[kNNations][kMaxUnitTypes];
static word g_prevKilled[kNNations][kMaxUnitTypes];

static void ResetTracking()
{
	memset(g_lastWant, kNoReportedOutcome, sizeof g_lastWant);
	memset(g_hasPDist, 0, sizeof g_hasPDist);
	memset(g_hasMineRules, 0, sizeof g_hasMineRules);
	for (int ni = 0; ni < kNNations; ni++)
	{
		memcpy(g_prevProduced[ni], NATIONS[ni].NProduced, sizeof g_prevProduced[ni]);
		memcpy(g_prevKilled[ni], NATIONS[ni].NKilled, sizeof g_prevKilled[ni]);
	}
}

static void CloseLog()
{
	if (g_file)
	{
		fclose(g_file);
		g_file = NULL;
	}
	g_nextSnapshot = 0;
	g_lastTeamLogged = false;
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
	// NatNames is indexed by a unit's own nation (as in the selection panel), not by PINFO.NationID
	int N = NtNUnits[NI];
	word* units = NatList[NI];
	for (int i = 0; i < N && NatNames; i++)
	{
		if (units[i] == 0xFFFF) continue;
		OneObject* OB = Group[units[i]];
		if (OB && !OB->Sdoxlo && OB->Ref.General->NatID < NNations && NatNames[OB->Ref.General->NatID])
			return NatNames[OB->Ref.General->NatID];
	}
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
	ResetTracking();
	fprintf(g_file, "# Cossacks AI log, started %04d-%02d-%02d %02d:%02d:%02d\n",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	fprintf(g_file, "# INSANE %s\n", g_InsaneAI ? "on" : "off");
	if (g_AutoPilot) fprintf(g_file, "# AUTOPILOT on (the AI plays the local player)\n");
	g_playersMask = 0;
	for (int ni = 0; ni < 8; ni++)
	{
		if (NationInGame(ni))
		{
			g_playersMask |= 1 << ni;
			fprintf(g_file, "# P%d %s nation=%s name=%s%s\n", ni, NATIONS[ni].AI_Enabled ? "AI" : "HUMAN",
				NationName(ni), PlayerName(ni), NATIONS[ni].AI_Enabled && (g_InsaneAI || g_InsaneNation[ni]) ? " insane=yes" : "");
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

static const char* WhoTag(int NI)
{
	return NATIONS[NI].AI_Enabled ? "AI" : "HUMAN";
}

static bool AnyAI()
{
	for (int ni = 0; ni < kNNations; ni++)
	{
		if (NATIONS[ni].AI_Enabled) return true;
	}
	return false;
}

void AILog_Event(int NI, const char* fmt, ...)
{
	if (NI < 0 || NI > 7 || !AnyAI()) return;
	if (!EnsureLog()) return;
	WriteStamp(NI, WhoTag(NI));
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
	PeasantJobCounts Work;
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

// "<tag> type=n ..." for counters that grew since the previous snapshot; updates the baseline.
// MADE lists units only (a building counted there would be a foundation), LOST everything.
static void PrintDeltas(const char* tag, const word* cur, word* prev, Nation* NT, bool withBuildings)
{
	fprintf(g_file, "%s", tag);
	for (int j = 0; j < NT->NMon && j < kMaxUnitTypes; j++)
	{
		word d = word(cur[j] - prev[j]);
		prev[j] = cur[j];
		GeneralObject* GO = NT->Mon[j];
		if (!d || !GO || !GO->Message) continue;
		if (!withBuildings && GO->newMons->Building) continue;
		std::string nm = GO->Message;
		for (size_t c = 0; c < nm.size(); c++)
		{
			if (nm[c] == ' ') nm[c] = '_';
		}
		fprintf(g_file, " %s=%d", nm.c_str(), int(d));
	}
	fputc('\n', g_file);
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
		CountPeasantJobs(i, &S[i].Work);
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
				bool known = false;
				for (int k = 0; k < kMineKinds; k++)
				{
					if (OB->RType == *kMineKindDefs[k].ResID)
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
		const char* who = WhoTag(ni);
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
		fprintf(g_file, "WORK food=%d wood=%d stone=%d", N.Work.Food, N.Work.Wood, N.Work.Stone);
		for (int k = 0; k < kMineKinds; k++) fprintf(g_file, " %s=%d", kMineKindDefs[k].Label, N.Mines[k].Inside);
		fprintf(g_file, " tomine=%d build=%d idle=%d other=%d\n", N.Work.ToMine, N.Work.Build, N.Work.Idle, N.Work.Other);
		WriteStamp(ni, who);
		PrintDeltas("MADE", NT->NProduced, g_prevProduced[ni], NT, false);
		WriteStamp(ni, who);
		PrintDeltas("LOST", NT->NKilled, g_prevKilled[ni], NT, true);
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

void AILog_Want(int NI, bool Upgrade, int Index, const char* Name, AILogWant Outcome, int A, int B)
{
	if (NI < 0 || NI >= kNNations || Index < 0 || Index >= (Upgrade ? kMaxUpgrades : kMaxUnitTypes)) return;
	if (!EnsureLog()) return;
	byte& last = g_lastWant[NI][Upgrade ? 1 : 0][Index];
	if (last == byte(Outcome)) return;
	last = byte(Outcome);
	const char* tag = Upgrade ? "WANT-UPG" : "WANT";
	switch (Outcome)
	{
	case AILOG_WANT_ACCEPTED:
		AILog_Event(NI, "%s %s accepted", tag, Name);
		break;
	case AILOG_WANT_MAX_REACHED:
		AILog_Event(NI, "%s %s skip: max reached (%d/%d)", tag, Name, A, B);
		break;
	case AILOG_WANT_TOO_EXPENSIVE:
		AILog_Event(NI, "%s %s skip: too expensive (costs %d%% of stock, allowed %d%%)", tag, Name, A, B);
		break;
	case AILOG_WANT_UNAVAILABLE:
		AILog_Event(NI, "%s %s skip: not available", tag, Name);
		break;
	case AILOG_WANT_BUSY:
		AILog_Event(NI, "%s %s skip: busy (another building project in progress)", tag, Name);
		break;
	}
}

void AILog_PDist(int NI, int OnFood, int OnWood, int OnStone)
{
	if (NI < 0 || NI >= kNNations || !EnsureLog()) return;
	int* last = g_lastPDist[NI];
	if (g_hasPDist[NI] && last[0] == OnFood && last[1] == OnWood && last[2] == OnStone) return;
	g_hasPDist[NI] = true;
	last[0] = OnFood;
	last[1] = OnWood;
	last[2] = OnStone;
	AILog_Event(NI, "PDIST food=%d wood=%d stone=%d (relative weights of the peasants gathering each)", OnFood, OnWood, OnStone);
}

void AILog_MineRules(int NI, const int* Table)
{
	if (NI < 0 || NI >= kNNations || !EnsureLog()) return;
	if (g_hasMineRules[NI] && !memcmp(g_lastMineRules[NI], Table, sizeof g_lastMineRules[NI])) return;
	g_hasMineRules[NI] = true;
	memcpy(g_lastMineRules[NI], Table, sizeof g_lastMineRules[NI]);
	char buf[kMineRulesSize * 8 + 8];
	int n = 0;
	for (int i = 0; i < kMineRulesSize; i++)
	{
		bool lastOfGroup = i % 3 == 2;
		bool lastOverall = i == kMineRulesSize - 1;
		n += snprintf(buf + n, sizeof buf - n, "%d%s", Table[i], lastOverall ? "" : (lastOfGroup ? " | " : ","));
	}
	AILog_Event(NI, "MINE-RULES (3 values per group; groups: gold,iron,coal for level I, then II, then III) %s", buf);
}

// Fork: the end of the game.

// Lowest nation of the only team still alive, -1 while two or more teams fight (allies share NMask bits)
static int LastTeamLeader()
{
	int first = -1;
	for (int ni = 0; ni < kNNations; ni++)
	{
		if (!(g_playersMask & (1 << ni)) || NtNUnits[ni] <= 0) continue;
		if (first == -1) first = ni;
		else if (!(NATIONS[first].NMask & NATIONS[ni].NMask)) return -1;
	}
	return first;
}

static void WriteSummary(const char* what)
{
	fprintf(g_file, "%02d:%02d %s length=%d:%02d\n", GameTime / 60, GameTime % 60, what, GameTime / 60, GameTime % 60);
	for (int ni = 0; ni < kNNations; ni++)
	{
		if (!(g_playersMask & (1 << ni))) continue;
		fprintf(g_file, "%02d:%02d   P%d %-5s nation=%s alive=%s score=%d units=%d\n", GameTime / 60, GameTime % 60,
			ni, WhoTag(ni), NationName(ni), NtNUnits[ni] > 0 ? "yes" : "no", CITY[ni].Account, NtNUnits[ni]);
	}
	fflush(g_file);
}

static void LogLastTeam()
{
	if (!g_lastTeamLogged && LastTeamLeader() >= 0)
	{
		g_lastTeamLogged = true;
		WriteSummary("LAST-TEAM-STANDING");
	}
}

void AILog_GameOver()
{
	if (!g_file) return;
	WriteSummary("END");
	CloseLog();
}

void AILog_Tick()
{
	if (GameTime < g_lastGameTime) CloseLog();
	g_lastGameTime = GameTime;
	if (GameTime < g_nextSnapshot) return;
	if (!AnyAI()) return;
	if (!EnsureLog()) return;
	g_nextSnapshot = GameTime - GameTime % kSnapshotPeriodSec + kSnapshotPeriodSec;
	Snapshot();
	LogLastTeam();
}
#endif
