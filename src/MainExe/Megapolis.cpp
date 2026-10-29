// City organization module
#include <cstdint>
#include <vector>
#include <algorithm>
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "multipl.h"
#include "fog.h"
#include "walls.h"
#include "Nature.h"
#include "Nucl.h"

#include <assert.h>
#include "Megapolis.h"

#include "MapSprites.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "Path.h"
#include "NewMon.h"
#include <math.h>
#include "StrategyResearch.h"

#include "Safety.h"
#include "ActiveScenary.h"
#include "Sort.h"
#include "ConstStr.h"
#include "EinfoClass.h"
#include <crtdbg.h>

extern const int kImportantMessageDisplayTime;

extern int PeaceTimeLeft;
extern City CITY[8];
bool CheckTowersPresence();
void CheckArmies(City* CT);

int CheckPositionForDanger(int x, int y, int z);
int GetTopology(int x, int y);
void A_BitvaLink(AI_Army* ARM);
void BuildWithSelected(byte NI, word ObjID, byte OrdType);
bool FindPortPlace(Nation* NT, int* xx, int* yy);
bool FindOilSpot(int* x, int* y, int r);
void SmartGamer(City* CT);
int DHAPROB;
int DRECLPROB;
int DDIVPROB;
int HANSWPROB;
int HREQRADIUS;
int CROWDMIN;
int HCPROB;
int MaxAllowedComputerAI;
int CMC1;
int CMC2;
int CMC3;

extern bool GodMode;
extern bool House0;
void SmartGamer(City* CT);
int BestForce;
int BestNat;
int NextBest;
byte NLocks[64][64];
Cell8x8::Cell8x8()
{
	for (int i = 0; i < 5; i++)UnitsAmount[i] = 0;
	Neighbor = 0;
};
word FindTerrEnemy(int xCell, int yCell, int mx, int my, int dist, byte Mask);
//TotalCInfo  TCInf;

int Trx;
int Try;
int Trx1;
int Try1;
byte TerrMap[64][64];
int wer(byte x)
{
	if (x)return 1;
	else return 0;
};
void TerrSpot(int x, int y, byte c)
{
	if (x > 0 && y > 0)TerrMap[y][x] += c;
};
void CreateTerrMap(byte NN)
{
};
//Блок оценки сил противника
void Forces::SetDefaults()
{
	MinX = 55255;
	MaxX = 0;
	MinY = 55255;
	MaxX = 0;
	Damage = 0;
	Life = 0;
	Shield = 0;
	Population = 0;
	NBuild = 0;
	CenterX = 0;
	CenterY = 0;
	StrongFactor = 0;
};
Forces NForces[8];
void GetForces()
{
};

//Функционалы для определения места для здания
//1-место для стандартного здания (ферма, барак,...)
int FUNC1(int* x, int* y, int Lx, int Ly, byte NI)
{
	return 0;
};
int FUNC3(int* x, int* y, int Lx, int Ly, byte NI)
{
	return 0;
};
//for Towers
int FUNC2(int* x, int* y, int Lx, int Ly, byte NI)
{
	return 0;
};
void OccupyCell(int x, int y, byte NI)
{

};
word FindPeasantInCell(int x, int y, byte NI)
{
	return 0xFFFF;
};
word FindPeasant(int x, int y, byte NI)
{
	return 0xFFFF;
};
word FindWPeasantInCell(int x, int y, byte NI)
{
	return 0xFFFF;
};
word FindWPeasant(int x, int y, byte NI)
{
	return 0xFFFF;
};
void FreeCell(int x, int y, byte NI)
{

};
void City::CreateCity(byte N)
{
	memset(this, 0, sizeof(City));
	NATIONS[N].CITY = this;
	Nat = &NATIONS[N];
	NI = N;
	CenterFound = false;
	PresentProject = false;
	FarmsUnderConstruction = 0;
	NConstructions = 0;
	NFields = 0;
	AllowWood = false;
	AllowStone = false;
	AllowMine = false;
	INFORM = NULL;
	FieldReady = false;
	BestOnField = 60;
	NDivr = 0;
	MaxGuards = 100;
	AbsMaxGuards = 100;

	for (int i = 0; i < MaxBrig; i++)Brigs[i].Init(this, i);
	for (int i = 0; i < MaxArm; i++)ARMS[i].InitArmy(this);
	OnField.Enabled = true;
	OnStone.Enabled = true;
	OnWood.Enabled = true;
	Builders.Enabled = true;
	Free.Enabled = true;
	InMines.Enabled = true;
	FreeArmy.Enabled = true;
	Defenders.Enabled = true;
	Guards.Enabled = true;
	Agressors.Enabled = true;
	Settlers.Enabled = true;
	NAgressors = 5;

	TransportID = 0xFFFF;
	TransportSN = 0xFFFF;

	Nat->SELO.Init();
	Nat->ARMY.Init();
	Nat->SCIENCE.Init();
	Nat->GENERAL.Init();
	AI_DEVELOPMENT = 1;
	AI_WATER_BATTLE = 1;
	AI_LAND_BATTLE = 1;
	AI_MINES_CPTURE = 1;
	AI_TOWN_DEFENCE = 1;
	AI_MINES_UPGRADE = 1;
	AI_FAST_DIVERSION = 1;
	/*
	NewMonster* NM=Nat->Mon[Nat->UID_HOUSE]->newMons;
	int x0=NM->BuildX0-NM->BuildY0;
	int x1=NM->BuildX1-NM->BuildY1;
	int y0=NM->BuildX0+NM->BuildY0;
	int y1=NM->BuildX1+NM->BuildY1;
	FarmLX=(x1-x0)+20;
	FarmLY=(y1-y0)+20;
	*/
	LFarmIndex = 0;
	LFarmRadius = 0;
	for (int i = 0; i < 8; i++)Nat->GENERAL.RESP[i] = 100;
	MAX_WORKERS = Nat->DEFAULT_MAX_WORKERS;
	NDefn = 0;
	MaxDefn = 0;
	DefInf = NULL;
	DefArms = NULL;
	NDefArms = 0;
	MaxDefArms = 0;
	for (int i = 0; i < 128; i++)WaterBrigs[i] = 0xFFFF;
	Nat->MU1G_PERCENT[0] = 80;
	Nat->MU1G_PERCENT[1] = 80;
	Nat->MU1G_PERCENT[2] = 80;

	Nat->MU1I_PERCENT[0] = 15;
	Nat->MU1I_PERCENT[1] = 15;
	Nat->MU1I_PERCENT[2] = 15;

	Nat->MU1C_PERCENT[0] = 15;
	Nat->MU1C_PERCENT[1] = 15;
	Nat->MU1C_PERCENT[2] = 15;
	//=============II==============//

	Nat->MU2G_PERCENT[0] = 90;
	Nat->MU2G_PERCENT[1] = 50;
	Nat->MU2G_PERCENT[2] = 30;

	Nat->MU2I_PERCENT[0] = 25;
	Nat->MU2I_PERCENT[1] = 20;
	Nat->MU2I_PERCENT[2] = 15;

	Nat->MU2C_PERCENT[0] = 25;
	Nat->MU2C_PERCENT[1] = 20;
	Nat->MU2C_PERCENT[2] = 15;
	//=============III==============//

	Nat->MU3G_PERCENT[0] = 90;
	Nat->MU3G_PERCENT[1] = 90;
	Nat->MU3G_PERCENT[2] = 90;

	Nat->MU3I_PERCENT[0] = 30;
	Nat->MU3I_PERCENT[1] = 30;
	Nat->MU3I_PERCENT[2] = 30;

	Nat->MU3C_PERCENT[0] = 30;
	Nat->MU3C_PERCENT[1] = 30;
	Nat->MU3C_PERCENT[2] = 30;

	MyIsland = 255;
};
void City::GetNextPlaceForFarm(int* x, int* y)
{
	int xx, yy;
	int szzz = msx << 5;
	if (LFarmRadius > 70)LFarmRadius = 0;
	//getting minimal topological distance on this radius
	int top0 = GetTopology(CenterX << 7, CenterY << 7);
	if (top0 >= 0xFFFE)return;
	int MinTDS = 100000;
	top0 *= NAreas;
	char* xi = Rarr[LFarmRadius].xi;
	char* yi = Rarr[LFarmRadius].yi;
	int N = Rarr[LFarmRadius].N;
	for (int p = 0; p < N; p++)
	{
		int xi = Rarr[LFarmRadius].xi[LFarmIndex] * FarmLX;
		int yi = Rarr[LFarmRadius].yi[LFarmIndex] * FarmLY;
		xx = (CenterX << 7) + ((xi + yi) >> 1);
		yy = (CenterY << 7) + ((yi - xi) >> 1);
		if (xx > 0 && xx < szzz && yy>0 && yy < szzz)
		{
			int top = SafeTopRef((xx >> 6), (yy >> 6));
			if (top < 0xFFFE)
			{
				int dst = LinksDist[top0 + top];
				if (dst < MinTDS)MinTDS = dst;
			};
		};
	};
	if (MinTDS >= 0xFFFE)MinTDS = 0;
	do
	{
		this->LFarmIndex++;
		if (LFarmIndex >= Rarr[LFarmRadius].N)
		{
			LFarmIndex = 0;
			LFarmRadius++;
		};
		if (LFarmRadius >= 60)
		{
			LFarmIndex = 0;
			LFarmRadius = 60;
			*x = 0;
			*y = 0;
			return;
		};
		int xi = Rarr[LFarmRadius].xi[LFarmIndex] * FarmLX;
		int yi = Rarr[LFarmRadius].yi[LFarmIndex] * FarmLY;
		xx = (CenterX << 7) + ((xi + yi) >> 1);
		yy = (CenterY << 7) + ((yi - xi) >> 1);
		if (xx > 0 && xx < szzz && yy>0 && yy < szzz)
		{
			if (!GNFO.EINF[NI]->GetSafeVal(xx >> 7, yy >> 7))
			{
				int top = SafeTopRef((xx >> 6), (yy >> 6));
				if (top < 0xFFFE)
				{
					int dst = LinksDist[top0 + top];
					if (dst - MinTDS < 30)
					{
						*x = xx;
						*y = yy;
						return;
					};
				};
			};
		};
	} while (true);
};
int  City::GetFreeBrigade()
{
	for (int i = 0; i < MaxBrig - 9; i++)if (!Brigs[i].Enabled)
	{
		memset(Brigs + i, 0, sizeof(Brigade));
		Brigs[i].CT = this;
		Brigs[i].ID = i;
		Brigs[i].SN = rando();
		Brigs[i].BOrder = NULL;
		Brigs[i].ArmyID = 0xFFFF;
		Brigs[i].LastTopology = 0xFFFF;
		Brigs[i].LastEnemyID = 0xFFFF;
		Brigs[i].LastEnemySN = 0xFFFF;
		return i;
	};
	return -1;
};
void City::AddInform(Inform* Inf, I_Clear* ICL)
{
	Inf->Next = INFORM;
	Inf->Previous = NULL;
	Inf->CT = this;
	Inf->IClr = ICL;
	if (INFORM)
	{
		INFORM->Previous = Inf;
	};
	INFORM = Inf;
};
Inform* City::SearchInform(word ID, word Essence, Inform* Inf)
{
	if (!Inf)Inf = INFORM;
	while (Inf && (Inf->ID != ID || Essence != Inf->Essence))Inf = Inf->Next;
	return Inf;
};
Inform* City::SearchInform(word ID, Inform* Inf)
{
	if (!Inf)Inf = INFORM;
	else Inf = Inf->Next;
	while (Inf && Inf->ID != ID)Inf = Inf->Next;
	return Inf;
};
void City::DelInform(Inform* INF)
{
	if (INF->IClr)INF->IClr(INF);
	if (INF->Previous)
	{
		INF->Previous->Next = INF->Next;
	}
	else
	{
		INFORM = INF->Next;
	};
	if (INF->Next)INF->Next->Previous = INF->Previous;
	free(INF);
};
void City::DelInform()
{
	while (INFORM)
	{
		if (INFORM->IClr)INFORM->IClr(INFORM);
		INFORM = INFORM->Next;
	};
};
Inform* SearchInform(word ID, word Essence, Inform* inf);

void TakeResLink(OneObject* OBJ);
void BuildObjLink(OneObject* OBJ);
void NewMonsterSendToLink(OneObject* OB);
void NewMonsterSmartSendToLink(OneObject* OBJ);
bool PUSE(OneObject* OB)
{
	return OB->UnlimitedMotion || CheckBar(OB->x, OB->y, OB->Lx, OB->Lx);
};
int CheckBuilders(OneObject* OB, City* CT)
{
	if (PUSE(OB))return -1;
	OB->DoNotCall = true;
	//OB->NoBuilder=true;
	//OB->DoWalls=false;
	if (OB->LocalOrder)
	{
		Order1* OR1 = OB->LocalOrder;
		if (OR1 && OR1->DoLink == NewMonsterSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1 && OR1->DoLink == NewMonsterSmartSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1->DoLink != &BuildObjLink)return CT->Free.ID;
	}
	else return CT->Free.ID;
	return -1;
};
int CheckFree(OneObject* OB, City* CT)
{
	if (PUSE(OB))return -1;
	OB->DoNotCall = false;
	if (OB->LocalOrder)
	{
		Order1* OR1 = OB->LocalOrder;
		if (OR1 && OR1->DoLink == NewMonsterSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1 && OR1->DoLink == NewMonsterSmartSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1->DoLink == &BuildObjLink)return CT->Builders.ID;
		if (OR1->DoLink == &TakeResLink)
		{
			if (OR1->info.TakeRes.ResID == FoodID)return -1;
		};
	};
	return -1;
};
int CheckFieldWorker(OneObject* OB, City* CT)
{
	if (OB->EnemyID != 0xFFFF && rando() > 1000)return -1;
	if (!CT->FieldReady)return -1;
	if (PUSE(OB))return -1;
	OB->DoNotCall = false;
	if (OB->LocalOrder)
	{
		Order1* OR1 = OB->LocalOrder;
		if (OR1 && OR1->DoLink == NewMonsterSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1 && OR1->DoLink == NewMonsterSmartSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1->DoLink == &BuildObjLink)return CT->Builders.ID;
		if (OR1->DoLink == &TakeResLink)
		{
			if (OR1->info.TakeRes.ResID == FoodID)return -1;
		};
	};
	for (int j = 0; j < 20; j++)
	{
		int k = (int(rando()) * CT->NFields) >> 15;
		OneSprite* OS = &Sprites[CT->FieldsID[k]];
		if (OS->Enabled)
		{
			OB->TakeResource(OS->x, OS->y, FoodID, 128, 0);
			return -1;
		};
	};
	return -1;
};
Area* GetAr(int rx, int ry)
{
	int tx = rx >> 10;
	int ty = ry >> 10;
	int ofst = tx + (ty << TopSH);
	if (ofst >= 0 && ofst < MaxTop)
	{
		word ti = TopRef[ofst];
		Area* AR = nullptr;
		bool OurZone = false;
		if (ti != 0xFFFF)
		{
			return TopMap + ti;
		};
	};
	return nullptr;
};
bool CheckPL(int rx, int ry)
{
	return !CheckBar((rx >> 8) - 2, (ry >> 8) - 2, 5, 5);
};
int Norma(int, int);

//returns -1
int CheckFreeArmy(OneObject* OB, City* CT)
{
	return -1;
};

int Norma(int, int);

//Fork: wood and stone workers go to the storehouse with the best mix of distance and workers already there.
//Before: the one nearest to the peasant, so most of them crowded around the storehouses next to the Town Centre.
static const int kSkladWorkerCost = 64 * 16;//one worker already gathering there weighs like 64 px more to walk (RealX units)
static const int kSkladLoadRecount = 32;    //game steps between two recounts of the workers per storehouse
static const int kMaxSklads = 8;            //City::WoodSkladID / StoneSkladID
struct SkladLoad
{
	const Brigade* Workers;
	int Time;
	int Load[kMaxSklads];
};
static SkladLoad g_SkladLoad[8][2];//[nation][wood, stone]
extern int tmtmt;

static int NearestSklad(int x, int y, const word* ids, const word* sns, int n)
{
	int best = -1;
	int bestR = 0;
	for (int i = 0; i < n; i++)
	{
		OneObject* OBS = Group[ids[i]];
		if (!OBS || OBS->Serial != sns[i] || OBS->Sdoxlo)continue;
		int R = Norma(x - OBS->RealX, y - OBS->RealY);
		if (best == -1 || R < bestR)
		{
			best = i;
			bestR = R;
		}
	}
	return best;
}

static int PickSklad(City* CT, OneObject* OB, const word* ids, const word* sns, int n, const Brigade* workers)
{
	if (n > kMaxSklads)n = kMaxSklads;
	SkladLoad& L = g_SkladLoad[CT->NI][workers == &CT->OnWood ? 0 : 1];
	if (L.Workers != workers || tmtmt - L.Time >= kSkladLoadRecount || tmtmt < L.Time)
	{
		L.Workers = workers;
		L.Time = tmtmt;
		memset(L.Load, 0, sizeof(L.Load));
		for (int j = 0; j < workers->NMemb; j++)
		{
			OneObject* W = Group[workers->Memb[j]];
			if (!W || W->Serial != workers->MembSN[j] || W->Sdoxlo)continue;
			int s = NearestSklad(W->RealX, W->RealY, ids, sns, n);
			if (s != -1)L.Load[s]++;
		}
	}
	int best = -1;
	int bestCost = 0;
	for (int i = 0; i < n; i++)
	{
		OneObject* OBS = Group[ids[i]];
		if (!OBS || OBS->Serial != sns[i] || OBS->Sdoxlo)continue;
		int cost = Norma(OB->RealX - OBS->RealX, OB->RealY - OBS->RealY) + L.Load[i] * kSkladWorkerCost;
		if (best == -1 || cost < bestCost)
		{
			best = i;
			bestCost = cost;
		}
	}
	if (best != -1)L.Load[best]++;
	return best;
}

int CheckWoodWorker(OneObject* OB, City* CT)
{
	//if(!CT->FieldReady)return true;
	if (OB->EnemyID != 0xFFFF && rando() > 1000)return -1;
	if (PUSE(OB))return -1;
	OB->DoNotCall = false;
	OB->NoBuilder = false;
	if (OB->LocalOrder)
	{
		Order1* OR1 = OB->LocalOrder;
		if (OR1 && OR1->DoLink == NewMonsterSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1 && OR1->DoLink == NewMonsterSmartSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1->DoLink == &BuildObjLink)return CT->Builders.ID;
		if (OR1->DoLink == &TakeResLink)
		{
			if (OR1->info.TakeRes.ResID == TreeID)return -1;
		};
	};
	if (CT->NWoodSklads)
	{
		//1.Nearest, the workers already there counted as extra distance (Fork)
		OneObject* OBS = NULL;
		int Best = PickSklad(CT, OB, CT->WoodSkladID, CT->WoodSkladSN, CT->NWoodSklads, &CT->OnWood);
		if (Best != -1)
		{
			OBS = Group[CT->WoodSkladID[Best]];
			int xx = OBS->RealX >> 4;
			int yy = OBS->RealY >> 4;
			int xxx = xx + (rando() & 511) - 256;
			int yyy = yy + (rando() & 511) - 256;
			OB->TakeResource(xxx, yyy, TreeID, 128, 0);
			return -1;
		};
	};
	if (CT->NStoneSklads)
	{
		//1.Nearest, the workers already there counted as extra distance (Fork)
		OneObject* OBS = NULL;
		int Best = PickSklad(CT, OB, CT->StoneSkladID, CT->StoneSkladSN, CT->NStoneSklads, &CT->OnStone);
		if (Best != -1)
		{
			OBS = Group[CT->StoneSkladID[Best]];
			int xx = OBS->RealX >> 4;
			int yy = OBS->RealY >> 4;
			int xxx = xx + (rando() & 511) - 256;
			int yyy = yy + (rando() & 511) - 256;
			OB->TakeResource(xxx, yyy, TreeID, 128, 0);
			return -1;
		};
	};
	return -1;
};
int CheckStoneWorker(OneObject* OB, City* CT)
{
	if (OB->EnemyID != 0xFFFF && rando() > 1000)return -1;
	//if(!CT->FieldReady)return true;
	if (PUSE(OB))return -1;
	OB->DoNotCall = false;
	OB->NoBuilder = false;
	if (OB->LocalOrder)
	{
		Order1* OR1 = OB->LocalOrder;
		if (OR1 && OR1->DoLink == NewMonsterSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1 && OR1->DoLink == NewMonsterSmartSendToLink && OR1->NextOrder)OR1 = OR1->NextOrder;
		if (OR1->DoLink == &BuildObjLink)return CT->Builders.ID;
		if (OR1->DoLink == &TakeResLink)
		{
			if (OR1->info.TakeRes.ResID == StoneID)return -1;
		};
	};
	if (CT->NStoneSklads)
	{
		//1.Nearest, the workers already there counted as extra distance (Fork)
		OneObject* OBS = NULL;
		int Best = PickSklad(CT, OB, CT->StoneSkladID, CT->StoneSkladSN, CT->NStoneSklads, &CT->OnStone);
		if (Best != -1)
		{
			OBS = Group[CT->StoneSkladID[Best]];
			int xx = OBS->RealX >> 4;
			int yy = OBS->RealY >> 4;
			int xxx = xx + (rando() & 511) - 256;
			int yyy = yy + (rando() & 511) - 256;
			OB->TakeResource(xxx, yyy, StoneID, 128, 0);
			return -1;
		};
	};
	if (CT->NWoodSklads)
	{
		//1.Nearest, the workers already there counted as extra distance (Fork)
		OneObject* OBS = NULL;
		int Best = PickSklad(CT, OB, CT->WoodSkladID, CT->WoodSkladSN, CT->NWoodSklads, &CT->OnWood);
		if (Best != -1)
		{
			OBS = Group[CT->WoodSkladID[Best]];
			int xx = OBS->RealX >> 4;
			int yy = OBS->RealY >> 4;
			int xxx = xx + (rando() & 511) - 256;
			int yyy = yy + (rando() & 511) - 256;
			OB->TakeResource(xxx, yyy, StoneID, 128, 0);
			return -1;
		};
	};
	return -1;
};

bool City::CheckTZone(int x, int y, int Lx, int Ly)
{
	return true;
};
void City::CloseCity()
{
	for (int i = 0; i < MaxBrig; i++)if (Brigs[i].Enabled)Brigs[i].DeleteAll();
	for (int i = 0; i < MaxArm; i++)if (ARMS[i].Enabled)ARMS[i].ClearArmy();
	DelInform();
	DelIdeas();
	memset(this, 0, sizeof(City));
	if (NDefn)
	{
		for (int i = 0; i < NDefn; i++)
		{
			if (DefInf[i].Def)
			{
				free(DefInf[i].Def);
				free(DefInf[i].DefSN);
			};
		};
	};
	if (DefInf)free(DefInf);
	DefInf = NULL;
	NDefn = 0;
	MaxDefn = 0;
	NDivr = 0;
	for (int i = 0; i < NGroups; i++)free(GroupsSet[i]);
	if (GroupsSet)
	{
		free(GroupsSet);
		free(NGroupsInSet);
	};
	GroupsSet = NULL;
	NGroupsInSet = NULL;
	NGroups = 0;
};
bool City::TryToFindPlace(int* x, int* y, int Lx, int Ly, byte Kind)
{
	return true;
};
void MakeShipBattle(Brigade* BR);
void MakeDiversion(Brigade* BR);
void SearchArmy(OneObject* OB);
void City::RegisterNewUnit(OneObject* OB)
{
	if (!Nat->AI_Enabled)return;
	if (OB)
	{
		if (OB->newMons->Peasant)
		{
			if (PUSE(OB))return;
			OB->Borg = 1;
			Free.AddObject(OB);
		}
		else
		{
			if (OB->newMons->Officer || OB->newMons->Baraban)
			{
				SearchArmy(OB);
				OB->Borg = 1;
				OB->DoNotCall = 1;
				OB->NoBuilder = 1;
				return;
			};
			if (OB->newMons->Transport)
			{
				OB->Borg = 1;
				int ID = GetFreeBrigade();
				if (ID != -1)
				{
					Brigade* BR = Brigs + ID;
					BR->Enabled = true;
					BR->AddObject(OB);
					MakeDiversion(BR);
				};
				return;
			};
			if (!(OB->newMons->Building || OB->newMons->LockType))
			{
				if (PUSE(OB))return;
				OB->Borg = 1;
				byte use = OB->newMons->Usage;
				bool AGRES = false;
				if (use == PushkaID || use == MortiraID || use == FastHorseID || use == SupMortID
					|| use == GrenaderID || use == HardHorceID || use == StrelokID || use == ArcherID
					|| use == HorseStrelokID)AGRES = true;
				if (!AI_TOWN_DEFENCE)AGRES = true;
				if (use == SupMortID)
				{
					OB->Zombi = true;
				}
				else
				{
					if (AGRES)
					{
						Agressors.AddObject(OB);
						OB->DoNotCall = true;
						//OB->AutoKill=true;
					}
					else
					{
						if (FreeArmy.NMemb > 15 && Guards.NMemb > MaxGuards)
						{
							if (rando() < 2048 && Guards.NMemb < AbsMaxGuards)Defenders.AddObject(OB);
							else Agressors.AddObject(OB);
							OB->DoNotCall = true;
						}
						else
						{
							if (rando() < 10000 && FreeArmy.NMemb < 20)FreeArmy.AddObject(OB);
							else if (rando() < 6000 && Guards.NMemb < 100)Defenders.AddObject(OB);
							else
							{
								Agressors.AddObject(OB);
								OB->DoNotCall = true;
							};
						};
					};
				};
			};
			if (OB->newMons->LockType && AI_WATER_BATTLE)
			{
				byte use = OB->newMons->Usage;
				if (use != FisherID)
				{
					int id = GetFreeBrigade();
					if (id != -1)
					{
						Brigade* BR = Brigs + id;
						RegisterWaterBrigade(id);
						BR->Enabled = true;
						BR->AddObject(OB);
						BR->Direction = 0;
						if (OB->newMons->Usage == GaleraID)
						{
							BR->Direction = 1;
						};
						MakeShipBattle(BR);
						OB->Borg = 1;
					};
				};
			};
		};
	};
};
void CheckArmies(City*);
void City::UnRegisterNewUnit(OneObject* OB)
{
	//if(!Nat->AI_Enabled)return;
	if (OB && OB->BrigadeID != 0xFFFF && !OB->NewBuilding)
	{
		Brigade* BR = Brigs + OB->BrigadeID;
		if (!(BR->NMemb && BR->Enabled))
		{
			OB->BrigadeID = 0xFFFF;
			OB->InArmy = false;
			return;
		};
		word* Memb = BR->Memb;
		word* MembSN = BR->MembSN;
		int NMemb = BR->NMemb;
		int Index = OB->BrIndex;
		//assert(BR->NMemb<8000);
		if (Index < NMemb - 1)
		{
			if (OB->InArmy || BR->WarType)
			{
				Memb[Index] = 0xFFFF;
				MembSN[Index] = 0xFFFF;
			}
			else
			{
				memmove(Memb + Index, Memb + Index + 1, (NMemb - Index - 1) << 1);
				memmove(MembSN + Index, MembSN + Index + 1, (NMemb - Index - 1) << 1);
				if (BR->PosCreated)
				{
					memmove(BR->posX + Index, BR->posX + Index + 1, (NMemb - Index - 1) << 2);
					memmove(BR->posY + Index, BR->posY + Index + 1, (NMemb - Index - 1) << 2);
				};
				BR->NMemb--;
			};
		}
		else
		{
			if (OB->InArmy || BR->WarType)
			{
				Memb[Index] = 0xFFFF;
				MembSN[Index] = 0xFFFF;
			}
			else
			{
				BR->NMemb--;
			};
		};
		(&BR->BM.Peons)[GetBMIndex(OB)]--;
		OB->BrigadeID = 0xFFFF;
		OB->InArmy = false;
		BR->SetIndex();
		if (OB->InArmy)
		{
			int N = 0;
			for (int j = 0; j < NMemb; j++)
			{
				if (Memb[j] != 0xFFFF)N++;
			};
			if (!N)
			{
				if (BR->Memb)
				{
					free(BR->Memb);
					free(BR->MembSN);
					BR->Memb = NULL;
					BR->MembSN = NULL;
				};
				BR->MaxMemb = 0;
				BR->NMemb = 0;
				BR->Enabled = false;
				memset(&BR->BM, 0, sizeof(BR)->BM);
			};
		};
	};
};
void EraseBrigade(Brigade* BR);
void SearchArmy(OneObject* OB);
bool CanProduce = 0;

void City::EnumUnits()
{
	if (NI == MyNation)
		CanProduce = 0;

	OneObject* OB;

	memset(UnitAmount, 0, sizeof(UnitAmount));
	memset(ReadyAmount, 0, sizeof(ReadyAmount));
	memset(UnBusyAmount, 0, sizeof(UnBusyAmount));
	memset(PRPIndex, 255, sizeof(PRPIndex));
	memset(UPGIndex, 255, sizeof(UPGIndex));

	Amount = 0;
	Nat->NFarms = 0; // Сбрасываем NFarms для всех наций перед подсчётом
	Nat->NGidot = 0;

	bool BUNT = Nat->GoldBunt;
	int NBUILDERS = 0;
	bool REGI = Nat->AI_Enabled;

	for (int i = 0; i < 8; i++)
		Nat->ResSpeed[i] = 0;

	Nat->NArtdep = 0; // Инициализация перед циклом
	for (int i = 0; i < 4; i++)
		Nat->NArtUnits[i] = 0;

	for (int i = 0; i < MAXOBJECT; i++)
	{
		OB = Group[i];
		if (OB && OB->NNUM == NI && (OB->Hidden || !OB->Sdoxlo))
		{
			if (REGI && !(OB->Borg || OB->NewBuilding))
			{
				if (OB->BrigadeID == 0xFFFF)
					RegisterNewUnit(OB);
				else
					OB->Borg = 1;
			}

			if (OB->DoWalls && !OB->Hidden)
				NBUILDERS++;

			UnitAmount[OB->NIndex]++;

			AdvCharacter* ACR = OB->Ref.General->MoreCharacter;
			NewMonster* NM = OB->newMons;

			if (REGI && (NM->Officer || NM->Baraban))
			{
				if (OB->BrigadeID == 0xFFFF && !OB->LocalOrder)
				{
					SearchArmy(OB);
				}
			}

			if (OB->Wall)
			{
				if (OB->Ready)
					ReadyAmount[OB->NIndex]++;
			}
			else
			{
				if ((NM->Building && OB->Stage >= ACR->ProduceStages) || (!NM->Building))
				{
					ReadyAmount[OB->NIndex]++;

					// Подсчёт ферм: исключаем при house0 и в GodMode для своей нации
					if (NM->Farm)
					{
						if (!House0 && (!GodMode || (GodMode && NI != MyNation)))
						{
							Nat->NFarms += NM->NInFarm;
						}
					}

					if (!(NM->Building || (OB->Sdoxlo && !OB->Hidden)))
						Nat->NGidot++;
				}
			}

			if (OB->Ready || OB->LocalOrder)
			{
				if (NM->ArtDepo)
				{
					if (GodMode)
						Nat->NArtdep = 40000; // Фиксированный лимит в godmode
					else
						Nat->NArtdep++;       // Стандартный подсчёт
				}

				if (NM->ArtSet)
					Nat->NArtUnits[NM->ArtSet - 1]++;
			}

			Nat->ResSpeed[NM->ResConsID] += NM->ResConsumer;
			word NIND = OB->NIndex;

			if (OB->Ready && !(intptr_t)(OB->LocalOrder))
			{
				UnBusyAmount[NIND]++;
				Producer[NIND] = i;
			}

			if (NIND > MaxType) MaxType = NIND;
			if (NIND < MinType) MinType = NIND;

			OB->TurnOff = false;
			if (BUNT)
			{
				switch (NM->Behavior)
				{
				case 1: // TURNOFF
					OB->TurnOff = true;
					break;
				case 2:
					if (OB->NNUM == MyNation)
						CreateTimedHintEx(GOLDFIN, kImportantMessageDisplayTime, 32);
					NATIONS[OB->NNUM].CITY->Account -= OB->newMons->Ves * 3;
					OB->NNUM = 7;
					OB->NMask = 1 << 7;
					OB->Ref.General = NATIONS[7].Mon[OB->NIndex];
					OB->Nat = NATIONS + 7;
					break;
				}

				if (OB->InArmy && OB->BrigadeID != 0xFFFF)
					EraseBrigade(OB->Nat->CITY->Brigs + OB->BrigadeID);
			}

			if (NI == MyNation && !CanProduce)
				CanProduce = OB->Nat->PACount[OB->NIndex] != 0;
		}
	}

	// Если house0: у остальных наций обнуляем фермы
	if (House0 && NI != MyNation)
	{
		Nat->NFarms = 0;
	}

	// Если GodMode: для своей нации — 15000
	if (GodMode && NI == MyNation)
	{
		Nat->NFarms = 15000;
	}

	word PID = Nat->UID_PEASANT;
	int NPS = UnitAmount[PID];
	int NEEDBLD = 12;

	if (NPS > 20)
		NEEDBLD += (NPS - 20) >> 3;
	if (NEEDBLD > 20)
		NEEDBLD = 20;

	if (NBUILDERS < NEEDBLD)
	{
		int np = NEEDBLD - NBUILDERS;
		for (int i = 0; i < MAXOBJECT && np; i++)
		{
			OB = Group[i];
			if (OB && OB->NIndex == PID && OB->NNUM == NI && !(OB->Sdoxlo || OB->NoBuilder || OB->DoWalls))
			{
				OB->DoWalls = true;
				np--;
			}
		}
	}
}

void City::AddProp(word NIN, GeneralObject* GO, word prod, word per)
{
	if (NProp >= 100)return;
	//if(GO->MaxAutoAmount<=UnitAmount[NIN])return;
	ProposedProject* PR = &Prop[NProp];
	NProp++;
	PR->PKind = 0;
	PR->NIndex = NIN;
	PR->Percent = per;
	PRPIndex[NIN] = NProp - 1;
	//PR->Cost=GO->cost;
	PR->ProducerIndex = prod;
};

void City::AddUpgr(word UIN, word prod, word per)
{
	if (NProp >= 100)return;
	for (int i = 0; i < NProp; i++)
	{
		ProposedProject* PRP = &Prop[i];
		if (PRP->NIndex == UIN && PRP->PKind == 1)return;
	};
	ProposedProject* PR = &Prop[NProp];
	NProp++;
	PR->PKind = 1;
	PR->NIndex = UIN;
	PR->Percent = per;
	UPGIndex[UIN] = NProp - 1;
	PR->ProducerIndex = prod;
};

void GetUnitCost(byte NI, word NIndex, int* Cost);
void City::EnumProp()
{
	NProp = 0;
	//enumerating monsters could be produced 
	int PC;
	//int* RESU=&RESRC[Nat->NNUM][0];
	GeneralObject** GOA = Nat->Mon;
	for (int i = MinType; i <= MaxType; i++)
	{
		if (UnBusyAmount[i])
		{
			PC = Nat->PACount[i];
		}
		else
		{
			if (GOA[i]->newMons->Usage == PeasantID)
			{
				PC = Nat->PACount[i];
			}
			else
			{
				PC = 0;
			}
		}
		for (int j = 0; j < PC; j++)
		{
			word s = Nat->PAble[i][j];
			//проверить возможность производства
			GeneralObject* GO = Nat->Mon[s];
			bool Able = true;
			if (GO->newMons->Peasant)
			{
				if (MAX_WORKERS < FreePS)Able = false;
			}
			if (GO->Enabled && Able)
			{
				int COST[8];
				GetUnitCost(Nat->NNUM, s, COST);
				int j;
				for (j = 0; j < 8 && COST[j] <= XRESRC(Nat->NNUM, j); j++);
				if (j == 8)
				{
					int maxper = Nat->GENERAL.GetMonsterCostPercent(NI, s);
					/*
					switch(GO->Branch){
					case 0://SELO
						maxper=Nat->SELO.GetMonsterCostPercent(NI,s);
						break;
					case 1://ARMY
						maxper=Nat->ARMY.GetMonsterCostPercent(NI,s);
						break;
					case 2://SCIENCE
						maxper=Nat->SCIENCE.GetMonsterCostPercent(NI,s);
						break;
					case 255:
						maxper=Nat->GENERAL.GetMonsterCostPercent(NI,s);
						break;
					};
					*/
					/*
					for(int p=0;p<8;p++){
						if(RESU[p]){
							int per1=div(COST[p]<<7,RESU[p]).quot;
							if(per1>maxper)maxper=per1;
						};
					};
					*/
					AddProp(s, GO, Producer[i], maxper);
				}
			}
		}

		GeneralObject* GO = GOA[i];
		if (UnBusyAmount[i]) PC = GO->NUpgrades;
		else PC = 0;
		for (int j = 0; j < PC; j++)
		{
			word s = GO->Upg[j];
			//проверить возможность производства
			NewUpgrade* NU = Nat->UPGRADE[s];
			bool OKK = false;
			if (NU->Individual || NU->StageUp)
			{
				OneObject* POB = Group[Producer[i]];
#ifdef CONQUEST
				if (NU->StageUp)
				{
					byte st = (POB->StageState >> (NU->Level * 3)) & 7;
					OKK = POB->Ready && st != 2 && NU->Enabled;
				};
#endif
				if (NU->Individual)OKK = POB->Ready && POB->SingleUpgLevel == NU->Level && NU->Enabled;
			}
			else
			{
				OKK = NU->Enabled && !NU->IsDoing;
			};
			if (OKK)
			{
				word* COST = NU->Cost;
				int j;
				for (j = 0; j < 8 && COST[j] <= XRESRC(Nat->NNUM, j); j++);
				if (j == 8)
				{
					int maxper = Nat->GENERAL.GetUpgradeCostPercent(NI, s);
					/*
					switch(GO->Branch){
					case 0://SELO
						maxper=Nat->SELO.GetUpgradeCostPercent(NI,s);
						break;
					case 1://ARMY
						maxper=Nat->ARMY.GetUpgradeCostPercent(NI,s);
						break;
					case 2://SCIENCE
						maxper=Nat->SCIENCE.GetUpgradeCostPercent(NI,s);
						break;
					case 255:
						maxper=Nat->GENERAL.GetUpgradeCostPercent(NI,s);
						break;
					};
					*/
					AddUpgr(s, Producer[i], maxper);
				};
			};
		};
	};
};

#ifdef COS_AI_LOG
#include "AILog.h"
#endif
#define FARMID 20

static bool ProducerHasUsage(Nation* nation, word producerType, byte usage)
{
	if (producerType >= nation->NMon || !nation->PAble[producerType])return false;
	for (int i = 0; i < nation->PACount[producerType]; i++)
	{
		word unitType = nation->PAble[producerType][i];
		if (unitType < nation->NMon && nation->Mon[unitType]->newMons->Usage == usage)
		{
			return true;
		}
	}
	return false;
}

static word FindInsaneProducerType(Nation* nation, byte requiredUsage, bool requireMixedCavalry)
{
	for (word i = 0; i < nation->NMon; i++)
	{
		GeneralObject* producer = nation->Mon[i];
		if (!producer || !producer->newMons->Building || !ProducerHasUsage(nation, i, requiredUsage))continue;
		if (!requireMixedCavalry || ProducerHasUsage(nation, i, FastHorseID) || ProducerHasUsage(nation, i, HardHorceID))
		{
			return i;
		}
	}
	return 0xFFFF;
}

static word FindProducedUnitType(Nation* nation, word producerType, byte usage)
{
	if (producerType >= nation->NMon || !nation->PAble[producerType])return 0xFFFF;
	for (int i = 0; i < nation->PACount[producerType]; i++)
	{
		word unitType = nation->PAble[producerType][i];
		if (unitType < nation->NMon && nation->Mon[unitType]->newMons->Usage == usage)
		{
			return unitType;
		}
	}
	return 0xFFFF;
}

static bool UpgradeAffectsUnitType(const NewUpgrade* upgrade, Nation* nation, word unitType)
{
	if (!upgrade || unitType >= nation->NMon)return false;
	if (upgrade->UnitType == 0)
	{
		if (upgrade->UnitGroup)
		{
			for (int i = 0; i < upgrade->NUnits; i++)
			{
				if (upgrade->UnitGroup[i] == unitType)return true;
			}
			return false;
		}
		return upgrade->UnitValue == unitType;
	}

	const byte unitKind = nation->Mon[unitType]->newMons->Kind;
	if (upgrade->UnitGroup)
	{
		for (int i = 0; i < upgrade->NUnits; i++)
		{
			if (upgrade->UnitGroup[i] == unitKind)return true;
		}
		return false;
	}
	return upgrade->UnitValue == unitKind;
}

static bool IsAcademyProducer(const ProposedProject* project, Nation* nation)
{
	OneObject* producer = Group[project->ProducerIndex];
	if (!producer || producer->NIndex >= nation->NMon)return false;
	GeneralObject* producerType = nation->Mon[producer->NIndex];
	for (int i = 0; i < producerType->NUpgrades; i++)
	{
		word upgradeID = producerType->Upg[i];
		if (upgradeID < nation->NUpgrades)
		{
			byte category = nation->UPGRADE[upgradeID]->CtgUpgrade;
			if (category == 20 || category == 21)return true;//wood or stone gathering
		}
	}
	return false;
}

static int GetInsaneUpgradePriority(const ProposedProject* project, Nation* nation, word pikeType, word dragoonType)
{
	if (project->PKind != 1 || project->NIndex >= nation->NUpgrades)return 100;
	NewUpgrade* upgrade = nation->UPGRADE[project->NIndex];
	//food gathering (Fork: it had no priority and Insane AI starved), but not with the last food: bought
	//first thing at the start it left 0 food and 8 of 18 peasants starved (AI log 2026-10-28 15:57, P6)
	const int kInsaneFoodUpgradeReserve = 1500;
	if (upgrade->CtgUpgrade == 19)
	{
		return XRESRC(nation->NNUM, FoodID) - upgrade->Cost[FoodID] >= kInsaneFoodUpgradeReserve ? 0 : 100;
	}
	if (upgrade->CtgUpgrade == 20)return 0;//wood gathering
	if (upgrade->CtgUpgrade == 21)return 1;//stone gathering

	const bool affectsPike = UpgradeAffectsUnitType(upgrade, nation, pikeType);
	const bool affectsDragoon = UpgradeAffectsUnitType(upgrade, nation, dragoonType);
	const bool academy = IsAcademyProducer(project, nation);
	if (upgrade->CtgUpgrade == 5 && affectsDragoon)return 2;//horse production speed
	if (!academy && (affectsPike || affectsDragoon) && upgrade->CtgUpgrade != 5 && upgrade->CtgUpgrade != 11)
	{
		return 3;//unit attack, protection and other direct upgrades
	}
	if (academy && upgrade->CtgUpgrade == 12 && affectsDragoon)
	{
		return 4;//academy firearm damage
	}
	return 100;
}

static word FindInsaneBuildingProject(City* city, word barracksType, word stableType)
{
	if (city->PresentProject)return 0xFFFF;
	const int peasants = city->UnitAmount[city->Nat->UID_PEASANT];
	const int desiredBarracks = (peasants >= 18) + (peasants >= 45) + (peasants >= 70);
	const int desiredStables = (peasants >= 25) + (peasants >= 40) + (peasants >= 55) +
		(peasants >= 70) + (peasants >= 85) + (peasants >= 100);

	bool needBarracks = barracksType != 0xFFFF && city->UnitAmount[barracksType] < desiredBarracks &&
		city->PRPIndex[barracksType] != 0xFFFF;
	bool needStable = stableType != 0xFFFF && city->UnitAmount[stableType] < desiredStables &&
		city->PRPIndex[stableType] != 0xFFFF;
	if (needBarracks && needStable)
	{
		return city->UnitAmount[stableType] < city->UnitAmount[barracksType] * 2 ?
			city->PRPIndex[stableType] : city->PRPIndex[barracksType];
	}
	if (needBarracks)return city->PRPIndex[barracksType];
	if (needStable)return city->PRPIndex[stableType];
	return 0xFFFF;
}

static void PrependProject(City* city, int& projectCount, word project)
{
	if (project == 0xFFFF)return;
	int oldPosition = -1;
	for (int i = 0; i < projectCount; i++)
	{
		if (city->BestProj[i] == project)
		{
			oldPosition = i;
			break;
		}
	}
	if (oldPosition == 0)return;
	if (oldPosition > 0)
	{
		for (int i = oldPosition; i > 0; i--)city->BestProj[i] = city->BestProj[i - 1];
	}
	else
	{
		if (projectCount >= 127)return;
		for (int i = projectCount; i > 0; i--)city->BestProj[i] = city->BestProj[i - 1];
		projectCount++;
	}
	city->BestProj[0] = project;
}

static void AppendProject(City* city, int& projectCount, word project)
{
	if (project == 0xFFFF || projectCount >= 127)return;
	for (int i = 0; i < projectCount; i++)if (city->BestProj[i] == project)return;
	city->BestProj[projectCount++] = project;
}

//Project kinds that Insane must never push behind its own priorities (matched on the produced unit's Usage).
struct InsaneExemptUsage
{
	byte Usage;
	const char* Reason;
};
static const InsaneExemptUsage kInsaneExemptUsages[] =
{
	{ FarmID, "houses: the AI already builds them in time for its population target, Insane must not starve it of housing" },
};

static bool IsInsaneExemptProject(City* city, word project)
{
	const ProposedProject* proposal = city->Prop + project;
	if (proposal->PKind != 0 || proposal->NIndex >= city->Nat->NMon)return false;
	const byte usage = city->Nat->Mon[proposal->NIndex]->newMons->Usage;
	for (const InsaneExemptUsage& exempt : kInsaneExemptUsages)
	{
		if (exempt.Usage == usage)return true;
	}
	return false;
}

//Stable move of exempt projects (houses) back to the front, keeping their original relative order
static void KeepExemptProjectsFirst(City* city, int projectCount)
{
	int front = 0;
	for (int i = 0; i < projectCount; i++)
	{
		word project = city->BestProj[i];
		if (!IsInsaneExemptProject(city, project))continue;
		for (int j = i; j > front; j--)city->BestProj[j] = city->BestProj[j - 1];
		city->BestProj[front++] = project;
	}
}

//Insane AI for every computer player (lobby option) or for this one (lobby difficulty "Insane")
static bool IsInsaneAI(Nation* nation)
{
	return g_InsaneAI || g_InsaneNation[nation->NNUM];
}

static int ApplyInsaneStrategy(City* city, int projectCount)
{
	if (!IsInsaneAI(city->Nat))return projectCount;

	Nation* nation = city->Nat;
	const word barracksType = FindInsaneProducerType(nation, LightInfID, false);
	const word stableType = FindInsaneProducerType(nation, HorseStrelokID, true);
	const word pikeType = FindProducedUnitType(nation, barracksType, LightInfID);
	const word dragoonType = FindProducedUnitType(nation, stableType, HorseStrelokID);

	word bestUpgradeProject = 0xFFFF;
	int bestUpgradePriority = 100;
	int bestUpgradeLevel = 256;
	for (int i = 0; i < city->NProp; i++)
	{
		ProposedProject* project = city->Prop + i;
		int priority = GetInsaneUpgradePriority(project, nation, pikeType, dragoonType);
		if (priority < 100)
		{
			int level = nation->UPGRADE[project->NIndex]->Level;
			if (priority < bestUpgradePriority ||
				(priority == bestUpgradePriority && level < bestUpgradeLevel) ||
				(priority == bestUpgradePriority && level == bestUpgradeLevel &&
					(bestUpgradeProject == 0xFFFF || project->NIndex < city->Prop[bestUpgradeProject].NIndex)))
			{
				bestUpgradePriority = priority;
				bestUpgradeLevel = level;
				bestUpgradeProject = i;
			}
		}
	}

	word priorityProject = bestUpgradePriority <= 1 ? bestUpgradeProject :
		FindInsaneBuildingProject(city, barracksType, stableType);
	if (priorityProject == 0xFFFF)priorityProject = bestUpgradeProject;
	PrependProject(city, projectCount, priorityProject);

	word pikeProject = pikeType == 0xFFFF ? 0xFFFF : city->PRPIndex[pikeType];
	word dragoonProject = dragoonType == 0xFFFF ? 0xFFFF : city->PRPIndex[dragoonType];
	word trainingProject = 0xFFFF;
	if (pikeProject != 0xFFFF && dragoonProject != 0xFFFF)
	{
		trainingProject = city->UnitAmount[pikeType] < city->UnitAmount[dragoonType] * 3 + 72 ?
			pikeProject : dragoonProject;
	}
	else if (pikeProject != 0xFFFF)trainingProject = pikeProject;
	else trainingProject = dragoonProject;
	//Fork: no more soldiers than kInsaneSoldiersPerPeasant per peasant (as AI_SOLDIERS_PER_PEASANT in Extraction.h):
	//a bigger army eats more food than the peasants gather and starves
	const int kInsaneSoldiersPerPeasant = 3;
	int soldiers = (pikeType != 0xFFFF ? city->UnitAmount[pikeType] : 0) + (dragoonType != 0xFFFF ? city->UnitAmount[dragoonType] : 0);
	if (soldiers > city->UnitAmount[nation->UID_PEASANT] * kInsaneSoldiersPerPeasant)trainingProject = 0xFFFF;
	AppendProject(city, projectCount, trainingProject);

	KeepExemptProjectsFirst(city, projectCount);

	city->NBestProjects = projectCount;
	return projectCount;
}

//Fork: one fast rider about every kRaiderTrainPeriod game steps while the next raid lacks riders: the cheapest
//one the AI can train now (Diplomatic Centre mercenaries included), if it costs at most kRaiderCostPercent of
//the stock. The scripts never trained fast cavalry.
static const int kRaiderTrainPeriod = 40;//game steps, about a second
//before minute 20 one rider every 10 s: 60 mercenary Sich Cossacks a minute took the resources of the early army
static const int kRaiderEarlySec = 1200;
extern "C" int GetGameSeconds();
static const int kRaiderTrainPeriodEarly = 400;
static const int kRaiderCostPercent = 20;//as the scripts' other units
static int g_RaiderTrainAt[8];
extern int tmtmt;
static int RaidSize(City* CT);
static int CountRaiders(City* CT);
//Raids leave only above the easiest difficulty (original rule in CheckDiversants). Training riders for them
//and keeping fast cavalry out of the armies follow the same rule: on the easiest level the riders sat at home
//for raids that never left (AI log 2026-10-28 17:59, P4: about 150 riders, no raid)
static bool RaidsEnabled(const City* CT)
{
	return CT->AI_FAST_DIVERSION && CT->Difficulty;
}
static bool AIPeriodDue(int& last, int period);
//The cheapest proposed unit of this Usage costing at most maxPercent of the stock, -1 if none
static int PickUnitProject(City* city, byte usage, int maxPercent)
{
	Nation* nation = city->Nat;
	int best = -1;
	for (int i = 0; i < city->NProp; i++)
	{
		ProposedProject* P = city->Prop + i;
		if (P->PKind != 0 || P->NIndex >= nation->NMon)continue;
		if (nation->Mon[P->NIndex]->newMons->Usage != usage || P->Percent > maxPercent)continue;
		if (best == -1 || P->Percent < city->Prop[best].Percent)best = i;
	}
	return best;
}
//The cheapest proposed upgrade improving units of this Usage costing at most maxPercent of the stock, -1 if none
static int PickUnitUpgrade(City* city, byte usage, int maxPercent)
{
	Nation* nation = city->Nat;
	int best = -1;
	for (int i = 0; i < city->NProp; i++)
	{
		ProposedProject* P = city->Prop + i;
		if (P->PKind != 1 || P->NIndex >= nation->NUpgrades || P->Percent > maxPercent)continue;
		if (best != -1 && P->Percent >= city->Prop[best].Percent)continue;
		NewUpgrade* NU = nation->UPGRADE[P->NIndex];
		if (NU->CtgUpgrade == 11)continue;//unlocks something, does not improve units
		bool fits = false;
		for (word t = 0; t < nation->NMon && !fits; t++)
		{
			if (nation->Mon[t]->newMons->Usage == usage && UpgradeAffectsUnitType(NU, nation, t))fits = true;
		}
		if (fits)best = i;
	}
	return best;
}
static int AddBestProject(City* city, int projectCount, int best)
{
	if (best == -1)return projectCount;
	for (int i = 0; i < projectCount; i++)if (city->BestProj[i] == best)return projectCount;
	city->BestProj[projectCount++] = best;
	return projectCount;
}
static int AddRaiderTraining(City* city, int projectCount)
{
	if (!RaidsEnabled(city) || PeaceTimeLeft || projectCount >= 127)return projectCount;
	int& at = g_RaiderTrainAt[city->NI];
	int period = GetGameSeconds() < kRaiderEarlySec ? kRaiderTrainPeriodEarly : kRaiderTrainPeriod;
	if (tmtmt - at < period && tmtmt >= at)return projectCount;
	if (CountRaiders(city) >= RaidSize(city))return projectCount;
	int best = PickUnitProject(city, FastHorseID, kRaiderCostPercent);
	if (best == -1)return projectCount;
	at = tmtmt;
	return AddBestProject(city, projectCount, best);
}

//Fork: from the second 18th-century barracks on, the AI researches upgrades of its fast cavalry for the raids:
//the cheapest one on offer about every kRaiderTrainPeriod game steps, if it costs at most
//kRaiderUpgradeCostPercent of the stock. The 18th-century barracks are counted as the buildings the century
//upgrade unlocks that train units (their names differ per nation).
static const int kRaiderUpgradeBarracks = 2;//was 3: user request (2026-10-28)
static const int kRaiderUpgradeCostPercent = 50;
static int g_RaiderUpgradeAt[8];
static bool IsCenturyUpgrade(const NewUpgrade* NU);

static int CountCenturyBarracks(City* city)
{
	Nation* nation = city->Nat;
	int n = 0;
	for (int u = 0; u < nation->NUpgrades; u++)
	{
		NewUpgrade* NU = nation->UPGRADE[u];
		if (NU->CtgUpgrade != 11 || !IsCenturyUpgrade(NU))continue;//11 = ENABLE
		int count = NU->UnitGroup ? NU->NUnits : 1;
		for (int i = 0; i < count; i++)
		{
			word type = NU->UnitGroup ? NU->UnitGroup[i] : NU->UnitValue;
			if (type < nation->NMon && nation->Mon[type]->newMons->Building && nation->PACount[type] > 0)
			{
				n += city->UnitAmount[type];
			}
		}
	}
	return n;
}

static int AddRaiderUpgrades(City* city, int projectCount)
{
	if (!RaidsEnabled(city) || projectCount >= 127)return projectCount;
	int& at = g_RaiderUpgradeAt[city->NI];
	if (tmtmt - at < kRaiderTrainPeriod && tmtmt >= at)return projectCount;
	at = tmtmt;
	if (CountCenturyBarracks(city) < kRaiderUpgradeBarracks)return projectCount;
	return AddBestProject(city, projectCount, PickUnitUpgrade(city, FastHorseID, kRaiderUpgradeCostPercent));
}

//Fork: from the third 18th-century barracks the AI trains grenadiers, researches their upgrades and sends squads
//of kGrenSquadSize against enemy mines (HandleGrenadierSquads); while a squad is missing, grenadiers do not
//join the main armies (CheckMinArmyCreationAbility). User request (2026-10-29).
static const int kGrenadierBarracks = 3;
static const int kGrenSquadSize = 25;
static const int kMaxGrenSquads = 2;
static const int kGrenSquadMinAlive = 5;     //fewer left: the squad goes home
static const int kGrenadiersWanted = kGrenSquadSize * kMaxGrenSquads;//trained on top of the scripts while fewer
static const int kGrenTrainPeriod = 80;      //game steps between two grenadiers
static const int kGrenCostPercent = 20;      //as the scripts' other units
static const int kGrenUpgradeCostPercent = 50;
static const int kGrenArmyType = 6;          //ArmType of GRENADER units
static int g_GrenTrainAt[8];
static int g_GrenUpgradeAt[8];
static int g_GrenSquad[8][kMaxGrenSquads];   //brigade index + 1, 0 = none
static int g_GrenSquadAt[8];
static bool GrenadiersEnabled(City* city)
{
	return city->Difficulty && CountCenturyBarracks(city) >= kGrenadierBarracks;
}
static int CountGrenSquads(City* city)
{
	int n = 0;
	for (int s = 0; s < kMaxGrenSquads; s++)if (g_GrenSquad[city->NI][s])n++;
	return n;
}
static bool IsGrenSquadBrigade(int NI, word br)
{
	for (int s = 0; s < kMaxGrenSquads; s++)if (g_GrenSquad[NI][s] && g_GrenSquad[NI][s] - 1 == br)return true;
	return false;
}
static bool GrenSquadsWanted(City* city)
{
	return GrenadiersEnabled(city) && CountGrenSquads(city) < kMaxGrenSquads;
}
static int CountUnitsOfUsage(City* city, byte usage)
{
	int n = 0;
	int N = NtNUnits[city->NI];
	word* units = NatList[city->NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (OB && !OB->Sdoxlo && OB->newMons->Usage == usage)n++;
	}
	return n;
}
static int AddGrenadierTraining(City* city, int projectCount)
{
	if (PeaceTimeLeft || projectCount >= 127 || !GrenadiersEnabled(city))return projectCount;
	if (!AIPeriodDue(g_GrenTrainAt[city->NI], kGrenTrainPeriod))return projectCount;
	if (CountUnitsOfUsage(city, GrenaderID) >= kGrenadiersWanted)return projectCount;
	return AddBestProject(city, projectCount, PickUnitProject(city, GrenaderID, kGrenCostPercent));
}
static int AddGrenadierUpgrades(City* city, int projectCount)
{
	if (projectCount >= 127 || !GrenadiersEnabled(city))return projectCount;
	if (!AIPeriodDue(g_GrenUpgradeAt[city->NI], kRaiderTrainPeriod))return projectCount;
	return AddBestProject(city, projectCount, PickUnitUpgrade(city, GrenaderID, kGrenUpgradeCostPercent));
}

word City::FindNeedProject()
{
	//Groups calculating
	Nation* NT = Nat;
	if (NT->hLibAI && NT->ProcessAIinDLL)
	{
		NBestProjects = 0;
		AiIsRunNow = true;
		CCIT = this;
		CNAT = Nat;
		CurAINation = Nat->NNUM;
		NT->ProcessAIinDLL();
		AiIsRunNow = false;
		static int fnp_dbg_counter = 0;
		if ((fnp_dbg_counter++ % 200) == 0)
			fprintf(stderr, "[AI] FindNeedProject: Nat=%d NBestProjects=%d\n", Nat->NNUM, NBestProjects);
		int n = ApplyInsaneStrategy(this, NBestProjects);
		n = AddRaiderTraining(this, n);
		n = AddRaiderUpgrades(this, n);
		n = AddGrenadierTraining(this, n);
		n = AddGrenadierUpgrades(this, n);
		NBestProjects = n;
		return n;
	};

	int MaxAI = -1;
	for (int i = 0; i < NT->N_AI_Levels; i++)
	{
		bool OK = true;
		int nrq = NT->N_AI_Req[i];
		for (int p = 0; p < nrq; p++)
		{
			sAI_Req* SRQ = &NT->AI_Req[i][p];
			switch (SRQ->Kind)
			{
			case 0://unit
				if (UnitAmount[SRQ->ObjID] < SRQ->Amount)OK = false;
				break;
			case 1://upgrade
				if (!NT->UPGRADE[SRQ->ObjID]->Done)OK = false;
				break;
			case 16://unit cost
				break;
			case 17://upgrade cost
				break;

				/*
								case 2://group
									if(NT->GAmount[SRQ->ObjID]<SRQ->Amount)OK=false;
									break;
				*/
			};

		};
		if (OK)MaxAI = i;
	};
	NT->AI_Level = MaxAI;
	//Let us use current AI method
	//----Checking farms
	/*
	int NFNeed=div(NT->NGidot,15).quot+1;
	if(NFNeed>1)NFNeed++;
	if(NFNeed>5)NFNeed++;
	if(NFNeed>10)NFNeed++;
	int NF=NT->NFarms;
	if(NFNeed>UnitAmount[FARMID]){
		word FPR=PRPIndex[FARMID];
		if(FPR!=0xFFFF)return FPR;
	};
	*/
	if (MaxAI == -1)return 0;
	//------execute the command------
	sAI_Cmd* SCM = NT->AI_Cmd[MaxAI];
	int NCM = NT->N_AI_Cmd[MaxAI];
	MAX_WORKERS = NT->DEFAULT_MAX_WORKERS;
	for (int i = 0; i < NCM; i++)
	{
		switch (SCM->Kind)
		{
		case 1://SELO
			memcpy(NT->SELO.RESP, SCM->Info, 16);
			break;
		case 2://ARMY
			memcpy(NT->ARMY.RESP, SCM->Info, 16);
			break;
		case 3://SCIENCE
			memcpy(NT->SCIENCE.RESP, SCM->Info, 16);
			break;
		case 4://MAX_WORKERS
			MAX_WORKERS = SCM->Info[0];
			break;
		};
		SCM++;
	};
	//----Checking other units by script
	sAI_Devlp* SAD = NT->AI_Devlp[MaxAI];
	word ndev = NT->N_AI_Devlp[MaxAI];
	int npro = 0;
	for (int i = 0; i < ndev; i++)
	{
		sAI_Devlp* sad = &SAD[i];
		if (!sad->Kind)
		{
			//units
			GeneralObject* GO = NT->Mon[sad->ObjID];
			GO->Branch = sad->Source;
			if (sad->Amount > UnitAmount[sad->ObjID])
			{
				word prid = PRPIndex[sad->ObjID];
				if (prid != 0xFFFF)
				{
					//random checking,money checking 
					ProposedProject* PRP = &Prop[prid];
					if (rando() < sad->AtnPercent && sad->GoldPercent >= PRP->Percent)
					{
						if (PresentProject)
						{
							if (!GO->newMons->Building)
							{
								BestProj[npro] = prid;
								npro++;
							};
						}
						else
						{
							BestProj[npro] = prid;
							npro++;
						};
						if (npro >= 127)return npro;
					};
				};
			};
		}
		else
		{

			//upgrades
			word UpgID = sad->ObjID;
			NewUpgrade* NUP = Nat->UPGRADE[UpgID];
			NUP->Branch = sad->Source;
			//random condition checking
			word PropInd = UPGIndex[UpgID];
			ProposedProject* PRP = &Prop[PropInd];
			if (sad->AtnPercent > rando() && sad->GoldPercent >= PRP->Percent)
			{
				//checking the need conditions
				if (PropInd != 0xFFFF)
				{
					//project present
					//ProposedProject* PRP=&Prop[PropInd];
					BestProj[npro] = PropInd;
					npro++;
					if (npro >= 127)return npro;
				};
			};

		};
	};
	return ApplyInsaneStrategy(this, npro);
}

void PerformUpgradeLink(OneObject* OBJ);

//Fork: a finished building that is busy researching is not Ready, but it still counts for requirements
//(long fork upgrades in the Blacksmith/Academy otherwise locked e.g. the 18th century for the whole research)
static bool HasTypeResearching(Nation* Nat, word type)
{
	int N = NtNUnits[Nat->NNUM];
	word* units = NatList[Nat->NNUM];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (OB && !OB->Sdoxlo && OB->NIndex == type && OB->LocalOrder && OB->LocalOrder->DoLink == &PerformUpgradeLink)
		{
			return true;
		}
	}
	return false;
}

//Determine possible updates and abilities
void City::RefreshAbility()
{
	//Let's disable all checked units/upgrades 
	for (int i = 0; i < Nat->NCOND; i++)
	{
		word MID = Nat->CLRef[i][0];
		if (MID < 8192)
		{
			//Monster
			GeneralObject* GO = Nat->Mon[MID];
			GO->Enabled = false;
		}
		else
		{
			Nat->UPGRADE[MID & 8191]->Enabled = false;
		}
	}

	//Let us check it now and enable all allowed monsters/upgrades
	for (int i = 0; i < Nat->NCOND; i++)
	{
		word MID = Nat->CLRef[i][0];
		word NC = Nat->CLSize[i];
		bool enab = true;
		for (int j = 0; j < NC; j++)
		{
			word uc = Nat->CLRef[i][j + 1];
			if (uc < 8192)
			{
				//Monster
				if (!ReadyAmount[uc] && !(UnitAmount[uc] && HasTypeResearching(Nat, uc)))enab = false;
			}
			else
			{
				if (!Nat->UPGRADE[uc - 8192]->Done)enab = false;
			}
		}

		if (enab)
		{
			if (MID < 8192)
			{
				//Monster
				GeneralObject* GO = Nat->Mon[MID];
				if (GO->CondEnabled || GO->newMons->Building)
				{
					GO->Enabled = true;
				}
			}
			else
			{
				NewUpgrade* sutp = Nat->UPGRADE[MID & 8191];
				if (sutp->PermanentEnabled && !sutp->Done)
				{
					sutp->Enabled = true;
				}
			}
		}
	}

	for (int i = 0; i < Nat->NMon; i++)
	{
		GeneralObject* GO = Nat->Mon[i];
		if (GO->ManualDisable)
		{
			GO->Enabled = false;
		}
		else
		{
			if (GO->LockID != 0xFFFF)
			{
				if (UnitAmount[Nat->Mon[i]->LockID] >= Nat->Mon[i]->NLockUnits)
				{
					Nat->Mon[i]->Enabled = false;
				}
			}
		}
	}
}

void RepairControl(City* CT)
{
	byte NI = CT->NI;
	word PS;
	for (int i = 0; i < MAXOBJECT; i++)
	{
		OneObject* OB = Group[i];
		if (OB && OB->capBuilding && (!OB->delay) && OB->Stage == OB->NStages)
		{
			int maxl = OB->MaxLife;
			if (OB->Life < maxl)
			{
				PS = FindPeasant(OB->x, OB->y, NI);
				if (PS != 0xFFFF)
				{
					OneObject* OBP = Group[PS];
					if (OBP)
					{

						OBP->BuildObj(OB->Index, 128, false, 0);
					};
					OB->delay = 200;
					OB->MaxDelay = 200;
				};
			};
			if (OB->Life < (maxl << 1))
			{
				PS = FindPeasant(OB->x, OB->y, NI);
				if (PS != 0xFFFF)
				{
					OneObject* OBP = Group[PS];
					if (OBP)
					{

						OBP->BuildObj(OB->Index, 128, false, 0);
					};
					OB->delay = 200;
					OB->MaxDelay = 200;
				};
			};
		};
	};
};
extern byte NPORTS;
extern short PORTSX[32];
extern short PORTSY[32];
bool SearchTowerPlace(int* xx1, int* yy1, SearchFunction* SFN, int r, int rMin, int xc, int yc, int xe, int ye, bool toEnemy);
//Fork: towers go just outside the base: from kTowerOutside cells beyond its farthest building (not counting mines,
//towers and walls), between kTowerMinRing and kTowerMaxRing cells (128 px) from the Town Centre
static const int kTowerOutside = 2;
static const int kTowerMinRing = 8;
static const int kTowerMaxRing = 25;
static const int kTowerRingSpan = 15;
bool CheckTower(int x, int y);
//Fork: storehouses go where the most wood and stone is not served yet (SearchSkladPlace, NewAI.cpp)
static const int kSkladSearchRadius = 30;//cells (128 px), as the original wood/stone storehouse search
//first within this radius: a big forest at the edge of the search won over a small one by the Town Centre,
//so the first storehouse went far away
static const int kSkladNearRadius = 12;
static const int kMaxDepots = 64;
static bool FindSkladPlace(City* CT, int* xx, int* yy, int* options)
{
	int depX[kMaxDepots];
	int depY[kMaxDepots];
	int nDep = 0;
	int N = NtNUnits[CT->NI];
	word* units = NatList[CT->NI];
	for (int i = 0; i < N && nDep < kMaxDepots; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || OB->Sdoxlo)continue;
		byte use = OB->newMons->Usage;
		if (use != SkladID && use != CenterID)continue;
		depX[nDep] = OB->RealX >> 11;
		depY[nDep] = OB->RealY >> 11;
		nDep++;
	}
	//a resource without a storehouse of its own comes first (wood, then stone): P6 of the 13:09 log had one
	//storehouse at a forest, no stone, so no second Town Centre; the original stone search is skipped here
	int want = !CT->NWoodSklads ? 1 : !CT->NStoneSklads ? 2 : 3;
	int mask = 0;
	bool found = false;
	for (int r : { kSkladNearRadius, kSkladSearchRadius })
	{
		if (SearchSkladPlace(xx, yy, r, depX, depY, nDep, want, &mask) ||
			(want != 3 && SearchSkladPlace(xx, yy, r, depX, depY, nDep, 3, &mask)))
		{
			found = true;
			break;
		}
	}
	if (!found)return false;
	*options = 121 + ((((mask & 1) ? 2 : 0) | ((mask & 2) ? 4 : 0)) << 8);//as below: wood 2, stone 4
	return true;
}

bool City::FindApproximateBuildingPlace(GeneralObject* GO)
{
	NewMonster* NM = GO->newMons;
	bool Res;
	int xx = CenterX;
	int yy = CenterY;
	BPR.Options = 0;
	switch (NM->Usage)
	{
	case MelnicaID:
		Res = SearchPlace(&xx, &yy, &CheckMelnicaWide, 25);
		if (!Res)
		{
			xx = CenterX;
			yy = CenterY;
			Res = SearchPlace(&xx, &yy, &CheckMelnica, 25);
		}
		break;
	case CenterID:
		Res = SearchPlace(&xx, &yy, &CheckStoneWoodSklad, 20);
		if (!Res)
		{
			if (NWoodSklads < NStoneSklads)
			{
				Res = SearchPlace(&xx, &yy, &CheckWoodSklad, 30);
				if (Res)BPR.Options = 121 + (2 << 8);
			}
			else
			{
				Res = SearchPlace(&xx, &yy, &CheckStoneSklad, 30);
				if (Res)BPR.Options = 121 + (4 << 8);
			};
		}
		else
		{
			BPR.Options = 121 + ((2 + 4) << 8);
		};
		if (!Res)
		{
			Res = SearchPlace(&xx, &yy, &CheckBuilding, 30);
		};
		break;
	case SkladID:
		if (FindSkladPlace(this, &xx, &yy, &BPR.Options))
		{
			Res = true;
			break;
		}
		Res = SearchPlace(&xx, &yy, &CheckStoneWoodSklad, 10);
		if (!Res)
		{
			if (NWoodSklads <= NStoneSklads)
			{
				Res = SearchPlace(&xx, &yy, &CheckWoodSklad, 30);
				if (Res)BPR.Options = 121 + (2 << 8);
			}
			else
			{
				Res = SearchPlace(&xx, &yy, &CheckStoneSklad, 30);
				if (Res)BPR.Options = 121 + (4 << 8);
			};
		}
		else
		{
			BPR.Options = 121 + ((2 + 4) << 8);
		};
		break;
	case PortID:
		NPORTS = 0;
		{
			for (int k = 0; k < MAXOBJECT && NPORTS < 32; k++)
			{
				OneObject* OB = Group[k];
				if (OB && (!OB->Sdoxlo) && (OB->Ref.General == GO))
				{
					PORTSX[NPORTS] = OB->RealX >> (4 + 7);
					PORTSY[NPORTS] = OB->RealY >> (4 + 7);
					NPORTS++;
				};
			};
		};
		Res = SearchPlace(&xx, &yy, &CheckPort, 60);
		break;
	case TowerID:
	{
		//Fork: all own towers keep their distance (CheckTower), the enemy side is the average of the enemies'
		//Town Centres (before: player 0's units only), the ring starts at the edge of the base
		int exc = 0;
		int eyc = 0;
		int ne = 0;
		int baseR = 0;
		NPORTS = 0;
		for (int k = 0; k < MAXOBJECT; k++)
		{
			OneObject* OB = Group[k];
			if (!OB || OB->Sdoxlo || !OB->NewBuilding)continue;
			byte use = OB->newMons->Usage;
			if (OB->NNUM == NI)
			{
				if (use == TowerID)
				{
					if (NPORTS < 32)
					{
						PORTSX[NPORTS] = OB->RealX >> (4 + 7);
						PORTSY[NPORTS] = OB->RealY >> (4 + 7);
						NPORTS++;
					}
				}
				else if (!OB->Wall && use != MineID)
				{
					int d = Norma((OB->RealX >> 11) - CenterX, (OB->RealY >> 11) - CenterY);
					if (d > baseR)baseR = d;
				}
			}
			else if (use == CenterID && !(OB->NMask & Nat->NMask))
			{
				ne++;
				exc += OB->RealX >> 11;
				eyc += OB->RealY >> 11;
			}
		}
		if (ne)
		{
			exc /= ne;
			eyc /= ne;
		};
		int rMin = baseR + kTowerOutside;
		if (rMin < kTowerMinRing)rMin = kTowerMinRing;
		if (rMin > kTowerMaxRing)rMin = kTowerMaxRing;
		Res = SearchTowerPlace(&xx, &yy, &CheckTower, rMin + kTowerRingSpan, rMin, CenterX, CenterY, exc, eyc, ne > 0);
	};
	break;
	case FarmID:
	default:
		Res = SearchPlace(&xx, &yy, &CheckBuilding, 40);
		break;
	};
	BPR.AttemptsToFindApprPlace++;
	BPR.AttemptsToStand = 0;
	if (Res)
	{
		BPR.NearX = xx;
		BPR.NearY = yy;
		//assert(BPR.NearY!=17648);
		return true;
	}
	else return false;
};

void City::MarkUnusablePlace()
{
	switch (BPR.Usage)
	{
	case MelnicaID:
		SetUnusable(BPR.NearX, BPR.NearY, CB_Melnica);
		break;
	case CenterID:
		SetUnusable(BPR.NearX, BPR.NearY, CB_Sklad);
		SetUnusable(BPR.NearX, BPR.NearY, CB_Building);
		break;
	case SkladID:
		SetUnusable(BPR.NearX, BPR.NearY, CB_Sklad);
		break;
	case PortID:
		SetUnusable(BPR.NearX, BPR.NearY, CB_Port);
		break;
	default:
		SetUnusable(BPR.NearX, BPR.NearY, CB_Building);
	};
};
int CheckCreationAbility(byte NI, NewMonster* NM, int* x2i, int* y2i, word* BLD, int NBLD);
bool City::FindPreciseBuildingPlace(GeneralObject* GO)
{
	BPR.AttemptsToStand++;
	int xx = (int(BPR.NearX) << 11) + (rando() & 4096) - 2048;
	int yy = (int(BPR.NearY) << 11) + (rando() & 4096) - 2048;
	if (CheckCreationAbility(NI, GO->newMons, &xx, &yy, NULL, 0) != -1)
	{
		BPR.x = xx;
		BPR.y = yy;
		return true;
	};
	return false;
};

void GoToMineLink(OneObject* OB);
int SearchPeasants(int x, int y, bool Workers, byte NI, word* AddTo, int maxpos)
{
	if (x < 0 || y < 0 || x >= VAL_MAXCX - 1 || y >= VAL_MAXCX - 1)return 0;
	int cell = (x + 1) + ((y + 1) << VAL_SHFCX);
	int pos = 0;
	int NMon = MCount[cell];
	if (!NMon)return NULL;
	int ofs1 = cell << SHFCELL;
	word MID;
	int BRID = 0x5432;
	if (!Workers)BRID = NATIONS[NI].CITY->Builders.ID;
	for (int i = 0; i < NMon; i++)
	{
		MID = GetNMSL(ofs1 + i);
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->NNUM == NI && OB->BrigadeID != BRID && (!OB->Sdoxlo) && OB->newMons->Peasant && !OB->DoNotCall/*&&OB->DoWalls*/)
			{
				if (!(OB->LocalOrder && OB->LocalOrder->DoLink == &BuildObjLink))
				{
					if (maxpos)
					{
						AddTo[pos] = OB->Index;
						maxpos--;
						pos++;
						if (!maxpos)return pos;
					};
				};
			};
		};
	};
	return pos;
};
int SearchPeasantsInSquare(int x, int y, int r, byte NI, bool Workers, word* AddOn, int max)
{
	int pos = 0;
	for (int i = 0; i <= r; i++)
	{
		int dx, dy;
		if (r)
		{
			dx = -i;
			for (dy = -i; dy <= i; dy++)
			{
				int dd = SearchPeasants(x + dx, y + dy, Workers, NI, AddOn + pos, max);
				if (dd)
				{
					max -= dd;
					pos += dd;
					if (!max)return pos;
				};
			};
			dx = i;
			for (dy = -i; dy <= i; dy++)
			{
				int dd = SearchPeasants(x + dx, y + dy, Workers, NI, AddOn + pos, max);
				if (dd)
				{
					max -= dd;
					pos += dd;
					if (!max)return pos;
				};
			};
			dy = -i;
			for (dx = -i + 1; dx < i; dx++)
			{
				int dd = SearchPeasants(x + dx, y + dy, Workers, NI, AddOn + pos, max);
				if (dd)
				{
					max -= dd;
					pos += dd;
					if (!max)return pos;
				};
			};
			dy = i;
			for (dx = -i + 1; dx < i; dx++)
			{
				int dd = SearchPeasants(x + dx, y + dy, Workers, NI, AddOn + pos, max);
				if (dd)
				{
					max -= dd;
					pos += dd;
					if (!max)return pos;
				};
			};
		}
		else
		{
			int dd = SearchPeasants(x, y, Workers, NI, AddOn, max);
			max -= dd;
			pos += dd;
			if (!max)return pos;
		};
	};
	return pos;
};
int CreateTeamOfPeasants(byte NNUM, int x, int y, int MinP, int MaxP)
{
	word pids[128];
	int npids = 0;
	if (MaxP > 120)MaxP = 120;
	int xc = x >> 11;
	int yc = y >> 11;
	int pos = SearchPeasantsInSquare(xc, yc, 25, NNUM, false, pids, MaxP);
	if (pos < MinP)pos += SearchPeasantsInSquare(xc, yc, 25, NNUM, true, pids + pos, MinP - pos);
	if (!pos)
	{
		pos = SearchPeasantsInSquare(xc, yc, 200, NNUM, false, pids, MaxP);
		if (pos < MinP)pos += SearchPeasantsInSquare(xc, yc, 200, NNUM, true, pids + pos, MinP - pos);
	};
	if (pos)
	{
		if (Selm[NNUM])
		{
			int ns = NSL[NNUM];
			word* smn = Selm[NNUM];
			word* ssn = SerN[NNUM];
			for (int j = 0; j < ns; j++)
			{
				word MID = smn[j];
				if (MID != 0xFFFF)
				{
					OneObject* OB = Group[MID];
					if (OB->Serial == ssn[j])OB->Selected &= ~GM(OB->NNUM);
				};
			};
			free(Selm[NNUM]);
		};
		if (SerN[NNUM])free(SerN[NNUM]);
		Selm[NNUM] = new word[pos];
		SerN[NNUM] = new word[pos];
		int ps1 = 0;
		for (int i = 0; i < pos; i++)
		{
			OneObject* OB = Group[pids[i]];
			if (OB)
			{
				Selm[NNUM][ps1] = OB->Index;
				SerN[NNUM][ps1] = OB->Serial;
				OB->Selected |= GM(NNUM);
				ps1++;
			};
		};
		NSL[NNUM] = ps1;
	};
	return pos;
};
void City::AddConstruction(OneObject* OB)
{
	if (NConstructions < 64)
	{
		Construction[NConstructions] = OB->Index;
		ConstructionSN[NConstructions] = OB->Serial;
		ConstructionTime[NConstructions] = 95;
		NConstructions++;
	};
};
void City::HandleConstructions()
{
	for (int i = 0; i < NConstructions; i++)
	{
		word Idx = Construction[i];
		word tim = ConstructionTime[i];
		OneObject* OB = Group[Idx];
		bool DeleteThis = false;
		if (OB)
		{
			if (OB->Serial == ConstructionSN[i])
			{
				if (OB->Stage < OB->Ref.General->MoreCharacter->ProduceStages)
				{
					NewMonster* NM = OB->newMons;
					if (NM->Farm && NM->NInFarm)
					{
						FarmsUnderConstruction += NM->NInFarm;
					}
				}
				else
				{
					DeleteThis = true;
				}
			}
			else
			{
				DeleteThis = true;
			}
		}
		else
		{
			DeleteThis = true;
		}

		if (DeleteThis)
		{
			if (i < NConstructions - 1)
			{
				memcpy(Construction + i, Construction + i + 1, (NConstructions - i - 1) << 1);
				memcpy(ConstructionSN + i, ConstructionSN + i + 1, (NConstructions - i - 1) << 1);
				memcpy(ConstructionTime + i, ConstructionTime + i + 1, (NConstructions - i - 1) << 1);
			}
			NConstructions--;
			i--;
		}
		else
		{
			if (ConstructionTime[i]++ > 100)
			{
				word MID = Construction[i];
				if (MID != 0xFFFF)
				{
					OneObject* OB = Group[MID];
					if (OB && OB->Serial == ConstructionSN[i])
					{
						int lNP = 0;
						if (OB->Life < 10)
						{
							//checking if someone wants to build this building
							int nw = 0;
							for (int i = 0; i < MAXOBJECT; i++)
							{
								OneObject* POB = Group[i];
								if (POB && POB->NNUM == NI && POB->newMons->Peasant)
								{
									Order1* OR1 = POB->LocalOrder;
									while (OR1)
									{
										if (OR1->DoLink == &BuildObjLink)
										{
											if (OR1->info.BuildObj.ObjIndex == MID)
											{
												nw++;
											}
											OR1 = NULL;
										}
										else
										{
											OR1 = OR1->NextOrder;
										}
									}
								}
							}
							if (nw < 4)
							{
								lNP = CreateTeamOfPeasants(Nat->NNUM, OB->RealX, OB->RealY, 10, 10);
							}
							else
							{
								lNP = 0;
							}
						}
						else
						{
							lNP = CreateTeamOfPeasants(Nat->NNUM, OB->RealX, OB->RealY, 3, 3);
						}
						if (lNP)
						{
							BuildWithSelected(Nat->NNUM, OB->Index, 1);
							ConstructionTime[i] = 0;
						}
						else
						{
							ConstructionTime[i] = 30;
						}
					}
				}
			}
		}
	}
}

int Norma(int, int);
extern bool SpriteSuccess;
extern int LastSpriteIndex;
#define FieldR1 3
void CHKS();
void City::MakeZasev()
{
	int FieldR = FieldR1;
	if (Nat->NGidot > 200)
	{
		//FieldR+=2;
		BestOnField = 120;
	};
	//search for the field GO
	int N = Nat->NMon;
	int i;
	for (i = 0; i < N && Nat->Mon[i]->newMons->Usage != FieldID; i++);
	if (i < N)
	{
		word FGID = i;
		bool FDone = false;
		for (int i = 0; i < NMeln && !FDone; i++)
		{
			OneObject* OB = Group[MelnID[i]];
			if (OB && OB->Ready && OB->Serial == MelnSN[i])
			{
				int fcx = OB->RealX + 40 * 16;
				int fcy = OB->RealY - 80 * 16;
				for (int dx = -FieldR; dx <= FieldR; dx++)
				{
					for (int dy = -FieldR; dy <= FieldR; dy++)
					{
						int dxx = ((dx + dy) << 10) + 32 * 16;
						int dyy = ((dx - dy) << 10) - 32 * 16;
						SpriteSuccess = false;
						Nat->CreateNewMonsterAt(fcx + dxx, fcy + dyy, FGID, false);
						if (SpriteSuccess)
						{
							if (NFields < 512)
							{
								FieldsID[NFields] = LastSpriteIndex;
								OneSprite* OS = &Sprites[LastSpriteIndex];
								if (OS->Enabled)
								{
									FieldsSN[NFields] = (OS->x >> 5) + ((OS->y >> 5) << 8);
									NFields++;
								};
							};
						};
					};
				};
			};
		};
	};
};
word FIELDID = 0xFFFF;
void City::MakeSlowZasev()
{
	int FieldR = FieldR1;
	//search for the field GO
	int N = Nat->NMon;
	if (FIELDID == 0xFFFF)
	{
		int i;
		for (i = 0; i < N && Nat->Mon[i]->newMons->Usage != FieldID; i++);
		if (i >= N)return;
		FIELDID = i;
	};
	word FGID = FIELDID;
	bool FDone = false;
	for (int i = 0; i < NMeln && !FDone; i++)
	{
		OneObject* OB = Group[MelnID[i]];
		if (OB && OB->Ready && OB->Serial == MelnSN[i])
		{
			int fcx = OB->RealX + 40 * 16;
			int fcy = OB->RealY - 80 * 16;
			for (int dx = -FieldR; dx <= FieldR; dx++)
			{
				for (int dy = -FieldR; dy <= FieldR; dy++)
				{
					if (rando() < 1024)
					{
						int dxx = ((dx + dy) << 10) + 32 * 16;
						int dyy = ((dx - dy) << 10) - 32 * 16;
						SpriteSuccess = false;
						Nat->CreateNewMonsterAt(fcx + dxx, fcy + dyy, FGID, false);
						if (SpriteSuccess)
						{
							if (NFields < 512)
							{
								FieldsID[NFields] = LastSpriteIndex;
								OneSprite* OS = &Sprites[LastSpriteIndex];
								if (OS->Enabled)
								{
									FieldsSN[NFields] = (OS->x >> 5) + ((OS->y >> 5) << 8);
									NFields++;
								};
							};
						};
					};
				};
			};
		};
	};
};
void City::HandleFields()
{
	if (NMeln)
	{
		if ((!NFields) || (NFields < MaxFields - 5))
		{
			//need zasev
			MakeZasev();
			MaxFields = NFields;
		}
		else
		{
			if (rando() < 4096)MakeSlowZasev();
		};
	};
	FieldReady = false;
	if (NFields)
	{
		for (int i = 0; i < NFields; i++)
		{
			bool RemoveField = false;
			OneSprite* OS = &Sprites[FieldsID[i]];
			if (OS->Enabled)
			{
				//int SN=(OS->x>>5)+((OS->y>>5)<<8);
				//if(SN==FieldsSN[i]){
				SprGroup* SG = OS->SG;
				ObjCharacter* OC = &SG->ObjChar[OS->SGIndex];
				if (OC->ResType == FoodID)FieldReady = true;
				//}else RemoveField=true;
			}
			else RemoveField = true;
			if (RemoveField)
			{
				if (i < NFields - 1)
				{
					memcpy(FieldsID + i, FieldsID + i + 1, (NFields - i - 1) << 2);
					memcpy(FieldsSN + i, FieldsSN + i + 1, (NFields - i - 1) << 1);
				}
				NFields--;
				i--;
			}
		}
	}
}

void City::HandleBrigades(int pNP)
{
	CalculateBalance();
	int NN = NeedPF + NeedPW + NeedPS;

	if (!NN)
		return;

	//if(!FieldReady){
		//NeedOnWood=0;
		//NeedOnStone=1;
		//NeedOnField=0;
		//FreePS=NP-1;
	//}else{
	NeedOnWood = (NeedPW * pNP) / NN;
	NeedOnStone = (NeedPS * pNP) / NN;
	NeedOnField = pNP - NeedOnWood - NeedOnStone;
	FreePS = pNP - NN;
	if (XRESRC(Nat->NNUM, FoodID) > 50000)
	{
		NeedOnWood += NeedOnField >> 2;
		NeedOnStone += NeedOnField >> 2;
		NeedOnField -= (NeedOnField >> 2) << 1;
	}
	else
	{
		if (XRESRC(Nat->NNUM, FoodID) > 75000)
		{
			NeedOnWood += NeedOnField >> 1;
			NeedOnStone += NeedOnField >> 1;
			NeedOnField -= (NeedOnField >> 1) << 1;
		}
	}
	if (FreePS < 0)
	{
		FreePS = 0;
	}
	//};
	if (OnField.NMemb > NeedOnField)
	{
		OnField.RemoveObjects(OnField.NMemb - NeedOnField, &Free);
	}
	if (OnStone.NMemb > NeedOnStone)
	{
		OnStone.RemoveObjects(OnStone.NMemb - NeedOnStone, &Free);
	}
	if (OnWood.NMemb > NeedOnWood)
	{
		OnWood.RemoveObjects(OnWood.NMemb - NeedOnWood, &Free);
	}
	if (Free.NMemb)
	{
		if (NeedOnField > OnField.NMemb)
		{
			int NO = NeedOnField - OnField.NMemb;
			if (NO > Free.NMemb)
			{
				NO = Free.NMemb;
			}
			Free.RemoveObjects(NO, &OnField);
		}
		if (NeedOnWood > OnWood.NMemb)
		{
			//int NO=NeedOnWood-OnWood.NMemb;
			if (NeedOnWood > Free.NMemb)
			{
				NeedOnWood = Free.NMemb;
			}
			Free.RemoveObjects(NeedOnWood, &OnWood);
		}
		if (NeedOnStone > OnStone.NMemb)
		{
			//int NO=NeedOnWood-OnStone.NMemb;
			if (NeedOnStone > Free.NMemb)
			{
				NeedOnStone = Free.NMemb;
			}
			Free.RemoveObjects(NeedOnStone, &OnStone);
		}
	}
	OnField.CFN = &CheckFieldWorker;
	OnField.CheckMembers(this);

	OnWood.CFN = &CheckWoodWorker;
	OnWood.CheckMembers(this);

	OnStone.CFN = &CheckStoneWorker;
	OnStone.CheckMembers(this);

	FreeArmy.CFN = &CheckFreeArmy;
	FreeArmy.CheckMembers(this);

	Free.CFN = &CheckFree;
	Free.CheckMembers(this);

	Builders.CFN = &CheckBuilders;
	Builders.CheckMembers(this);
}

void ProduceByAI(byte nat, word Producer, word ID);
void CmdAddBuildObj(byte NI, word ObjID);
int SmartCreationUnit(byte NI, int NIndex, int x, int y);

extern int tmtmt;
int CURRENTAINATION;
byte DefPreBDef[6] = { TowerID,5,6 };
byte* PreBDef = DefPreBDef;
int NPreBDef = 1;

extern byte AINatCountry[8];
// FIX (usermission/campaign Bavarian-buildings): remap a proposed building index to the AI nation's
// OWN country variant (same Usage) when the shared AI economy build-list resolved to a foreign
// nation's building (e.g. Poland's AI proposing Bavaria's Europ/Dom/Sklad because the base [PRODUCE]
// list is inherited). No-op for non-AI nations, same-country buildings, or when the nation has no
// building of its own with that Usage. Runtime-confirmed via [ARCH-BUILD]: Nat=1(Poland,country 9)
// was building 'Europ(BA)'/'Dom_Prussia(BA)'/'Sclad1(BA)' (NatID=0) instead of its own.
static word RemapBuildingToNation(Nation* Nat, word idx)
{
	byte country = AINatCountry[Nat->NNUM];
	if (country == 0xFF) return idx;
	GeneralObject* g = Nat->Mon[idx];
	if (!g || !g->newMons || g->NatID == country) return idx;
	byte usage = g->newMons->Usage;
	int N = Nat->NMon;
	bool foundOwnVariant = false;
	for (int j = 0; j < N; j++)
	{
		GeneralObject* gj = Nat->Mon[j];
		if (gj && gj->NatID == country && gj->newMons && gj->newMons->Usage == usage)
		{
			foundOwnVariant = true;
			if (gj->Enabled) return (word)j;
		}
	}

	if (foundOwnVariant) return 0xFFFF;
	return idx;
}

//Fork: AI mine upgrade plan, replacing City::UpgradeMines (whose percent-of-stock rule never fired for the
//expensive levels). Before kMinePlanFullSec only gold mines to level 2 (one, a second after
//kMinePlanSecondGoldSec), paid only when affordable. Then phases, each finished before the next:
//  1. every near gold mine to level 2 (cheap, quick gold)
//  2. the nearest iron mine and the nearest coal mine to level 2
//  3. one near gold mine all the way to the top level
//  4. every other mine, far ones included, to level 2, nearest first
//  5. every mine one level higher, lowest level first (ties: nearest), until all are at the top level
//     (far mines: kFarMineMaxLevel); paid only when affordable, nothing held back (the late levels are
//     expensive and the army comes first)
//"Near" = within the nation's MINE_UPGRADE1_RADIUS of the Town Centre (as in UpgradeMines). While the chosen
//upgrade of phases 1-4 is not affordable, its cost is held back from other AI spending (MineReserveBlocks).
static const int kMineBasicLevel = 1;//upgrades done at "level 2" in the game
static const int kMinePlanSecondGoldSec = 450;//7.5 min: a second gold mine to level 2
static const int kMinePlanFullSec = 900;      //15 min: the full plan, with resources held back for it
extern "C" int GetGameSeconds();//ActiveScenary.cpp: game-step clock, the same on every computer
static const int kMinePlanMaxMines = 64;
//Far mines (more than half the map width from the city centre) are hard to hold and easy prey:
//phase 5 raises them only to level 4 in the game
static const int kFarMineMapDivisor = 2;
static const int kFarMineMaxLevel = 3;//upgrades done

struct MinePlanMine
{
	OneObject* OB;
	int Lev;     //upgrades done (0 = new mine)
	int Res;     //resource the mine produces
	int Dist;    //distance to the Town Centre
	bool Near;
	bool Far;    //beyond kFarMineMapDivisor
	bool Busy;   //upgrading now
	bool CanUp;  //the next upgrade exists and is enabled
};

struct MinePlanResult
{
	const word* Reserve;  //cost to hold back, nullptr = nothing to save for
	bool FullGoldPhase;   //phase 3: the expensive one
};

enum MinePlanStep { MPS_DONE, MPS_WAIT, MPS_TARGET };

//Nearest mine among those to raise to kMineBasicLevel; MPS_WAIT while only busy ones are left
static MinePlanStep MinePlanToBasic(MinePlanMine* M, int NM, bool (*Filter)(const MinePlanMine&), int* target)
{
	bool busy = false;
	int best = -1;
	for (int i = 0; i < NM; i++)
	{
		if (M[i].Lev >= kMineBasicLevel || !Filter(M[i]))continue;
		if (M[i].Busy)
		{
			busy = true;
			continue;
		}
		if (!M[i].CanUp)continue;
		if (best == -1 || M[i].Dist < M[best].Dist)best = i;
	}
	if (best != -1)
	{
		*target = best;
		return MPS_TARGET;
	}
	return busy ? MPS_WAIT : MPS_DONE;
}

static bool MineNearGold(const MinePlanMine& m) { return m.Near && m.Res == GoldID; }
static bool MineAny(const MinePlanMine&) { return true; }

//Phase 2 for one resource: done once any mine of it reached (or is upgrading to) level 2
static MinePlanStep MinePlanOneBasic(MinePlanMine* M, int NM, int res, int* target)
{
	int best = -1;
	for (int i = 0; i < NM; i++)
	{
		if (M[i].Res != res)continue;
		if (M[i].Lev >= kMineBasicLevel || M[i].Busy)return MPS_DONE;
		if (M[i].Near && M[i].CanUp && (best == -1 || M[i].Dist < M[best].Dist))best = i;
	}
	if (best == -1)return MPS_DONE;
	*target = best;
	return MPS_TARGET;
}

static bool MineUpgradeAffordable(Nation* Nat, NewUpgrade* NU)
{
	for (int i = 0; i < 8; i++)
	{
		if (XRESRC(Nat->NNUM, i) < NU->Cost[i])return false;
	}
	return true;
}

static MinePlanResult ForkMinePlan(City* CT)
{
	MinePlanResult R = { nullptr, false };
	Nation* Nat = CT->Nat;
	U_Grp* MUP = &Nat->UGRP_MINEUP;
	if (!MUP->N || !MUP->UIDS)return R;
	if (!Nat->MU1G_PERCENT[0])return R;//the nation script has mine upgrades switched off (StopMineUpgrades)

	MinePlanMine M[kMinePlanMaxMines];
	int NM = 0;
	int cx = CT->CenterX << 11;
	int cy = CT->CenterY << 11;
	int nearDist = Nat->MINE_UPGRADE1_RADIUS << 9;
	int farDist = (msx << 9) / kFarMineMapDivisor;//msx: map width in 32 px cells; Dist is in 1/16 px
	for (MineBase* MBS = (MineBase*)CT->SearchInform(0x4519, NULL); MBS && NM < kMinePlanMaxMines;
		MBS = (MineBase*)CT->SearchInform(0x4519, MBS))
	{
		OneObject* OB = Group[MBS->M_ID];
		if (!OB || OB->Serial != MBS->M_SN || OB->NNUM != Nat->NNUM)continue;
		bool busy = OB->LocalOrder && OB->LocalOrder->DoLink == &PerformUpgradeLink;
		if (!OB->Ready && !busy)continue;//still under construction
		MinePlanMine& m = M[NM++];
		m.OB = OB;
		m.Lev = OB->SingleUpgLevel - 2;
		m.Res = MBS->ResKind;
		m.Dist = Norma(OB->RealX - cx, OB->RealY - cy);
		m.Near = m.Dist < nearDist;
		m.Far = m.Dist > farDist;
		m.Busy = busy;
		m.CanUp = m.Lev >= 0 && m.Lev < MUP->N && MUP->UIDS[m.Lev] != 0xFFFF && Nat->UPGRADE[MUP->UIDS[m.Lev]]->Enabled;
	}

	int target = -1;
	int gameSec = GetGameSeconds();
	if (gameSec < kMinePlanFullSec)
	{//early game: the army comes first. One gold mine to level 2, a second one after kMinePlanSecondGoldSec;
		//nothing is held back for them (upgraded only when the AI can pay right away)
		int allowed = gameSec < kMinePlanSecondGoldSec ? 1 : 2;
		int started = 0;
		for (int i = 0; i < NM; i++)
		{
			if (M[i].Res == GoldID && (M[i].Lev >= kMineBasicLevel || M[i].Busy))started++;
		}
		if (started >= allowed)return R;
		for (int i = 0; i < NM; i++)
		{
			if (MineNearGold(M[i]) && M[i].Lev < kMineBasicLevel && !M[i].Busy && M[i].CanUp &&
				(target == -1 || M[i].Dist < M[target].Dist))target = i;
		}
		if (target == -1)return R;
		word upg = MUP->UIDS[M[target].Lev];
		if (!MineUpgradeAffordable(Nat, Nat->UPGRADE[upg]))return R;
		M[target].OB->PerformUpgrade(upg, M[target].OB->Index);
		return R;
	}
	MinePlanStep step = MinePlanToBasic(M, NM, &MineNearGold, &target);
	if (step == MPS_DONE)step = MinePlanOneBasic(M, NM, IronID, &target);
	if (step == MPS_DONE)step = MinePlanOneBasic(M, NM, CoalID, &target);
	if (step == MPS_DONE)
	{//one near gold mine to the top: the most upgraded one (ties: nearest)
		int best = -1;
		for (int i = 0; i < NM; i++)
		{
			if (!MineNearGold(M[i]))continue;
			if (best == -1 || M[i].Lev > M[best].Lev || (M[i].Lev == M[best].Lev && M[i].Dist < M[best].Dist))best = i;
		}
		if (best != -1 && M[best].Lev < MUP->N)
		{
			if (M[best].Busy)step = MPS_WAIT;
			else if (M[best].CanUp)
			{
				step = MPS_TARGET;
				target = best;
				R.FullGoldPhase = true;
			}
		}
	}
	if (step == MPS_DONE)step = MinePlanToBasic(M, NM, &MineAny, &target);
	if (step == MPS_DONE)
	{//phase 5 (iron and coal stayed at level 2 for the rest of the game before it)
		int best = -1;
		for (int i = 0; i < NM; i++)
		{
			if (M[i].Busy || !M[i].CanUp || (M[i].Far && M[i].Lev >= kFarMineMaxLevel))continue;
			//Fork: far mines last (user request 2026-10-29), then the lowest level, then the nearest
			if (best == -1 || (M[best].Far && !M[i].Far) ||
				(M[best].Far == M[i].Far && (M[i].Lev < M[best].Lev || (M[i].Lev == M[best].Lev && M[i].Dist < M[best].Dist))))best = i;
		}
		if (best == -1)return R;
		word upg = MUP->UIDS[M[best].Lev];
		if (MineUpgradeAffordable(Nat, Nat->UPGRADE[upg]))M[best].OB->PerformUpgrade(upg, M[best].OB->Index);
		return R;
	}
	if (step != MPS_TARGET)return R;

	OneObject* OB = M[target].OB;
	word upg = MUP->UIDS[M[target].Lev];
	NewUpgrade* NU = Nat->UPGRADE[upg];
	for (int i = 0; i < 8; i++)
	{
		if (XRESRC(Nat->NNUM, i) < NU->Cost[i])
		{
			R.Reserve = NU->Cost;
			return R;
		}
	}
	OB->PerformUpgrade(upg, OB->Index);
	R.FullGoldPhase = false;
	return R;
}

//Fork: once kGoldMinersEnough peasants work in gold mines, the AI sells wood and stone for gold only above
//kKeepWoodStone of each. The scripts sold everything above 200,000 for gold, about 10:1 late in the game
//(AI log 2026-10-28 13:54: 202,886 stone -> 20,537 gold), and never kept enough for Town Centres (the most
//expensive European one costs about 510,000) or more Stables and barracks.
static const int kGoldMinersEnough = 150;
static const int kKeepWoodStone = 600000;
static int CountGoldMiners(City* CT)
{
	int n = 0;
	for (MineBase* MBS = (MineBase*)CT->SearchInform(0x4519, NULL); MBS; MBS = (MineBase*)CT->SearchInform(0x4519, MBS))
	{
		OneObject* OB = Group[MBS->M_ID];
		if (OB && OB->Serial == MBS->M_SN && OB->NNUM == CT->Nat->NNUM && MBS->ResKind == GoldID)n += OB->NInside;
	}
	return n;
}

//AI_Torg (ActiveScenary.cpp): the amount of SellRes the AI may sell for BuyRes (engine resource IDs)
//No trades at all while gold, iron, coal and food are all above kRichStock: late in the game the AI traded wood and
//stone away and could not pay for its sixth Town Centre (170,000 wood and stone) or the next barracks (user
//report 2026-10-29)
static const int kRichStock = 100000;
static int* const kRichStockRes[] = { &GoldID, &IronID, &CoalID, &FoodID };
int ForkLimitAISale(City* CT, int SellRes, int BuyRes, int SellAmount)
{
	bool rich = true;
	for (int* res : kRichStockRes)if (XRESRC(CT->Nat->NNUM, *res) < kRichStock)rich = false;
	if (rich)return 0;
	if (BuyRes != GoldID || (SellRes != TreeID && SellRes != StoneID))return SellAmount;
	if (CountGoldMiners(CT) < kGoldMinersEnough)return SellAmount;
	int spare = XRESRC(CT->Nat->NNUM, SellRes) - kKeepWoodStone;
	if (spare <= 0)return 0;
	return SellAmount < spare ? SellAmount : spare;
}

//Upgrade that starts the 18th century: NewUpgrade::Name is "MAIN" + two-letter nation code (as in AILog.cpp)
static const char* const kCenturyUpgradePrefix = "MAIN";
static const size_t kCenturyUpgradeNameLen = 6;

static bool IsCenturyUpgrade(const NewUpgrade* NU)
{
	return NU->Name && strlen(NU->Name) == kCenturyUpgradeNameLen &&
		!strncmp(NU->Name, kCenturyUpgradePrefix, strlen(kCenturyUpgradePrefix));
}

//A building that the 18th century unlocks (the 18c barracks) while the AI has fewer than kEarlyCenturyBuildings:
//the second one waited for the mine reserve and then never became affordable (AI log 2026-10-28 18:33, P4: one
//18c barracks from minute 20 to 40, so its fast cavalry upgrades never started either)
static const int kEarlyCenturyBuildings = 3;//as AI_KAZ18 in the scripts: the first 3 at full priority
static bool IsEarlyCenturyBuilding(City* CT, word type)
{
	Nation* Nat = CT->Nat;
	if (CT->UnitAmount[type] >= kEarlyCenturyBuildings || !Nat->Mon[type]->newMons->Building)return false;
	for (int u = 0; u < Nat->NUpgrades; u++)
	{
		NewUpgrade* NU = Nat->UPGRADE[u];
		if (NU->CtgUpgrade != 11 || !IsCenturyUpgrade(NU))continue;//11 = ENABLE
		if (NU->UnitGroup)
		{
			for (int i = 0; i < NU->NUnits; i++)if (NU->UnitGroup[i] == type)return true;
		}
		else if (NU->UnitValue == type)return true;
	}
	return false;
}

//Spending that the mine reserve never holds back, by the produced unit's Usage
struct MineReserveExemptUsage
{
	byte Usage;
	const char* Reason;
};
static const MineReserveExemptUsage kMineReserveExemptUsages[] =
{
	{ FarmID, "houses: without population capacity the AI stops growing" },
	{ PeasantID, "peasants: they are the workers who fill the mines and earn the reserve" },
};

//Fork: no new soldiers while the food stock would run out within kFoodRunOutSteps at the trend of the last
//kFoodTrendSamples * kFoodSampleSteps. Units die when there is no food: the Insane AI kept training pikemen
//until its stock hit 0 and then lost 45 peasants and 174 pikemen in two minutes, mostly to hunger (AI log
//2026-10-28 13:35, P6). Peasants are still trained: they gather the food.
static const int kFoodSampleSteps = 400;   //10 s
static const int kFoodTrendSamples = 4;    //trend over 40 s
static const int kFoodRunOutSteps = 2400;  //60 s
static const int kFoodEmptyStock = 200;    //below this it is running out whatever the trend (flat at 0 is not falling)
//above this the trend is spending (upgrades, the market), not hunger: P4 stopped training soldiers at 22962
//and 28384 food (AI log 2026-10-28 17:59)
static const int kFoodTrendMaxStock = 10000;
//once stopped, soldiers resume only from this stock: resuming at 200 the next spearmen ate it within 10 s, and
//the Insane AI switched 30 times in 10 minutes with its serfs starving (AI log 2026-10-28 20:17, P6)
static const int kFoodResumeStock = 1000;
struct FoodTrend
{
	int Time;
	int N;
	int Stock[kFoodTrendSamples];
	bool Starving;
};
static FoodTrend g_FoodTrend[8];

static bool FoodRunningOut(City* CT)
{
	FoodTrend& F = g_FoodTrend[CT->NI];
	if (tmtmt < F.Time)memset(&F, 0, sizeof(F));//a new game or a loaded one
	if (tmtmt - F.Time >= kFoodSampleSteps || !F.N)
	{
		F.Time = tmtmt;
		if (F.N < kFoodTrendSamples)F.N++;
		memmove(F.Stock + 1, F.Stock, (kFoodTrendSamples - 1) * sizeof(int));
		F.Stock[0] = XRESRC(CT->NI, FoodID);
		bool was = F.Starving;
		F.Starving = F.Stock[0] < kFoodEmptyStock || (was && F.Stock[0] < kFoodResumeStock);
		if (!F.Starving && F.N == kFoodTrendSamples && F.Stock[0] < kFoodTrendMaxStock)
		{
			int drop = F.Stock[kFoodTrendSamples - 1] - F.Stock[0];
			int span = (kFoodTrendSamples - 1) * kFoodSampleSteps;
			//runs out within kFoodRunOutSteps: Stock[0] / (drop / span) < kFoodRunOutSteps
			F.Starving = drop > 0 && (long long)F.Stock[0] * span < (long long)drop * kFoodRunOutSteps;
		}
#ifdef AILOG_ENABLED
		if (F.Starving != was)AILog_Event(CT->NI, F.Starving ? "FOOD-LOW %d food: no new soldiers" : "FOOD-OK %d food: soldiers again", F.Stock[0]);
#endif
	}
	return F.Starving;
}

static bool MineReserveBlocks(City* CT, const ProposedProject* PRP, const MinePlanResult& plan)
{
	Nation* Nat = CT->Nat;
	int cost[8];
	if (PRP->PKind == 1)
	{
		NewUpgrade* NU = Nat->UPGRADE[PRP->NIndex];
		if (IsCenturyUpgrade(NU))return false;//the 18th century unlocks the AI's late army; never delay it
		for (int i = 0; i < 8; i++)cost[i] = NU->Cost[i];
	}
	else
	{
		if (PRP->NIndex >= Nat->NMon)return false;
		byte usage = Nat->Mon[PRP->NIndex]->newMons->Usage;
		for (const MineReserveExemptUsage& exempt : kMineReserveExemptUsages)
		{
			if (exempt.Usage == usage)return false;
		}
		//buildings are held back only while saving for the top gold mine: the cheap phases blocked
		//the second barracks and Town Hall (they cost gold) and stalled production
		if (!plan.FullGoldPhase && Nat->Mon[PRP->NIndex]->newMons->Building)return false;
		//saving for the top gold mine takes long: the first 18c barracks may still be built meanwhile (kEarlyCenturyBuildings)
		if (plan.FullGoldPhase && IsEarlyCenturyBuilding(CT, PRP->NIndex))return false;
		GetUnitCost(Nat->NNUM, PRP->NIndex, cost);
	}
	for (int i = 0; i < 8; i++)
	{
		if (plan.Reserve[i] && cost[i] && XRESRC(Nat->NNUM, i) - cost[i] < int(plan.Reserve[i]))
		{
#ifdef AILOG_ENABLED
			//once per project and minute of game time (several projects alternate every tick)
			const int kSlots = 32;
			static int loggedKey[8][kSlots];
			static int loggedAt[8][kSlots];
			int key = PRP->PKind * 65536 + PRP->NIndex + 1;
			int slot = key % kSlots;
			if (loggedKey[Nat->NNUM][slot] != key || GetGameSeconds() - loggedAt[Nat->NNUM][slot] >= 60)
			{
				loggedKey[Nat->NNUM][slot] = key;
				loggedAt[Nat->NNUM][slot] = GetGameSeconds();
				const char* name = PRP->PKind == 1 ? Nat->UPGRADE[PRP->NIndex]->Message : Nat->Mon[PRP->NIndex]->Message;
				AILog_Event(Nat->NNUM, "RESERVE-SKIP %s (%s held back for a mine upgrade)", name ? name : "?", AILog_ResName(i));
			}
#endif
			return true;
		}
	}
	return false;
}

//Fork: while the 18th century can be researched, units and upgrades may not spend its gold and iron.
//The AI never got there: mercenaries, Sich Cossacks and mercenary dragoons spent each bit of gold the
//moment it came in, so it never held the 9800 gold the century costs (AI log 2026-10-28 17:59, P4:
//52000 food from minute 11, gold below 1000 for most of the 20 minutes, no 18th century).
//Food is not held back: soldiers eat it, and the scripts already save it with the market (AI_SetRes).
static int* const kCenturyReserveRes[] = { &GoldID, &IronID };

static NewUpgrade* CenturyToSaveFor(Nation* Nat)
{
	for (int u = 0; u < Nat->NUpgrades; u++)
	{
		NewUpgrade* NU = Nat->UPGRADE[u];
		if (NU && IsCenturyUpgrade(NU))return NU->Enabled && !NU->Done && !NU->IsDoing ? NU : nullptr;
	}
	return nullptr;
}

static bool CenturyReserveBlocks(City* CT, const ProposedProject* PRP, const NewUpgrade* century)
{
	Nation* Nat = CT->Nat;
	int cost[8];
	if (PRP->PKind == 1)
	{
		NewUpgrade* NU = Nat->UPGRADE[PRP->NIndex];
		if (NU == century)return false;
		for (int i = 0; i < 8; i++)cost[i] = NU->Cost[i];
	}
	else
	{
		if (PRP->NIndex >= Nat->NMon)return false;
		NewMonster* NM = Nat->Mon[PRP->NIndex]->newMons;
		if (NM->Building || NM->Peasant)return false;//buildings and workers grow the economy that pays for it
		GetUnitCost(Nat->NNUM, PRP->NIndex, cost);
	}
	for (int* res : kCenturyReserveRes)
	{
		int r = *res;
		if (cost[r] && century->Cost[r] && XRESRC(Nat->NNUM, r) - cost[r] < int(century->Cost[r]))
		{
#ifdef AILOG_ENABLED
			static int loggedAt[8];
			if (GetGameSeconds() - loggedAt[Nat->NNUM] >= 60 || GetGameSeconds() < loggedAt[Nat->NNUM])
			{
				loggedAt[Nat->NNUM] = GetGameSeconds();
				const char* name = PRP->PKind == 1 ? Nat->UPGRADE[PRP->NIndex]->Message : Nat->Mon[PRP->NIndex]->Message;
				AILog_Event(Nat->NNUM, "CENTURY-SAVE %s skipped (%s held back for the 18th century: %d of %d)",
					name ? name : "?", AILog_ResName(r), XRESRC(Nat->NNUM, r), int(century->Cost[r]));
			}
#endif
			return true;
		}
	}
	return false;
}

//Fork: the mine and century reserves never hold back soldiers while the army is smaller than the number of
//peasants or the base is under attack. Saving for the 18th century stopped the Russian spearmen (they cost iron)
//for 3 minutes after a lost battle, then the mine reserve held back every soldier for 9 minutes while 130,000
//food, 176,000 wood and 191,000 stone piled up: 17-39 soldiers against 700 (AI log 2026-10-28 20:36, P4)
static bool BaseThreatActive(int NI);
static bool IsSoldierProject(City* CT, const ProposedProject* PRP)
{
	if (PRP->PKind != 0 || PRP->NIndex >= CT->Nat->NMon)return false;
	NewMonster* NM = CT->Nat->Mon[PRP->NIndex]->newMons;
	return !NM->Building && !NM->Peasant;
}
static int CountSoldiers(City* CT)
{
	int n = 0;
	int N = NtNUnits[CT->NI];
	word* units = NatList[CT->NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || OB->Sdoxlo || OB->NewBuilding || OB->Wall)continue;
		NewMonster* NM = OB->newMons;
		if (!NM->Building && !NM->Peasant)n++;
	}
	return n;
}
static bool g_SoldiersFree[8];
static bool SoldiersFreeOfReserves(City* CT)
{
	int soldiers = CountSoldiers(CT);
	bool attacked = BaseThreatActive(CT->NI);
	bool release = soldiers < CT->NPeas || attacked;
#ifdef AILOG_ENABLED
	if (release != g_SoldiersFree[CT->NI])
		AILog_Event(CT->NI, release ? "RESERVE-RELEASE soldiers are no longer held back (%d soldiers, %d peasants%s)"
			: "RESERVE-HOLD soldiers may be held back again (%d soldiers, %d peasants%s)",
			soldiers, int(CT->NPeas), attacked ? ", base attacked" : "");
#endif
	g_SoldiersFree[CT->NI] = release;
	return release;
}

//Process production queues, upgrades, farm growing etc
void City::ProcessCreation()
{
	CURRENTAINATION = NI;

	EnumUnits();

	//Determine possible updates and abilities
	RefreshAbility();

	ExecuteBrigades();

	if (REALTIME - LastTime > 1000)
	{
		int Dt = REALTIME - LastTime;
		WoodSpeed = WoodAmount * 25 / Dt;
		FoodSpeed = FoodAmount * 25 / Dt;
		StoneSpeed = StoneAmount * 25 / Dt;
		LastTime = REALTIME;
		WoodAmount = 0;
		FoodAmount = 0;
		StoneAmount = 0;
	}

	rando();

	//Stop here if it's a human player
	if (!Nat->AI_Enabled)
	{
		return;
	}

	//AI logic past this point
	if (CenterFound)
	{
		SmartGamer(this);
		EngageIdleSoldiers();
		FarmsUnderConstruction = 0;

		HandleConstructions();
		HandleFields();
		HandleBrigades(Free.NMemb + OnField.NMemb + OnWood.NMemb + OnStone.NMemb);

		if (AI_MINES_CPTURE)
		{
			TakeNewMine();
		}
		HandleIdeas();

		//Fork: ForkMinePlan replaces UpgradeMines()
		MinePlanResult minePlan = { nullptr, false };
		if (AI_MINES_UPGRADE)minePlan = ForkMinePlan(this);

		SendAgressors();
		HandleDefending();
		EnumProp();
		word proj = 0xFFFF;
		int NGID = Nat->NGidot;
		int NF = Nat->NFarms + FarmsUnderConstruction;
		bool NeedFarm = false;
		if (!(NF == 0 && NGID < 10))
		{
			if (NGID + 1 >= NF)
			{
				NeedFarm = true;
			}
			else
			{
				if (NGID > 10 && NF - NGID < 7)
					NeedFarm = true;
				if (NGID > 20 && NF - NGID < 10)
					NeedFarm = true;
				if (NGID > 30 && NF - NGID < 20)
					NeedFarm = true;
				if (NGID > 50 && NF - NGID < 30)
					NeedFarm = true;
				if (NGID > 60 && NF - NGID < 40)
					NeedFarm = true;
				if (NGID > 80 && NF - NGID < 60)
					NeedFarm = true;
				// Fork: large AI populations keep a bigger reserve of free population capacity
				const int BigPopulation = 500;
				const int BigPopulationFreeCapacity = 80;
				if (NGID > BigPopulation && NF - NGID < BigPopulationFreeCapacity)
					NeedFarm = true;
			}
			if (NeedFarm && !PresentProject)
			{
				for (int i = 0; i < NProp; i++)
				{
					if (Prop[i].PKind == 0 && Nat->Mon[Prop[i].NIndex]->newMons->Usage == FarmID)
					{
						proj = i;
						i = NProp;
					}
				}
			}
		}
		if (!AI_DEVELOPMENT)
		{
			proj = 0xFFFF;
		}
		int NPR = 0;
		if (proj == 0xFFFF)
		{
			if (AI_DEVELOPMENT)
				NPR = FindNeedProject();
		}
		else
		{
			NPR = 1;
			BestProj[0] = proj;
		}
		bool foodRunningOut = FoodRunningOut(this);
		NewUpgrade* century = CenturyToSaveFor(Nat);
		bool soldiersFree = (minePlan.Reserve || century) && SoldiersFreeOfReserves(this);
		for (int k = 0; k < NPR; k++)
		{
			proj = BestProj[k];
			ProposedProject* PRP = &Prop[proj];
			bool reservesApply = !(soldiersFree && IsSoldierProject(this, PRP));
			if (reservesApply && minePlan.Reserve && MineReserveBlocks(this, PRP, minePlan))
			{
				continue;
			}
			if (reservesApply && century && CenturyReserveBlocks(this, PRP, century))
			{
				continue;
			}
			if (foodRunningOut && PRP->PKind == 0 && PRP->NIndex < Nat->NMon)
			{
				NewMonster* PNM = Nat->Mon[PRP->NIndex]->newMons;
				if (!PNM->Building && !PNM->Peasant)continue;
			}
			if (PRP->PKind == 1)
			{
				//upgrade
				OneObject* POB = Group[PRP->ProducerIndex];
				if (POB)
				{
					POB->PerformUpgrade(PRP->NIndex, PRP->ProducerIndex);
				}
			}
			else
			{
				PRP->NIndex = RemapBuildingToNation(Nat, PRP->NIndex); // FIX Bavarian-buildings
				if (PRP->NIndex == 0xFFFF) continue; // mission-forbidden for this nation's own building variant
				GeneralObject* IGO = Nat->Mon[PRP->NIndex];
				NewMonster* INM = IGO->newMons;
				if (!INM->Building)
				{
					OneObject* POB = Group[PRP->ProducerIndex];
					if (POB)
					{
						ProduceByAI(NI, POB->Index, PRP->NIndex);
					}
				}
				else
				{
					if (!PresentProject)
					{
						if (PRP->NIndex == Nat->UID_HOUSE)
						{
							BPR.Used = true;
							PresentProject = true;
							BPR.NIndex = PRP->NIndex;
							BPR.PlaceFound = false;
							BPR.PeasantsCalled = false;
							BPR.AttemptsToFindApprPlace = 0;
							BPR.AttemptsToStand = 0;
							BPR.Usage = IGO->newMons->Usage;
						}
						else
						{
							if (FindApproximateBuildingPlace(IGO))
							{
								BPR.Used = true;
								PresentProject = true;
								BPR.NIndex = PRP->NIndex;
								BPR.PlaceFound = false;
								BPR.PeasantsCalled = false;
								BPR.AttemptsToFindApprPlace = 0;
								BPR.AttemptsToStand = 0;
								BPR.Usage = IGO->newMons->Usage;
							}
						}
					}
				}
			}
		}

		if (PresentProject)
		{
			GeneralObject* IGO = Nat->Mon[BPR.NIndex];
			if (BPR.NIndex == Nat->UID_HOUSE)
			{
				int fx, fy;
				int IDX;
				int NN = 0;
				do
				{
					GetNextPlaceForFarm(&fx, &fy);
					IDX = SmartCreationUnit(Nat->NNUM, BPR.NIndex, fx << 4, fy << 4);
					NN++;
				} while (NN < 5 && IDX == -1);
				if (IDX != -1)
				{
					//need to send peasants
					OneObject* OB = Group[IDX];
					NewMonster* INM = IGO->newMons;
					BPR.Index = IDX;
					AddConstruction(OB);
#ifdef AILOG_ENABLED
					AILog_Event(Nat->NNUM, "BUILD %s", IGO->Message);
#endif
					int lNP = CreateTeamOfPeasants(Nat->NNUM, OB->RealX, OB->RealY, 2, 3);
					if (lNP)
					{
						BuildWithSelected(Nat->NNUM, BPR.Index, 1);
					}
					BPR.Used = false;
					PresentProject = false;
				};
			}
			else
			{
				if (!BPR.PlaceFound)
				{
					for (int F = 0; F < 1 && PresentProject && !BPR.PlaceFound; F++)
					{
						int xx = (int(BPR.NearX) << 11) + (rando() & 4096) - 2048;
						int yy = (int(BPR.NearY) << 11) + (rando() & 4096) - 2048;
						int IDX = SmartCreationUnit(Nat->NNUM, BPR.NIndex, xx, yy);
						if (IDX >= 0)
						{
							if (BPR.Usage == MelnicaID)
							{
								if (MaxFields)MaxFields += 20;
								int xx = BPR.NearX;
								int yy = BPR.NearY;
								for (int dx = -6; dx <= 6; dx++)
								{
									for (int dy = -6; dy <= 6; dy++)
									{
										//Fork: CB_Farm too: houses were built on the mill's field area
										SetUnusable(xx + ((dx + dy) >> 1), yy + ((dx - dy) >> 1), CB_Melnica | CB_Sklad | CB_Building | CB_Farm);
									};
								};
							};
							BPR.PlaceFound = true;
#ifdef AILOG_ENABLED
							AILog_Event(Nat->NNUM, "BUILD %s", IGO->Message);
#endif
							OneObject* OB = Group[IDX];
							NewMonster* INM = IGO->newMons;
							BPR.Index = IDX;
							AddConstruction(OB);
							if (INM->Usage == SkladID || INM->Usage == CenterID)
							{
								if ((BPR.Options & 255) == 121)
								{
									if (BPR.Options & 512)
									{
										//wood
										if (NWoodSklads < 8)
										{
											WoodSkladID[NWoodSklads] = IDX;
											WoodSkladSN[NWoodSklads] = OB->Serial;
											NWoodSklads++;
										};
									}
									else
									{
										bool UnFND = true;
										for (int i = 0; i < 8 && UnFND; i++)
										{
											word ID = WoodSkladID[i];
											if (ID != 0xFFFF)
											{
												OneObject* OBJ = Group[ID];
												if (!(OBJ && OBJ->Serial == WoodSkladSN[i]))
												{
													WoodSkladID[i] = IDX;
													WoodSkladSN[i] = OB->Serial;
													UnFND = false;
												};
											};
										};
									};
									if (BPR.Options & 1024)
									{
										//stone
										if (NStoneSklads < 8)
										{
											StoneSkladID[NStoneSklads] = IDX;
											StoneSkladSN[NStoneSklads] = OB->Serial;
											NStoneSklads++;
										}
										else
										{
											bool UnFND = true;
											for (int i = 0; i < 8 && UnFND; i++)
											{
												word ID = StoneSkladID[i];
												if (ID != 0xFFFF)
												{
													OneObject* OBJ = Group[ID];
													if (!(OBJ && OBJ->Serial == StoneSkladSN[i]))
													{
														StoneSkladID[i] = IDX;
														StoneSkladSN[i] = OB->Serial;
														UnFND = false;
													};
												};
											};
										};
									};
								};
							};
							if (INM->Usage == MelnicaID)
							{
								if (NMeln < 4)
								{
									MelnID[NMeln] = IDX;
									MelnSN[NMeln] = OB->Serial;
									NMeln++;
								};
							};
							//how mutch peasats do we need?
							AdvCharacter* IDC = IGO->MoreCharacter;
							if (IDC->ProduceStages > 800 || INM->Usage == MelnicaID || INM->Usage == CenterID)
							{
								BPR.MaxPeasants = byte(INM->NBuildPt);
								if (INM->NBuildPt > 7)BPR.MinPeasants = byte(INM->NBuildPt - 5);
								else BPR.MinPeasants = BPR.MaxPeasants - 1;
							}
							else
							{
								byte USAGE = IGO->newMons->Usage;
								bool used = 1;
								for (int p = 0; p < NPreBDef; p++)if (USAGE == PreBDef[p + p + p])
								{
									used = 0;
									BPR.MinPeasants = PreBDef[p + p + p + 1];
									BPR.MaxPeasants = PreBDef[p + p + p + 2];
								};
								if (used)
								{

									if (IDC->ProduceStages < 100)
									{
										BPR.MinPeasants = 1;
										BPR.MaxPeasants = 2;
									}
									else
										if (IDC->ProduceStages < 150)
										{
											BPR.MinPeasants = 2;
											BPR.MaxPeasants = 5;
										}
										else
											if (IDC->ProduceStages < 200)
											{
												BPR.MinPeasants = 3;
												BPR.MaxPeasants = 6;
											}
											else
												if (IDC->ProduceStages < 250)
												{
													BPR.MinPeasants = 4;
													BPR.MaxPeasants = 5;
												}
												else
													if (IDC->ProduceStages < 300)
													{
														BPR.MinPeasants = 4;
														BPR.MaxPeasants = 6;
													}
													else
														if (IDC->ProduceStages < 500)
														{
															BPR.MinPeasants = 6;
															BPR.MaxPeasants = 8;
														}
														else
														{
															BPR.MinPeasants = 10;
															BPR.MaxPeasants = 14;
														};
								};
							};

						}
						else
						{
							if (BPR.AttemptsToFindApprPlace > 4)
							{
#ifdef AILOG_ENABLED
								AILog_Event(Nat->NNUM, "BUILD-FAIL %s (no place)", IGO->Message);
#endif
								PresentProject = false;
								BPR.Used = false;
							}
							else
							{
								MarkUnusablePlace();
								if (!FindApproximateBuildingPlace(IGO))
								{
#ifdef AILOG_ENABLED
									AILog_Event(Nat->NNUM, "BUILD-FAIL %s (no place)", IGO->Message);
#endif
									PresentProject = false;
									BPR.Used = false;
								};
							};
						};
					};
				}
				else
				{
					//place found, now we need to call peasants!
					OneObject* OB = Group[BPR.Index];
					if (OB)
					{
						int lNP = CreateTeamOfPeasants(Nat->NNUM, OB->RealX, OB->RealY, BPR.MinPeasants, BPR.MaxPeasants);
						if (lNP)
						{
							BuildWithSelected(Nat->NNUM, BPR.Index, 1);
							BPR.Used = false;
							PresentProject = false;
						}

					}
					else
					{
						BPR.Used = false;
						PresentProject = false;
					}
				}
			}
		}
	}
	else
	{
		int xx, yy;
		CenterFound = FindCenter(&xx, &yy, Nat->NNUM);
		if (CenterFound)
		{
			CenterX = xx;
			CenterY = yy;
		}
	}
}

bool CINFMOD;

void City::HelpMe(word ID)
{
	for (int i = 0; i < 32; i++)
	{
		if (ID == EnemyList[i])return;
	};
	for (int i = 0; i < 32; i++)
	{
		if (EnemyList[i] == 0xFFFF)
		{
			EnemyList[i] = ID;
			return;
		};
	};
	EnemyList[rando() & 31] = ID;
};
word FindObjToAtt()
{
	return 0xFFFF;
};
void City::HandleFly()
{
	for (int i = 0; i < MAXOBJECT; i++)
	{
		OneObject* OB = Group[i];
		if (rando() < 4096 && OB && OB->NNUM == NI && OB->Kind == 5 && !OB->LocalOrder)
		{
			word ww = FindObjToAtt();
			if (ww != 0xFFFF)
				OB->AttackObj(ww, 0);
		};
	};
};
void Branch::AddTo(byte ResID, int Amount)
{
	RESRM[ResID] += Amount * RESP[ResID];
	div_t r = div(RESRM[ResID], 100);
	RESRM[ResID] = r.rem;
	if ((RESAM[ResID] += r.quot) < 0)
	{
		RESAM[ResID] = 0;
		RESRM[ResID] = 0;
	};
};
void Branch::AddPerc(byte ResID, int Amount, int perc)
{
	RESRM[ResID] += Amount * perc;
	div_t r = div(RESRM[ResID], 100);
	RESRM[ResID] = r.rem;
	if ((RESAM[ResID] += r.quot) < 0)
	{
		RESAM[ResID] = 0;
		RESRM[ResID] = 0;
	};
};
void Branch::AddEntire(byte ResID, int Amount)
{
	if ((RESAM[ResID] += Amount) < 0)RESAM[ResID] = 0;

};
void Branch::Check(byte NI)
{
	//int* RS=&RESRC[NI][0];
	for (int i = 0; i < 8; i++)if (RESAM[i] > XRESRC(NI, i))RESAM[i] = XRESRC(NI, i);
};
void Branch::Init()
{
	memset(this, 0, sizeof(Branch));
};
int Branch::GetMonsterCostPercent(byte NI, word NIndex)
{
	int COST[8];
	GetUnitCost(NI, NIndex, COST);
	int maxp = 0;
	//int* RSS=RESRC[NI];
	for (int i = 0; i < 8; i++)
	{
		int RP = XRESRC(NI, i);//RESAM[i];
		if (RP)
		{
			int per = div(COST[i] * 100, RP).quot;
			if (!per)per = 1;
			if (per > maxp)maxp = per;
		};
	};
	if (!maxp)return 101;
	return maxp;
};
int Branch::GetUpgradeCostPercent(byte NI, word NIndex)
{
	byte* LSTATE = CITY[NI].LockUpgrade;
	word* COST = NATIONS[NI].UPGRADE[NIndex]->Cost;
	int maxp = 0;
	//int* RSS=RESRC[NI];
	for (int i = 0; i < 8; i++)
	{
		int RP = XRESRC(NI, i);//RESAM[i];
		if (RP)
		{
			int per = div(int(COST[i]) * 100, RP).quot;
			if (LSTATE[i] && COST[i] > 600)per = 1000;
			if (!per)per = 1;
			if (per > maxp)maxp = per;
		};
	};
	if (!maxp)return 101;
	return maxp;
};
void Nation::AddResource(byte Rid, int Amount)
{
	SELO.AddTo(Rid, Amount);
	ARMY.AddTo(Rid, Amount);
	SCIENCE.AddTo(Rid, Amount);
	GENERAL.AddTo(Rid, Amount);

};
void Nation::ControlProduce(byte Branch, byte ResID, int Amount)
{
	switch (Branch)
	{
	case 0://SELO
		SELO.AddEntire(ResID, Amount);
		GENERAL.AddEntire(ResID, Amount);
		break;
	case 1:
		ARMY.AddEntire(ResID, Amount);
		GENERAL.AddEntire(ResID, Amount);
		break;
	case 2:
		SCIENCE.AddEntire(ResID, Amount);
		GENERAL.AddEntire(ResID, Amount);
		break;
	case 255:
		if (GENERAL.RESAM[ResID])
		{
			int pp = GENERAL.RESAM[ResID];;
			SELO.AddPerc(ResID, Amount, div(SELO.RESAM[ResID] * 100, pp).quot);
			ARMY.AddPerc(ResID, Amount, div(ARMY.RESAM[ResID] * 100, pp).quot);
			SCIENCE.AddPerc(ResID, Amount, div(SCIENCE.RESAM[ResID] * 100, pp).quot);
		};
		GENERAL.AddTo(ResID, Amount);
		break;
	};

};

Idea* City::AddIdea(IdeaBrain* IBR, bool Duplicate)
{
	Idea* ID;
	if (IDEA && !Duplicate)
	{
		ID = IDEA;
		while (ID)
		{
			if (ID->Brain == IBR)
			{
				return NULL;
			};
			ID = ID->NextIdea;
		};
	};
	if (!IDEA)
	{
		ID = new Idea;
		IDEA = ID;
		ID->PrevIdea = NULL;
	}
	else
	{
		ID = IDEA;
		while (ID->NextIdea)ID = ID->NextIdea;
		ID->NextIdea = new Idea;
		ID->NextIdea->PrevIdea = ID;
		ID = ID->NextIdea;
	};
	ID->NextIdea = NULL;
	ID->Brain = IBR;
	ID->IdeaData = NULL;
	ID->DataSize = 0;
	ID->CT = this;
	ID->FI = NULL;
	return ID;
}

void Idea::ClearIdea()
{
	if (PrevIdea)
	{
		PrevIdea->NextIdea = NextIdea;
		if (NextIdea)NextIdea->PrevIdea = PrevIdea;
	}
	else
	{
		CT->IDEA = NextIdea;
		if (NextIdea)NextIdea->PrevIdea = NULL;
	};
	if (FI)FI(this);
	free(this);
}

void City::DelIdeas()
{
	while (IDEA)
	{
		Idea* NID = IDEA->NextIdea;
		IDEA->ClearIdea();
		IDEA = NID;
	}
}

void City::HandleIdeas()
{
	if (IDEA)
	{
		Idea* ID = IDEA;
		do
		{
			Idea* ID1 = ID->NextIdea;
			if (ID->Brain)ID->Brain(ID);
			ID = ID1;
		} while (ID);
	}
}

const byte BMIND[40] = { 6,6,6,6,6,6,6,6,1,3,
					  4,5,6,6,6,6,6,6,6,6,
					  1,2,1,0,2,6,6,6,6,6,
					  6,6,5,6,6,6,6,6,6,6 };
int GetBMIndex(OneObject* OB)
{
	byte Use = OB->newMons->Usage;
	if (Use < 40)return BMIND[Use];
	else return 6;
};
void EraseBrigade(Brigade* BR);
void MakeStandGround(Brigade* BR);
void CalculateFreeUnits(AI_Army* AIR);
void City::ExecuteBrigades()
{
	try
	{
		// Обработка бригад (как было)
		for (int i = 0; i < MaxBrig; i++)
		{
			Brigade* BRIG = Brigs + i;
			if (BRIG && BRIG->Enabled)
			{
				if (BRIG->BrigDelay && !BRIG->BOrder)
				{
					BRIG->BrigDelay -= FrmDec;
					if (BRIG->BrigDelay <= 0)
						MakeStandGround(BRIG);
				}
				rando();
				if (BRIG->BOrder && BRIG->BOrder->BLink)
				{
					try { BRIG->BOrder->BLink(BRIG); }
					catch (...) {}
				}
				rando();
				if (i < MaxBrig - 11 && BRIG->ArmyID == 0xFFFF && BRIG->NMemb == 0 && !BRIG->BOrder)
				{
					BRIG->DeleteAll();
					BRIG->Enabled = false;
				}
			}
		}

		if (!Nat || !Nat->AI_Enabled)
		{
			for (int i = 0; i < MaxBrig; i++)
			{
				Brigade* BRIG = Brigs + i;
				if (BRIG && BRIG->Enabled)
				{
					word* BMM = (word*)&BRIG->BM;
					if (BRIG->WarType)
					{
						OrderDescription* ODE = ElementaryOrders + BRIG->WarType - 1;
						int NN = BMM[0] + BMM[1] + BMM[2] + BMM[3] + BMM[4] + BMM[5] + BMM[6] + BMM[7];
						if ((NN << 2) < ODE->NUnits)
						{
							EraseBrigade(BRIG);
						}
						else
						{
							if (NN + NN < ODE->NUnits)
							{
								if (!BRIG->ErasureTime)
									BRIG->ErasureTime = 300;
							}
							if (BRIG->ErasureTime)
							{
								if (BRIG->ErasureTime == 1)
									EraseBrigade(BRIG);
								else
									BRIG->ErasureTime--;
							}
						}
					}
				}
			}
		}

		rando();
		// Sequential AI army processing (threading removed for lockstep determinism)
		for (int i = 0; i < MaxArm; ++i)
		{
			AI_Army* AR = ARMS + i;
			if (!AR || !AR->Enabled) continue;
			if (AR->AOrder && AR->AOrder->ALink)
			{
				try { AR->AOrder->ALink(AR); }
				catch (...) {}
			}
			else
			{
				AR->MakeBattle();
			}
			CalculateFreeUnits(AR);
		}
		rando();

	}
	catch (...)
	{
		// Логировать общую ошибку, но не прерывать выполнение игры
	}
}

//-------------------------------IDEAS-----------------------------

void TakeResourceFromSpriteLink(OneObject* OBJ);

OneObject* DetermineMineBySprite(int Spr)
{
	OneSprite* OS = &Sprites[Spr];
	if (OS->Enabled)
	{
		for (int i = 0; i < MAXOBJECT; i++)
		{
			OneObject* OB = Group[i];
			if (OB && OB->newMons->ProdType)
			{
				Order1* OR1 = OB->LocalOrder;
				if (OR1 && OR1->DoLink == &TakeResourceFromSpriteLink)
				{
					if (OR1->info.BuildObj.ObjIndex == Spr)return OB;
				};
			};
		};
		return NULL;
	}
	else return NULL;
};
struct TNM_Data
{
	int NAttempts;
	int Mines[1];

};
int DefaultResTBL[12] = {
	0       ,4,
	0x010201,4,//iron
	0x000100,1,
	0x010101,1,//gold
	0x000101,5,
	0x010202,5 };//coal
int* ResTBL = DefaultResTBL;
int  NInResTBL = 6;
extern int tmtmt;
bool CheckBuildPossibility(byte NI, int x, int y);
static const int kMinGoldBeforeCoal = 2;
static const int kMinIronBeforeCoal = 1;

//Mines of the nation producing resource res, finished or under construction
static int CountMinesByRes(Nation* NT, int res)
{
	int n = 0;
	int N = NtNUnits[NT->NNUM];
	word* units = NatList[NT->NNUM];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (OB && !OB->Sdoxlo && OB->newMons->Usage == MineID && OB->RType == res)n++;
	}
	return n;
}

static const int kLateMineBarracks = 3;
void TakeNewMineBrain(Idea* ID)
{
	City* CT = ID->CT;
	bool GoodIdea = 1;
	if (CT->FreePS < CT->Nat->MIN_PBRIG)
	{
		ID->ClearIdea();
		GoodIdea = 0;
		return;
	};
	Nation* NT = CT->Nat;
	TNM_Data* TD = (TNM_Data*)ID->IdeaData;
	int cx = CT->CenterX;
	int cy = CT->CenterY;
	int IDI = -1;
	int StartI = 0;
	int TopC = GetTopology(CT->CenterX << 7, CT->CenterY << 7);
	bool Special = false;
	int ResID = IronID;
	//Fork: counted by the resource each mine produces (the UID_MINE offsets did not match the mine types)
	int NMinFE = CountMinesByRes(NT, IronID);
	int NMinGL = CountMinesByRes(NT, GoldID);
	int NMinCO = CountMinesByRes(NT, CoalID);
	//if(!NMinFE)Special=true;
	int RSM = NMinGL + (NMinFE << 8) + (NMinCO << 16);
	for (int u = 0; u < NInResTBL; u++)if (ResTBL[u + u] == RSM)
	{
		Special = true;
		ResID = ResTBL[u + u + 1];
	};
	//Fork: the scripts' tables never ask for coal; after 2 gold mines and 1 iron mine take a coal mine
	if (NMinCO == 0 && NMinGL >= kMinGoldBeforeCoal && NMinFE >= kMinIronBeforeCoal)
	{
		Special = true;
		ResID = CoalID;
	}

BeginMine:
	IDI = -1;
	int MinDist = 1000000;
	int i;
	for (i = 0; i < NMines; i++)
	{
		int SID = TD->Mines[i];
		if (SID < MaxSprt)
		{
			OneSprite* OS = &Sprites[SID];
			if ((!Special) || (Special && OS->OC->IntResType == ResID))
			{
				if (CheckBuildPossibility(NT->NNUM, OS->x, OS->y) && !GNFO.EINF[CT->NI]->GetSafeVal(OS->x >> 7, OS->y >> 7))
				{
					int dst = Norma(cx - (OS->x >> 7), cy - (OS->y >> 7));
					if (dst < MinDist)
					{
						MinDist = dst;
						IDI = i;
					};
				};
			};
		};
	};
	StartI = i;
	if (IDI != -1)
	{
		//Fork: at least the map width (msx cells of 32 px; MinDist is in 128 px), so the AI also takes the
		//mines of unused starting positions; the nearest safe mine still comes first
		int captureDist = max(NT->MINE_CAPTURE_RADIUS >> 2, msx >> 2);
		//from the third 18th-century barracks: the whole map (its diagonal); far mines are upgraded last and only
		//to level 4 (ForkMinePlan). User request (2026-10-29)
		if (CountCenturyBarracks(CT) >= kLateMineBarracks)captureDist = (msx >> 2) * 3 / 2;
		if (MinDist > captureDist)
		{
			TD->Mines[IDI] = INITBEST;
			return;
		};
		int SID = TD->Mines[IDI];
		OneSprite* OS = &Sprites[SID];
		int Top1 = GetTopology(OS->x, OS->y);
		if (TopC != Top1 && (Top1 >= 0xFFFE || MotionLinks[Top1 + TopC * NAreas] >= 0xFFFE))
		{
			TD->Mines[IDI] = INITBEST;
			return;
		};
		OneObject* OB = DetermineMineBySprite(SID);
		/*
		if(!(OB&&OB->NNUM==ID->CT->NI)){
			if(!GoodIdea)ID->ClearIdea();
			return;
		};
		*/
		word Npeons = CT->Nat->MIN_PBRIG;
		word Ninf = 2;
		word Nstrel = 0;
		word MinWar = 0;
		SendPInform* INF = (SendPInform*)CT->SearchInform(0x1256, SID, NULL);
		if (INF && tmtmt - INF->time > SendPTime)
		{
			CT->DelInform(INF);
			INF = NULL;
		};
		if (INF)
		{
			//CT->DelInform(INF);
			TD->Mines[IDI] = INITBEST;
			return;
		};
		if (OB)
		{
			if (OB->NNUM == NT->NNUM || OB->NNUM == 7)
			{
				int maxi = OB->Ref.General->MoreCharacter->MaxInside + OB->AddInside;
				if (OB->NInside < maxi)
				{
					Npeons = maxi - OB->NInside;
					if (Npeons < 3)
					{
						TD->Mines[IDI] = INITBEST;
						goto BeginMine;
					};
				}
				else
				{
					TD->Mines[IDI] = INITBEST;
					goto BeginMine;
				};
			}
			else
			{
				//
				TD->Mines[IDI] = INITBEST;
				goto BeginMine;
			};
		};
		int N = CT->GetFreeBrigade();
		if (N != -1)
		{
			Brigade* BR = CT->Brigs + N;
			BR->Enabled = true;
			BrigMemb BMEM;
			memset(&BMEM, 0, sizeof(BrigMemb));
			BMEM.Peons = Npeons;
			BMEM.Infantry = Ninf;
			BMEM.Strelkov = Nstrel;
			BR->AddInRadius(OS->x, OS->y, 20000, &BMEM);
			BrigMemb* BM1 = &BR->BM;
			int N1 = BM1->Grenaderov + BM1->Infantry + BM1->Mortir + BM1->Pushek + BM1->Strelkov;
			if (N1 < MinWar || BM1->Peons != Npeons)
			{
				BR->Rospusk();
				BR->DeleteAll();
				BR->Enabled = false;
				TD->Mines[IDI] = INITBEST;
				return;
			};
			BR->CaptureMine(SID, 128, 0);
			//int NN=N-OB->NInside;
			//int N=CreateTeamOfPeasants(OB->NNUM,OB->RealX,OB->RealY,NN,NN);
			SendPInform* IN1 = new SendPInform;
			IN1->ID = 0x1256;
			IN1->time = tmtmt;
			IN1->Size = sizeof(SendPInform);
			IN1->Essence = SID;
			CT->AddInform(IN1, NULL);
			return;
		};
	};
	ID->ClearIdea();
};
void City::TakeNewMine()
{
	if (FreePS < Nat->MIN_PBRIG)
	{
		return;
	}
	if (XRESRC(NI, TreeID) < 100 || XRESRC(NI, StoneID) < 100)
	{
		return;
	}
	Idea* ID = AddIdea(&TakeNewMineBrain, false);
	if (!ID)
	{
		return;
	}
	ID->DataSize = NMines * 4 - 4 + sizeof(TNM_Data);
	ID->IdeaData = malloc(ID->DataSize);
	TNM_Data* TDA = (TNM_Data*)ID->IdeaData;
	memcpy(&TDA->Mines[0], MineList, 4 * NMines);
	TDA->NAttempts = 0;
}

void City::UpgradeSomeMine()
{
}
void City::CreateWallNearMine()
{
}
void City::BuildTower()
{
}
//------------------------------Upgrade mines------------------------//
class UM_Data :public Inform
{
public:
	word N;
	bool Gold;
	byte Ulevel;
	word Sids[10];
};
void PerformUpgradeLink(OneObject* OBJ);
void UpgradeMineBrain(Idea* ID)
{
	UM_Data* UM = (UM_Data*)ID->IdeaData;
	int N = UM->N;
	int best = -1;
	bool Gold = UM->Gold;
	MineBase* BBase = NULL;
	int Mindis = 100000;
	City* CT = ID->CT;
	//if(CT->UnitAmount[CT->Nat->UID_PEASANT]<320)return;
	int cx = CT->CenterX << 7;
	int cy = CT->CenterY << 7;
	byte Lev = UM->Ulevel;
	int udst = CT->Nat->MINE_UPGRADE1_RADIUS << 5;
	for (int i = 0; i < N; i++)
	{
		word ess = UM->Sids[i];
		if (ess != 0xFFFF)
		{
			MineBase* MB = (MineBase*)CT->SearchInform(0x4519, ess, NULL);
			if (MB)
			{
				OneObject* OB = Group[MB->M_ID];
				if (OB && OB->Serial == MB->M_SN)
				{
					if (OB->SingleUpgLevel <= Lev + 2)
					{
						if ((!Gold) || (Gold && MB->ResKind == GoldID))
						{
							int dst = Norma(cx - (OB->RealX >> 4), cy - (OB->RealY >> 4));
							if (dst < Mindis && dst < udst)
							{
								Mindis = dst;
								best = i;
								BBase = MB;
							};
						};
					};
				};
			};
		};
	};
	if (best == -1)
	{
		if (Gold)UM->Gold = false;
		else ID->ClearIdea();
		return;
	};
	UM->Sids[best] = (unsigned short)(-1);
	OneObject* OB = Group[BBase->M_ID];
	if (OB->LocalOrder && OB->LocalOrder->DoLink == &PerformUpgradeLink)return;
	CT->N_mineUp[OB->SingleUpgLevel - 2]++;
	OB->PerformUpgrade(CT->Nat->UGRP_MINEUP.UIDS[OB->SingleUpgLevel - 2], OB->Index);

	ID->ClearIdea();
};
int MUtbl[10] = { 100,100,80,60,50,40,30,30,20,10 };
int GetMineMult(int n)
{
	if (n < 10)return MUtbl[n];
	else return MUtbl[9];
}
int GetMaxCostPercent(byte Nat, word* Cost)
{
	int maxp = 0;
	//int * res=RESRC[Nat];
	for (int i = 0; i < 8; i++)
	{
		if (Cost[i])
		{
			int p = XRESRC(Nat, i) ? (int(Cost[i]) * 100) / XRESRC(Nat, i) : 1000;
			if (p > maxp)maxp = p;
		};
	};
	return maxp;
};
void City::UpgradeMines()
{
	//Idea* ID1=AddIdea(&UpgradeMineBrain,false);
	//if(!ID1)return;
	//if(UnitAmount[Nat->UID_PEASANT]<280)return;
	MineBase* MBS = (MineBase*)SearchInform(0x4519, NULL);
	int MaxLev = 100;
	int N = 0;
	int cx = CenterX << 11;
	int cy = CenterY << 11;
	int udst = Nat->MINE_UPGRADE1_RADIUS << 9;
	word TotMines[32];
	word MIDX[32];
	int DST[32];
	byte Level[32];
	byte Type[32];
	int NMin = 0;
	//----------------------------//
	while (MBS)
	{
		OneObject* OB = Group[MBS->M_ID];
		if (NMin < 32 && OB && OB->Serial == MBS->M_SN && OB->Ready)
		{
			int D = Norma(OB->RealX - cx, OB->RealY - cy);
			if (D < udst)
			{
				TotMines[NMin] = OB->Index;
				MIDX[NMin] = NMin;
				Level[NMin] = OB->SingleUpgLevel - 2;
				Type[NMin] = MBS->ResKind;
				NMin++;
			};
		};
		MBS = (MineBase*)SearchInform(0x4519, MBS);
	};
	if (!NMin)return;
	UNISORT.CheckSize(NMin);
	memcpy(UNISORT.Uids, MIDX, NMin << 1);
	memcpy(UNISORT.Parms, DST, NMin << 2);
	UNISORT.Sort();
	//upgrading to 3-rd Level
	int NU1 = 0;
	int TotM = 0;

	word* L1PERCENT = Nat->MU1G_PERCENT;
	word* L2PERCENT = Nat->MU2G_PERCENT;
	word* L3PERCENT = Nat->MU3G_PERCENT;
	byte CurRes = GoldID;
	int MaxULevel = 0;
	U_Grp* MUP = &Nat->UGRP_MINEUP;
	for (int q = 0; q < MUP->N; q++)if (Nat->UPGRADE[MUP->UIDS[q]]->Enabled)MaxULevel = q;
	do
	{
		NU1 = 0;
		TotM = 0;
		for (int i = 0; i < NMin; i++)
		{
			if (Type[i] == CurRes)
			{
				TotM++;
				if (Level[i])NU1++;
			};
		};
		if (NU1 != TotM && TotM)
		{
			int p = GetMaxCostPercent(Nat->NNUM, Nat->UPGRADE[Nat->UGRP_MINEUP.UIDS[0]]->Cost);
			if (NU1 >= 2)NU1 = 2;
			if (L1PERCENT[NU1] >= p)
			{
				//need to upgrade
				for (int i = 0; i < NMin; i++)
				{
					if (Level[MIDX[i]] == 0)
					{
						OneObject* OB = Group[TotMines[MIDX[i]]];
						OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[0], OB->Index);
						return;
					};
				};
			};
		}
		else
		{
			//high level upgrade
			int NU = 0;
			for (int j = 0; j < NMin; j++)
			{
				if (Type[MIDX[j]] == CurRes)
				{
					if (Level[MIDX[j]] <= MaxULevel)
					{
						int Lev = Level[MIDX[j]];
						int p;
						for (p = 0; p < j; p++)
						{
							if (Level[MIDX[p]] >= Lev)
							{
								NU++;
							}
						}
						if (NU > 2)
						{
							NU = 2;
						}
						p = GetMaxCostPercent(Nat->NNUM, Nat->UPGRADE[Nat->UGRP_MINEUP.UIDS[Lev]]->Cost);
						if (Lev == 1)
						{
							if (L2PERCENT[NU] >= p)
							{
								OneObject* OB = Group[TotMines[MIDX[j]]];
								OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[Lev], OB->Index);
								return;
							}
						}
						else
						{
							if (Lev >= 2)
							{
								if (L3PERCENT[NU] >= p)
								{
									OneObject* OB = Group[TotMines[MIDX[j]]];
									OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[Lev], OB->Index);
									return;
								}

							}
						}
					}
				}
			}
		}
		if (CurRes == GoldID)
		{
			L1PERCENT = Nat->MU1I_PERCENT;
			L2PERCENT = Nat->MU2I_PERCENT;
			L3PERCENT = Nat->MU3I_PERCENT;
			CurRes = IronID;
		}
		else
		{
			if (CurRes == IronID)
			{
				L1PERCENT = Nat->MU1C_PERCENT;
				L2PERCENT = Nat->MU2C_PERCENT;
				L3PERCENT = Nat->MU3C_PERCENT;
				CurRes = CoalID;
			}
			else
			{
				CurRes = 0xFF;
			}
		}
	} while (CurRes != 0xFF);
}



bool FindNearestWall(int* x, int* y, byte NI)
{
	int xx = (*x) >> 6;
	int yy = (*y) >> 6;
	int MyTop = GetTopology(*x, *y);
	if (MyTop > NAreas)return false;
	int MTPS = MyTop * NAreas;
	int MinDis = 100000;
	int BestTop = 0xFFFF;
	EnemyInfo* EIN = GNFO.EINF[NI];
	if (!EIN)return false;
	for (int i = 0; i < EIN->NEnWalls; i++)
	{
		int xw = EIN->WallsX[i];
		int yw = EIN->WallsY[i];
		int Ofs0 = xw + (yw << TopSH);
		word Tp_U = 0xFFFF;
		word Tp_D = 0xFFFF;
		word Tp_L = 0xFFFF;
		word Tp_R = 0xFFFF;
		if (xw > 0 && yw > 0 && xw < TopLx - 1 && yw < TopLy - 1)
		{
			Tp_U = TopRef[Ofs0 - TopLx];
			Tp_D = TopRef[Ofs0 + TopLx];
			Tp_L = TopRef[Ofs0 - 1];
			Tp_R = TopRef[Ofs0 + 1];
		};
		int cds = 0xFFFF;
		int LocBestTop = 0xFFFF;
		if (Tp_U < 0xFFFE)
		{
			if (Tp_U == MyTop)
			{
				cds = 0;
				LocBestTop = Tp_U;
			}
			else
			{
				int dst = LinksDist[MTPS + Tp_U];
				if (dst < cds)
				{
					cds = dst;
					LocBestTop = Tp_U;
				};
			};
		};
		if (Tp_D < 0xFFFE)
		{
			if (Tp_D == MyTop)
			{
				cds = 0;
				LocBestTop = Tp_D;
			}
			else
			{
				int dst = LinksDist[MTPS + Tp_D];
				if (dst < cds)
				{
					cds = dst;
					LocBestTop = Tp_D;
				};
			};
		};
		if (Tp_L < 0xFFFE)
		{
			if (Tp_L == MyTop)
			{
				cds = 0;
				LocBestTop = Tp_L;
			}
			else
			{
				int dst = LinksDist[MTPS + Tp_L];
				if (dst < cds)
				{
					cds = dst;
					LocBestTop = Tp_L;
				};
			};
		};
		if (Tp_R < 0xFFFE)
		{
			if (Tp_R == MyTop)
			{
				cds = 0;
				LocBestTop = Tp_R;
			}
			else
			{
				int dst = LinksDist[MTPS + Tp_R];
				if (dst < cds)
				{
					cds = dst;
					LocBestTop = Tp_R;
				};
			};
		};
		if (cds < MinDis)
		{
			MinDis = cds;
			BestTop = LocBestTop;
		};
		if (!MinDis)
		{
			Area* AR = TopMap + BestTop;
			*x = int(AR->x) << 6;
			*y = int(AR->y) << 6;
			return true;
		};
	};
	if (BestTop != 0xFFFF)
	{
		Area* AR = TopMap + BestTop;
		*x = int(AR->x) << 6;
		*y = int(AR->y) << 6;
		return true;
	};
	return false;
};

void City::SendAgressors()
{
	int N = Agressors.NMemb;

	if (N > NAgressors)
	{
		if (N < 10)
		{
			int N1 = GetFreeBrigade();
			if (N1 != -1)
			{
				Brigade* BR = &Brigs[N1];
				BR->Enabled = true;
				Agressors.RemoveObjects(NAgressors, BR);
				BR->MakeBattle();
				NAgressors += 5000;
				if (NAgressors > 15000)NAgressors = 15000;
			};
		}
		else NAgressors = 15000;
	};
	/*
	if(N>100){
		int ID=GetFreeArmy();
		if(ID!=-1){
			AI_Army* ARM=ARMS+ID;
			ARM->AddBrigade(&Agressors);
			ARM->MakeBattle();
		};
	};
	*/

};
void City::ProtectMine()
{
	PRM_Info* PR1 = (PRM_Info*)SearchInform(0xFF4B, 0, NULL);
	if (PR1)
	{
		if (PR1->NBrigs > 2)return;
	}
	else
	{
		PR1 = new PRM_Info;
		PR1->NBrigs = 0;
		PR1->ID = 0xFF4B;
		PR1->Essence = 0;
		PR1->Size = sizeof(PRM_Info);
		AddInform((Inform*)PR1, NULL);
	};
	int N1 = GetFreeBrigade();
	if (N1 != -1)
	{
		Brigade* BR = Brigs + N1;
		BR->Enabled = true;
		BrigMemb BMEM;
		memset(&BMEM, 0, sizeof(BrigMemb));
		BMEM.Peons = 9;
		BMEM.Infantry = 6;
		BMEM.Strelkov = 2;
		BR->AddInRadius(CenterX << 7, CenterY << 7, 20000, &BMEM);
		BrigMemb* BM1 = &BR->BM;
		if (BM1->Peons != 9 || BM1->Infantry + BM1->Strelkov < 5)
		{
			BR->Rospusk();
			BR->DeleteAll();
			BR->Enabled = false;
			return;
		};
		BR->ProtectFarMines();
		PR1->BrigsID[PR1->NBrigs] = BR->ID;
		PR1->BrigsSN[PR1->NBrigs] = BR->SN;
		PR1->NBrigs++;
	};
};
//Fork: every unit eats, but the scripts' food/wood/stone weights ignore the population: the AI starved
//(food 0, peasants and soldiers dying) after a few minutes. AI logs: one field worker feeds about 8 people.
static const int kPeopleFedPerFoodWorker = 6;          //with a margin
static const int kLowFoodStock = 1000;
static const int kPeopleFedPerFoodWorkerLowStock = 4;  //refill the stock
static const int kHighFoodStock = 3000;                //enough food: the scripts' weights apply unchanged
//In the 18th century, with the mill upgrades done, the scripts' food weight piled up millions of food
//(AI logs: 5-7 million after 80 minutes) while wood and stone ran short: above this stock half of the
//food workers go to wood and stone
static const int kFoodSurplusStock = 200000;
static const int kFoodSurplusWorkerDivisor = 2;

static bool CenturyDone(Nation* Nat)
{
	for (int u = 0; u < Nat->NUpgrades; u++)
	{
		NewUpgrade* NU = Nat->UPGRADE[u];
		if (NU && IsCenturyUpgrade(NU))return NU->Done;
	}
	return false;
}

//At least one food worker per kPeopleFedPerFoodWorker people, taken from the wood and stone quotas
//(which keep at least a quarter of their peasants); fewer food workers on a large surplus
void City::KeepEnoughFoodWorkers()
{
	int food = XRESRC(NI, FoodID);
	if (food > kFoodSurplusStock && CenturyDone(Nat))
	{
		int freed = NeedPF - NeedPF / kFoodSurplusWorkerDivisor;
		int woodStone = NeedPW + NeedPS;
		int toWood = woodStone > 0 ? freed * NeedPW / woodStone : freed;
		NeedPF -= freed;
		NeedPW += toWood;
		NeedPS += freed - toWood;
		return;
	}
	if (food > kHighFoodStock)return;//a surplus took the wood and stone peasants and stopped all building
	int perWorker = food < kLowFoodStock ? kPeopleFedPerFoodWorkerLowStock : kPeopleFedPerFoodWorker;
	int needFood = (Nat->NGidot + perWorker - 1) / perWorker;
	int woodStone = NeedPW + NeedPS;
	int maxFood = NeedPF + woodStone - woodStone / 4;
	if (needFood > maxFood)needFood = maxFood;
	if (NeedPF >= needFood || woodStone <= 0)return;
	int take = needFood - NeedPF;
	int fromWood = take * NeedPW / woodStone;
	int fromStone = take - fromWood;
	if (fromStone > NeedPS)
	{
		fromWood += fromStone - NeedPS;
		fromStone = NeedPS;
	}
	NeedPW -= fromWood;
	NeedPS -= fromStone;
	NeedPF = needFood;
}

//Fork: at least a quarter of the peasants on wood and stone, whatever the weights, split by the script's own
//wood : stone weights (each gets its share). The scripts weigh food 9 : wood 1 : stone 1 until the Academy's
//wood upgrade runs, saving food for it; an AI spending all its food on soldiers (Insane) never afforded the
//upgrade, kept 1-3 peasants on wood and none on stone, could not pay for a Town Centre (it needs stone) and
//collapsed (AI logs 2026-10-28 12:48 and 13:09, P6)
static const int kMinWoodStoneShareDivisor = 4;
static const int kMinStoneShareDivisor = 10;
void City::KeepMinWoodStoneWorkers()
{
	int total = NeedPF + NeedPW + NeedPS;
	int minBoth = total / kMinWoodStoneShareDivisor;
	int pw = Nat->POnWood;
	int ps = Nat->POnStone;
	if (pw + ps <= 0)pw = ps = 1;
	int minWood = minBoth * pw / (pw + ps);
	int minStone = minBoth - minWood;
	//stone at least kMinStoneShareDivisor-th: the early weights (9:4:1) left 2-10 of 100 peasants on stone,
	//the stock stayed near 0 for 10 minutes and stone buildings waited
	if (minStone < total / kMinStoneShareDivisor)minStone = total / kMinStoneShareDivisor;
	int addWood = minWood > NeedPW ? minWood - NeedPW : 0;
	int addStone = minStone > NeedPS ? minStone - NeedPS : 0;
	if (addWood + addStone > NeedPF)
	{//not enough food workers to take from: cut both in proportion
		int avail = NeedPF;
		int sum = addWood + addStone;
		addWood = addWood * avail / sum;
		addStone = avail - addWood;
	}
	NeedPF -= addWood + addStone;
	NeedPW += addWood;
	NeedPS += addStone;
}

//Fork: from the fourth 18th-century barracks the peasants outside the mines aim at kLateFood food, kLateWood wood
//and kLateStone stone workers (fewer: in the same proportion). Mines stay fully staffed: only the peasants
//outside them are counted. A game peaked at 100 on wood, 100 on stone and 35 on food with 1,260 in the mines,
//too little wood and stone for the next Town Centre or barracks (user report 2026-10-29)
static const int kLateWorkerBarracks = 4;
static const int kLateFood = 100;
static const int kLateWood = 250;
static const int kLateStone = 250;
void City::KeepLateGameWorkers()
{
	if (CountCenturyBarracks(this) < kLateWorkerBarracks)return;
	int miners = 0;
	int N = NtNUnits[NI];
	word* units = NatList[NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (OB && !OB->Sdoxlo && OB->NewBuilding && OB->newMons->Usage == MineID && OB->NNUM == NI)miners += OB->NInside;
	}
	int land = NPeas - miners;
	int total = kLateFood + kLateWood + kLateStone;
	if (land <= 0)return;
	if (land > total)land = total;
	NeedPF = kLateFood * land / total;
	NeedPW = kLateWood * land / total;
	NeedPS = land - NeedPF - NeedPW;
}

//Fork: AI soldiers waiting at home (the base groups, not armies on the march) attack the nearest enemy within
//kAIEngageRadius, an attack-move of sorts. On their own they only searched their short radius, so an army
//shooting the base from further away, or standing next to a building that trains units, met no resistance.
//While an enemy army is at the base (SmartGamer, AI_TOWN_DEFENCE: g_BaseThreat), soldiers with nobody in reach
//march to it. Before, only the small FreeArmy group defended; the attack pool (cavalry, archers, mercenaries,
//the Insane AI's pikemen waiting for its big assault) stood still while the base was destroyed.
static const int kAIEngageRadius = 1200;  //px, beyond musket and bow range
static const int kAIEngagePeriod = 16;    //game steps between two checks of one nation
static const int kBaseThreatSteps = 400;  //a threat seen this recently still counts (10 s)
static const int kBaseThreatMapDivisor = 4;//only enemy armies within a quarter of the map width of the Town Centre
static const int kBaseThreatMinUnits = 15; //before: more than 50
//Home soldiers already walking somewhere (back to their post, to the threat) answer an enemy this close.
//They skipped every check while they had an order and walked on while shooters killed them.
static const int kAIEngageWalkingRadius = 600;//px, about musket range
struct BaseThreat
{
	int X, Y;   //px
	int Time;   //tmtmt when seen
	bool Seen;
};
static BaseThreat g_BaseThreat[8];
word SearchVictim(OneObject* OBJ, int R0, int R1);
extern int tmtmt;
static bool BaseThreatActive(int NI)
{
	BaseThreat& T = g_BaseThreat[NI];
	return T.Seen && tmtmt >= T.Time && tmtmt - T.Time < kBaseThreatSteps;
}

//Fork: an enemy army at the base, counted unit by unit: enemy soldiers within kBaseThreatBuildingCells cells of
//a building that stands within a quarter of the map width of the Town Centre. The old check took the armies of
//GlobalArmyInfo (neighbouring occupied cells joined together) and the corners of each one's bounding box: an
//attack joined to the column behind it spanned half the map, its corners were nowhere near the base and
//nobody defended (AI log 2026-10-28 18:33, P6 at 34:28: hundreds of enemies among its buildings, no BASE-THREAT)
static const int kBaseThreatScanPeriod = 32;    //game steps between two scans of one nation
static const int kBaseThreatCellShift = 8;      //256 px cells
static const int kBaseThreatBuildingCells = 6;  //about 1500 px around a building
static bool ScanBaseThreat(City* CT, int* tx, int* ty, int* count)
{
	Nation* Nat = CT->Nat;
	int NI = Nat->NNUM;
	int cx = CT->CenterX << 7;//px
	int cy = CT->CenterY << 7;
	int reach = (msx << 5) / kBaseThreatMapDivisor;//msx: map width in 32 px cells
	int G = ((msx << 5) >> kBaseThreatCellShift) + 1;
	static std::vector<byte> nearBase;
	nearBase.assign(G * G, 0);
	bool any = false;
	int N = NtNUnits[NI];
	word* units = NatList[NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || !OB->NewBuilding || OB->Sdoxlo || OB->Wall)continue;
		int bx = OB->RealX >> 4;
		int by = OB->RealY >> 4;
		if (Norma(bx - cx, by - cy) > reach)continue;
		int gx = bx >> kBaseThreatCellShift;
		int gy = by >> kBaseThreatCellShift;
		for (int y = gy - kBaseThreatBuildingCells; y <= gy + kBaseThreatBuildingCells; y++)
		{
			if (y < 0 || y >= G)continue;
			for (int x = gx - kBaseThreatBuildingCells; x <= gx + kBaseThreatBuildingCells; x++)
			{
				if (x >= 0 && x < G)nearBase[x + y * G] = 1;
			}
		}
		any = true;
	}
	if (!any)return false;
	int n = 0;
	long long sx = 0;
	long long sy = 0;
	for (int e = 0; e < 8; e++)
	{
		if (NATIONS[e].NMask & Nat->NMask)continue;//itself and allies
		int NE = NtNUnits[e];
		word* eu = NatList[e];
		for (int i = 0; i < NE; i++)
		{
			if (eu[i] == 0xFFFF)continue;
			OneObject* OB = Group[eu[i]];
			if (!OB || OB->Sdoxlo || OB->NewBuilding || OB->Wall)continue;
			NewMonster* NM = OB->newMons;
			if (NM->Building || NM->Peasant)continue;
			int x = (OB->RealX >> 4) >> kBaseThreatCellShift;
			int y = (OB->RealY >> 4) >> kBaseThreatCellShift;
			if (x < 0 || y < 0 || x >= G || y >= G || !nearBase[x + y * G])continue;
			n++;
			sx += OB->RealX >> 4;
			sy += OB->RealY >> 4;
		}
	}
	*count = n;
	if (n < kBaseThreatMinUnits)return false;
	*tx = int(sx / n);
	*ty = int(sy / n);
	return true;
}
//Fork: each nation's AI runs on every 8th game step only (Ddex1.cpp: tmtmt % 8 == its number), so a throttle
//like (tmtmt + NI) % 16 never fired for most nations: the Insane AI (P6) never engaged idle soldiers nor scanned
//for base threats. Periodic AI work keeps the time of its last run instead.
static bool AIPeriodDue(int& last, int period)
{
	if (tmtmt - last >= period || tmtmt < last)//tmtmt < last: a new or loaded game
	{
		last = tmtmt;
		return true;
	}
	return false;
}
static int g_EngageAt[8];
static int g_BaseScanAt[8];

void City::EngageIdleSoldiers()
{
	if (!AIPeriodDue(g_EngageAt[NI], kAIEngagePeriod))return;
	BaseThreat& T = g_BaseThreat[NI];
	bool threat = BaseThreatActive(NI);
	int N = NtNUnits[NI];
	word* units = NatList[NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || OB->Sdoxlo || OB->Hidden)continue;//dead or inside
		NewMonster* NM = OB->newMons;
		bool nearBase = Norma((OB->RealX >> 4) - (CenterX << 7), (OB->RealY >> 4) - (CenterY << 7)) <= (msx << 5) / kBaseThreatMapDivisor;
		if (OB->Zombi)
		{
			//led by its army (artillery is aimed by SearchEnemyForAIArtillery); artillery near home also comes to
			//a base threat: cannons and howitzers stayed out of the defence (user report 2026-10-29)
			if (NM->Artilery && threat && nearBase && !OB->LocalOrder)OB->NewMonsterSmartSendTo(T.X, T.Y, 0, 0, 0, 0);
			continue;
		}
		if (NM->Building || NM->Peasant || NM->Officer || NM->Baraban || OB->Wall)continue;
		word br = OB->BrigadeID;
		if (br != 0xFFFF && IsGrenSquadBrigade(NI, br))continue;//led by HandleGrenadierSquads
		if (br != 0xFFFF && br != Agressors.ID && br != FreeArmy.ID && br != Guards.ID && br != Defenders.ID)
		{
			//in an army: one that stands without orders (gathering, the Insane AI's held assault) fights back
			//too; in their brigades the soldiers never look for enemies themselves and stood still while the
			//enemy cut them down (2026-10-28 18:33, P6 at 34:28 and 37:00). Marching or fighting: the army leads.
			if (OB->LocalOrder)continue;
			word close = SearchVictim(OB, 0, kAIEngageWalkingRadius);
			if (close != 0xFFFF)OB->AttackObj(close, 128 + 16, 0);
			else if (threat && nearBase)OB->NewMonsterSmartSendTo(T.X, T.Y, 0, 0, 0, 0);
			continue;
		}
		if (OB->LocalOrder)
		{
			if (OB->EnemyID != 0xFFFF)continue;//already fighting
			word close = SearchVictim(OB, 0, kAIEngageWalkingRadius);
			if (close != 0xFFFF)OB->AttackObj(close, 128 + 16, 0);
			continue;
		}
		word victim = SearchVictim(OB, 0, kAIEngageRadius);
		if (victim != 0xFFFF)OB->AttackObj(victim, 128 + 16, 0);
		//low priority: they fight on the way. Fast cavalry waiting for a raid defends too (it stayed out before)
		else if (threat)OB->NewMonsterSmartSendTo(T.X, T.Y, 0, 0, 0, 0);
	}
}

void City::CalculateBalance()
{
	NPeas = UnitAmount[Nat->UID_PEASANT];
	if (!NPeas)
	{
		NeedPF = 0;
		NeedPW = 0;
		NeedPS = 0;
		return;
	};
	int POnWood = Nat->POnWood;
	int POnFood = Nat->POnFood;
	int POnStone = Nat->POnStone;
	int tot = POnWood + POnStone + POnFood;

	if (Nat->hLibAI && Nat->PBL && tot)
	{
		word nn = Nat->NPBal;
		word* pp = Nat->PBL;
		for (int i = 0; i < nn, pp[0] <= NPeas; pp += 2, i++);
		int NeedOnRes = pp[-1] + (int(NPeas - pp[-2]) * int(pp[1] - pp[-1])) / (pp[0] - pp[-2]);
		NeedPW = (POnWood * NeedOnRes) / tot;
		NeedPS = (POnStone * NeedOnRes) / tot;
		NeedPF = NeedOnRes - NeedPW - NeedPS;
		KeepEnoughFoodWorkers();
		KeepMinWoodStoneWorkers();
		KeepLateGameWorkers();
	};/*else{
		word nn=Nat->NPBal;
		word* pp=Nat->PBalance;
		for(int i=0;i<nn,pp[0]<=NPeas;pp+=4,i++);
		int nF=pp[1];
		int nW=pp[2];
		int nS=pp[3];
		NeedPF=pp[-3]+((int(NPeas-pp[-4])*int(pp[1]-pp[-3]))/(pp[0]-pp[-4]));
		NeedPW=pp[-2]+((int(NPeas-pp[-4])*int(pp[2]-pp[-2]))/(pp[0]-pp[-4]));
		NeedPS=pp[-1]+((int(NPeas-pp[-4])*int(pp[3]-pp[-1]))/(pp[0]-pp[-4]));
	};*/
};
//----------------------Build walls near mines --------------------

class BWM_Idea
{
public:
	word SID;
	MineBase MBAS;
	int Over;
	bool Ready;
};
int wrs = 5;
bool CheckGateUpgrade(OneObject* OB);
bool CheckGateUpXY(int x, int y)
{
	int LI = GetLI(x, y);
	if (LI >= 0 && LI < MaxLI)
	{
		WallCell* WCL = WRefs[LI];
		if (WCL)
		{
			OneObject* OPB = Group[WCL->OIndex];
			if (OPB)
			{
				return CheckGateUpgrade(OPB);
			};
		};
	};
	return false;
}

void CreateGates(OneObject* OB);
void BuildWallLink(OneObject* OB);
void LeaveMineLink(OneObject* OBJ);
void BuildWallBrain(Idea* ID)
{
}

void CreateWallBar(int tx, int ty, int r, City* CT)
{
}

void City::BuildWallsNearMines()
{
}

void FogSpot(int x, int y);
extern int minix;
extern int	miniy;
extern int MiniLx;
extern int MiniLy;
extern int MiniX;
extern int MiniY;
void HandleGeology()
{
	if (NATIONS[NatRefTBL[MyNation]].Geology)
	{
		for (int i = 0; i < NMines; i++)
		{
			int MID = MineList[i];
			OneSprite* OSP = &Sprites[MID];
			if (OSP->Enabled)
			{
				int x = (OSP->x >> (5 + ADDSH)) - MiniX;
				int y = (OSP->y >> (5 + ADDSH)) - MiniY;
				if (x > 0 && x < MiniLx && y>0 && y < MiniLy)
				{
	#ifdef _MSC_VER
				int ofst = int(ScreenPtr) + x + minix + (miniy + y) * ScrWidth;
					__asm {
						push	edi
						mov		edi, ofst
						mov		edx, ScrWidth
						mov		al, 0xFB
						mov[edi], al
						mov[edi + 1], al
						mov[edi - 1], al
						mov[edi + edx], al
						neg     edx
						mov[edi + edx], al
						pop		edi
					};
#else
					byte* ptr = (byte*)ScreenPtr + x + minix + (miniy + y) * ScrWidth;
					ptr[0] = 0xFB;
					ptr[1] = 0xFB;
					ptr[-1] = 0xFB;
					ptr[ScrWidth] = 0xFB;
					ptr[-(int)ScrWidth] = 0xFB;
#endif
				};
			};
		};
	};
};
void City::HandleDefending()
{
	if (rando() < 512)
	{
		//1.Check units 
		for (int i = 0; i < NDefn; i++)
		{
			DefendInfo* DI = DefInf + i;
			if (DI->NDefenders)
			{
				int Minx = int(DI->x) << 12;
				int Miny = int(DI->y) << 12;
				int Maxx = Minx + 4096;
				int Maxy = Miny + 4096;
				word* IDS = DI->Def;
				word* USN = DI->DefSN;
				int N = DI->NDefenders;
				for (int j = 0; j < N; j++)
				{
					OneObject* OB = Group[IDS[j]];
					if (OB && OB->Serial == USN[j])
					{
						//check coordinates
						//Fork: not while it fights or the base is under attack: the walk back is a plain move that
						//ignores enemies, so defenders who chased the attackers turned their backs and were shot down
						if (OB->EnemyID != 0xFFFF || BaseThreatActive(NI))continue;
						if (OB->RealX<Minx || OB->RealX>Maxx || OB->RealY<Miny || OB->RealY>Maxy)
						{
							OB->NewMonsterSendTo(Minx + (rando() & 4095), Miny + (rando() & 4095), 0, 0);
						};
					}
					else
					{
						if (j < N - 1)
						{
							memcpy(IDS + j, IDS + j + 1, (N - j - 1) << 1);
							memcpy(USN + j, USN + j + 1, (N - j - 1) << 1);
						};
						N--;
						DI->NDefenders--;
					};
				};
			};
		};
	};
	if (Defenders.NMemb)
	{
		int N = Defenders.NMemb;
		word* IDS = Defenders.Memb;
		word* MSN = Defenders.MembSN;
		for (int i = 0; i < N; i++)
		{
			OneObject* OB = Group[IDS[i]];
			if (OB && OB->Serial == MSN[i])AddUnitDefender(OB);
		};
		Defenders.RemoveObjects(N, &Guards);
		N = Guards.NMemb;
		IDS = Guards.Memb;
		MSN = Guards.MembSN;
		for (int i = 0; i < N; i++)
		{
			word MID = IDS[i];
			if (MID != 0xFFFF)
			{
				OneObject* OB = Group[MID];
				if (OB && OB->Serial == MSN[i])OB->DoNotCall = true;
			};
		};

	};
	//scanning the territory
	int N = NtNUnits[NI];
	word* UIDS = NatList[NI];
	if (rando() < 300)
	{
		for (int i = 0; i < N; i++)
		{
			OneObject* OB = Group[UIDS[i]];
			if (OB && OB->NewBuilding && OB->NNUM == NI)
			{
				int cx = OB->RealX >> 12;
				int cy = OB->RealY >> 12;
				if (CheckDefending(cx, cy) == -1)
				{
					AddDefending(cx, cy, 1);
				};
			};
		};
	};
};
int City::CheckDefending(byte x, byte y)
{
	for (int i = 0; i < NDefn; i++)
	{
		if (x == DefInf[i].x && y == DefInf[i].y)return i;
	};
	return -1;
};
void City::AddDefending(byte x, byte y, byte Imp)
{
	if (NDefn >= MaxDefn)
	{
		MaxDefn += 32;
		DefInf = (DefendInfo*)realloc(DefInf, MaxDefn * sizeof(DefendInfo));
	};
	DefendInfo* DI = DefInf + NDefn;
	DI->x = x;
	DI->y = y;
	DI->Importance = Imp;
	DI->NDefenders = 0;
	DI->MaxDefs = 0;
	DI->Def = NULL;
	DI->DefSN = NULL;
	NDefn++;
};
void City::AddUnitDefender(OneObject* OB)
{
	if (!NDefn)return;
	int didx = -1;
	int dmin = 100;
	for (int i = 0; i < NDefn; i++)
	{
		if (DefInf[i].NDefenders < dmin)
		{
			didx = i;
			dmin = DefInf[i].NDefenders;
		};
	};
	DefInf[didx].AddUnit(OB);
};
void DefendInfo::AddUnit(OneObject* OB)
{
	if (NDefenders >= MaxDefs)
	{
		MaxDefs += 8;
		Def = (word*)realloc(Def, MaxDefs * 2);
		DefSN = (word*)realloc(DefSN, MaxDefs * 2);
	};
	OB->NoBuilder = true;
	OB->DoNotCall = true;
	OB->DoWalls = false;
	Def[NDefenders] = OB->Index;
	DefSN[NDefenders] = OB->Serial;
	NDefenders++;
};
//------------------------------------ARMY--------------------------------//

byte ArmType[34] = { 0xFF,
				  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,   2,   5,   5,
					 6,0xFF,0xFF,0xFF,   8,0xFF,0xFF,   9,0xFF,   0,
					 1,   3,   7,   4,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
					 1,   1,   1 };
byte ArmSpec[34] = { 0,
				  0,0,0,0,0,2,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0 };

#define MaxBT 10
//0-Light infantry(short range)
//1-infantry(strelok)
//2-fasthorce
//3-hardhorce
//4-horse-strelok
//5-mortira&pushka
//6-grenader
//7-peasant
//8-weak units
//9-supermortira
//word BriMax[MaxBT]={36,16,16,16,16,36,16,16,25,36};
word BriMax[MaxBT] = { 256,256,256,256,256,36,256,16,256,256 };
word BriMin[MaxBT] = { 36 ,16 , 16, 16, 16, 1, 16,16, 25,  4 };
word DefBriMin[MaxBT] = { 4 ,16 , 16, 16, 16, 1, 16,16, 25,  4 };

void AI_Army::InitArmy(City* C)
{
	memset(this, 0, sizeof(AI_Army));
	CT = C;
	NT = C->Nat;
	NI = C->NI;
	LastBitvaTime = -10000;
};
void AI_Army::ClearArmy()
{
	for (int i = 0; i < NExBrigs; i++)
	{
		ExBrigs[i].Brig->DeleteAll();
		ExBrigs[i].Brig->Enabled = false;
	};
	if (ExBrigs)free(ExBrigs);
	InitArmy(CT);
	LastBitvaTime = -10000;
	SpecialOrder = false;
};
static const int kRaidArmyType = 2;//ArmType of FASTHORSE units
int CheckMinArmyCreationAbility(Brigade* BR)
{
	int N = BR->NMemb;
	word* Mem = BR->Memb;
	word NBMEM[MaxBT + 4];
	memset(NBMEM, 0, sizeof(NBMEM));

	for (int i = 0; i < N; i++)
	{
		word MID = Mem[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB)
			{
				int idx = ArmType[OB->Usage];
				if (idx != 0xFF)NBMEM[idx]++;
			};
		};
	};
	if (BR->CT->DefenceStage)
	{
		for (int i = 0; i < MaxBT; i++)
		{
			//Fork: fast cavalry gathers for raids (CheckDiversants) instead of marching with the main armies,
			//also while defending (those armies attack too); at home it still fights (EngageIdleSoldiers)
			if (i == kRaidArmyType && RaidsEnabled(BR->CT))continue;
			if (i == kGrenArmyType && GrenSquadsWanted(BR->CT))continue;
			if (NBMEM[i] >= DefBriMin[i])
			{
				return i;
			};
		};
	}
	else
	{
		for (int i = 0; i < MaxBT; i++)
		{
			if (i == kRaidArmyType && RaidsEnabled(BR->CT))continue;
			if (i == kGrenArmyType && GrenSquadsWanted(BR->CT))continue;
			if (NBMEM[i] >= BriMin[i])
			{
				return i;
			};
		};
	};
	return -1;
};
int CheckSuperMinArmyCreationAbility(Brigade* BR)
{
	int N = BR->NMemb;
	word* Mem = BR->Memb;
	word NBMEM[MaxBT + 4];
	memset(NBMEM, 0, sizeof(NBMEM));
	for (int i = 0; i < N; i++)
	{
		word MID = Mem[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB)
			{
				int idx = ArmType[OB->Usage];
				if (idx != 0xFF)NBMEM[idx]++;
			};
		};
	};
	for (int i = 0; i < MaxBT; i++)
	{
		if (NBMEM[i] >= 1)
		{
			return i;
			//if(i!=5)return i;
			//else if(NBMEM[0]>=8)return i;
		};
	};
	return -1;
};
//FIX



void AI_Army::CreateMinimalArmyFromBrigade(Brigade* BR, int Type)
{
	int N = BR->NMemb;
	if (N <= 0) return;

	int Nu = 0;
	const int NuMax = BriMax[Type];

	std::vector<word> BriInd(MaxBT, 0xFFFF);

	for (int i = 0; i < N; ++i) {
		if (Nu >= NuMax) break;

		word MID = BR->Memb[i];
		if (MID != 0xFFFF) {
			OneObject* OB = Group[MID];
			if (OB) {
				int AT = ArmType[OB->Usage];
				if (AT == Type) {
					word localBriInd = BriInd[AT];
					if (localBriInd != 0xFFFF && ExBrigs[localBriInd].Brig->NMemb >= BriMax[AT]) {
						localBriInd = 0xFFFF;
						BriInd[AT] = 0xFFFF;
					}

					if (localBriInd == 0xFFFF) {
						bool found = false;
						for (int j = 0; j < NExBrigs; ++j) {
							if (ExBrigs[j].BrigadeType == AT && ExBrigs[j].Brig->NMemb < BriMax[AT]) {
								localBriInd = j;
								found = true;
								break;
							}
						}
						if (!found) {
							NExBrigs++;
							if (NExBrigs >= MaxExBrigs) {
								MaxExBrigs += 32;
								ExtendedBrigade* newExBrigs = static_cast<ExtendedBrigade*>(
									realloc(ExBrigs, MaxExBrigs * sizeof(ExtendedBrigade)));
								if (!newExBrigs) return;
								ExBrigs = newExBrigs;
							}
							int BIN = CT->GetFreeBrigade();
							if (BIN == -1) return;
							localBriInd = NExBrigs - 1;
							ExtendedBrigade* EXB = ExBrigs + localBriInd;
							EXB->Brig = CT->Brigs + BIN;
							EXB->Brig->Enabled = 1;
							EXB->BrigadeType = AT;
							EXB->Brig->ArmyID = ArmyID;
							EXB->Force = 0;
							EXB->NeedMembers = 0;
							EXB->NextBrigade = 0;
							BriInd[AT] = localBriInd;
						}
					}

					ExtendedBrigade* EXB = ExBrigs + localBriInd;
					BR->RemoveOne(OB->BrIndex, EXB->Brig);
					OB->Zombi = 1;
					OB->DoNotCall = true;
					OB->NoBuilder = true;
					OB->DoWalls = false;

					Nu++;
				}
			}
		}
	}
}








void AI_Army::AddBrigade(Brigade* BR)
{
	if (BR->WarType)
	{
		if (NExBrigs >= MaxExBrigs)
		{
			MaxExBrigs += 32;
			ExBrigs = static_cast<ExtendedBrigade*>(realloc(ExBrigs, MaxExBrigs * sizeof(ExtendedBrigade)));
			if (!ExBrigs) return; // Проверка на случай неудачного выделения памяти
		}
		ExtendedBrigade* EXB = ExBrigs + NExBrigs;
		EXB->Brig = BR;
		BR->ArmyID = ArmyID;
		EXB->BrigadeType = 0;
		EXB->Force = 0;
		EXB->NeedMembers = 0;
		EXB->NextBrigade = 0;
		NExBrigs++;
		return;
	}

	int N = BR->NMemb;
	if (N <= 0) return;

	std::vector<word> IDS(BR->Memb, BR->Memb + N);
	std::vector<word> BriInd(MaxBT, 0xFFFF);

	for (int i = 0; i < N; ++i)
	{
		word MID = IDS[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB)
			{
				int AT = ArmType[OB->Usage];
				if (AT != 0xFF)
				{
					if (BriInd[AT] != 0xFFFF && ExBrigs[BriInd[AT]].Brig->NMemb >= BriMax[AT])
						BriInd[AT] = 0xFFFF;

					if (BriInd[AT] == 0xFFFF)
					{
						bool found = false;
						for (int j = 0; j < NExBrigs; ++j)
						{
							if (ExBrigs[j].BrigadeType == AT && ExBrigs[j].Brig->NMemb < BriMax[AT] && ExBrigs[j].Brig->WarType == 0)
							{
								BriInd[AT] = j;
								found = true;
								break;
							}
						}
						if (!found)
						{
							NExBrigs++;
							if (NExBrigs >= MaxExBrigs)
							{
								MaxExBrigs += 32;
								ExBrigs = static_cast<ExtendedBrigade*>(realloc(ExBrigs, MaxExBrigs * sizeof(ExtendedBrigade)));
								if (!ExBrigs) return;
							}
							int BIN = CT->GetFreeBrigade();
							if (BIN == -1) return;
							BriInd[AT] = NExBrigs - 1;
							ExtendedBrigade* EXB = ExBrigs + BriInd[AT];
							EXB->Brig = CT->Brigs + BIN;
							EXB->Brig->Enabled = 1;
							EXB->Brig->ArmyID = ArmyID;
							EXB->BrigadeType = AT;
							EXB->Force = 0;
							EXB->NeedMembers = 0;
							EXB->NextBrigade = 0;
						}
					}

					ExtendedBrigade* EXB = ExBrigs + BriInd[AT];
					Brigade* NBR = EXB->Brig;
					BR->RemoveOne(OB->BrIndex, NBR);
					OB->Zombi = 1;
					OB->DoNotCall = true;
					OB->NoBuilder = true;
					OB->DoWalls = false;
					OB->InArmy = 0;
				}
			}
		}
	}
}
ArmyOrder* AI_Army::CreateOrder(byte OrdType, int Size)
{
	ArmyOrder* OR1 = (ArmyOrder*)malloc(Size);
	ArmyOrder* OR2;
	switch (OrdType)
	{
	case 1:
		OR1->Next = AOrder;
		AOrder = OR1;
		break;
	case 2:
		OR1->Next = NULL;
		if (AOrder)
		{
			OR2 = AOrder;
			while (OR2->Next)OR2 = OR2->Next;
			OR2->Next = OR1;
		}
		else AOrder = OR1;
		break;
	default:
		ClearAOrders();
		AOrder = OR1;
		OR1->Next = NULL;
	};
	return OR1;
};
void AI_Army::DeleteAOrder()
{
	try
	{
		if (AOrder)
		{
			ArmyOrder* OR1 = AOrder->Next;
			try
			{
				free(AOrder);
				AOrder = OR1;
			}
			catch (...)
			{
				// Логировать ошибку освобождения памяти
				AOrder = OR1; // Установить следующий заказ, даже если free не удался
			}
		}
	}
	catch (...)
	{
		// Логировать общую ошибку, продолжить выполнение
		AOrder = nullptr; // Сбросить указатель на случай непредвиденной ошибки
	}
}
void AI_Army::ClearAOrders()
{
	while (AOrder)DeleteAOrder();
};
int City::GetFreeArmy()
{
	for (int i = 0; i < MaxArm; i++)
	{
		if (!ARMS[i].Enabled)
		{
			AI_Army* AR = ARMS + i;
			AR->InitArmy(this);
			AR->Enabled = true;
			AR->TopPos = -1;
			AR->ArmyID = i;
			return i;
		};
	};
	return -1;
};
//++++++++++++++f+++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++ LOCAL SEND TO +++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

class ALST_Order :public ArmyOrder
{
public:
	int N;
	word pos[15];//[xi][yi][BrigID][SN][done]
};
__forceinline bool ValidTXY(int x, int y)
{
	int TX = TopLx - 1;
	return x > 0 && y > 0 && x < TX&& y < TX;
};
void CheckTopPos(int xc, int yc, int* xd, int* yd)
{
	int x = *xd;
	int y = *yd;
	int mx = msx >> 1;
	int my = msy >> 1;

	if (xc <= 0)xc = 1;
	if (yc <= 0)yc = 1;
	if (xc >= mx)xc = mx - 1;
	if (yc >= my)yc = my - 1;

	if (x <= 0)x = 1;
	if (y <= 0)y = 1;
	if (x >= mx)x = mx - 1;
	if (y >= my)y = my - 1;

	int sx = xc > x ? -1 : 1;
	int sy = yc > y ? -1 : 1;
	int ax = abs(xc - x);
	int ay = abs(yc - y);
	int xp = xc;
	int yp = yc;
	if (ax > ay)
	{
		int cum = ax >> 1;
		while (xc != x)
		{
			xc += sx;
			cum += ay;
			if (cum >= ax)
			{
				cum -= ax;
				yc += sy;
			};
			int of1 = xc + (yc << TopSH);
			if (ValidTXY(xc, yc) && TopRef[of1] >= 0xFFFE || TopRef[of1 + 1] >= 0xFFFE || TopRef[of1 + TopLx] >= 0xFFFE || TopRef[of1 + TopLx + 1] >= 0xFFFE)
			{
				*xd = xp;
				*yd = yp;
				return;
			};
		};
	}
	else
	{
		int cum = ay >> 1;
		while (yc != y)
		{
			yc += sy;
			cum += ax;
			if (cum >= ay)
			{
				cum -= ay;
				xc += sx;
			};
			int of1 = xc + (yc << TopSH);
			if (ValidTXY(xc, yc) && TopRef[of1] >= 0xFFFE || TopRef[of1 + 1] >= 0xFFFE || TopRef[of1 + TopLx] >= 0xFFFE || TopRef[of1 + TopLx + 1] >= 0xFFFE)
			{
				*xd = xp;
				*yd = yp;
				return;
			};
		};
	};
	*xd = xc;
	*yd = yc;
	return;
};
char* AO_LST = "[AI_Army::LocalSendTo]";
static const int kArmyArrivedShareNum = 3;//3 of 4 brigades arrived end a step of the march
static const int kArmyArrivedShareDen = 4;
bool CheckArmyDanger(AI_Army* ARM);
void ArmyLocalSendToLink(AI_Army* ARM)
{
	if (rando() < 16384)
	{
		if (CheckArmyDanger(ARM))
		{
			ARM->DeleteAOrder();
			ARM->Bitva();
			return;
		};
	};

	rando();
	ALST_Order* AOR = (ALST_Order*)ARM->AOrder;
	word* pos = AOR->pos;
	int N = AOR->N;
	int ps = 0;
	City* CT = ARM->CT;
	bool NoReady = false;
	int NR = 0;
	int RD = 0;
	for (int i = 0; i < N; i++)
	{
		if (!pos[ps + 4])
		{
			Brigade* BR = CT->Brigs + pos[ps + 2];
			if (BR->Enabled)
			{
				if (BR->SN == pos[ps + 3])
				{
					if (BR->BOrder)NR++;
					else RD++;
				};
			};
		};
		ps += 5;
	};
	//Fork: the step ends once kArmyArrivedShare of the brigades are there (before: all of them), so the army does
	//not stop at every area of its way waiting for the slowest brigade (walk-pause-walk, user report 2026-10-29)
	if (!NR || NR * kArmyArrivedShareDen <= (NR + RD) * (kArmyArrivedShareDen - kArmyArrivedShareNum))
	{
		ARM->DeleteAOrder();
		ARM->Parad();
	};
}

void AI_Army::LocalSendTo(int px, int py, byte Prio, byte OrdType)
{
	if (!NExBrigs)
		return;

	px >>= 6;
	py >>= 6;
	int sz = sizeof(ALST_Order) + (NExBrigs - 3) * 10;
	ALST_Order* AOR = (ALST_Order*)CreateOrder(OrdType, sz);
	AOR->N = NExBrigs;
	int odis = 4;
	int Nx = int(sqrt(NExBrigs));
	int Ny = Nx;
	if (Nx * Ny < NExBrigs)Nx++;
	if (Nx * Ny < NExBrigs)Ny++;
	int x0 = px - (((Nx - 1) * odis) >> 1);
	int y0 = py - (((Ny - 1) * odis) >> 1);
	int ps = 0;
	int nn = 0;
	/*
		int xs=0;
		int ys=0;
		int nu=0;
		int nb=NExBrigs;
		for(int j=0;j<nb;j++){
			Brigade* BR=ExBrigs[j].Brig;
			int xb=0;
			int yb=0;
			if(BR->GetCenter(&xb,&yb)){
				xs+=xb;
				ys+=yb;
				nu++;
			};
		};
		int dx=0;
		int dy=0;
		if(nu){
			xs/=nu;
			ys/=nu;
			dx=(x<<6)-xs;
			dy=(y<<6)-ys;
			int N=Norma(dx,dy);
			if(N){
				dx=(dx*200)/N;
				dy=(dy*200)/N;
			};
		};
		*/
	for (int ix = 0; ix < Nx; ix++)
	{
		for (int iy = 0; iy < Ny; iy++)
		{
			if (nn < NExBrigs)
			{
				int x1 = x0 + ix * odis;
				int y1 = y0 + iy * odis;
				CheckTopPos(px, py, &x1, &y1);
				AOR->pos[ps] = x1;
				AOR->pos[ps + 1] = y1;
				Brigade* BR = ExBrigs[nn].Brig;
				AOR->pos[ps + 2] = BR->ID;
				AOR->pos[ps + 3] = BR->SN;
				AOR->pos[ps + 4] = 0;
				if (ExBrigs[nn].BrigadeType != 5)
				{
					BR->LocalSendTo((x1 << 6) + 32, (y1 << 6) + 32, Prio, 0);
				}
				else
				{
					BR->LinearLocalSendTo((x1 << 6) + 32, (y1 << 6) + 32, Prio, 0);
				}
				ps += 5;
				nn++;
			}
		}
	}
	AOR->Message = AO_LST;
	AOR->Prio = 0;
	AOR->Size = sz;
	AOR->ALink = &ArmyLocalSendToLink;
}

void AI_Army::WideLocalSendTo(int px, int py, byte Prio, byte OrdType)
{
	if (!NExBrigs)
		return;

	px >>= 6;
	py >>= 6;
	int sz = sizeof(ALST_Order) + (NExBrigs - 3) * 10;
	ALST_Order* AOR = (ALST_Order*)CreateOrder(OrdType, sz);
	AOR->N = NExBrigs;
	int odis = 4;
	int Nx = int(sqrt(NExBrigs));
	int Ny = Nx;

	if (Nx * Ny < NExBrigs)
		Nx++;

	if (Nx * Ny < NExBrigs)
		Ny++;

	int x0 = px - (((Nx - 1) * odis) >> 1);
	int y0 = py - (((Ny - 1) * odis) >> 1);
	int ps = 0;
	int nn = 0;
	for (int ix = 0; ix < Nx; ix++)
	{
		for (int iy = 0; iy < Ny; iy++)
		{
			if (nn < NExBrigs)
			{
				int x1 = x0 + ix * odis;
				int y1 = y0 + iy * odis;
				CheckTopPos(px, py, &x1, &y1);
				AOR->pos[ps] = x1;
				AOR->pos[ps + 1] = y1;
				Brigade* BR = ExBrigs[nn].Brig;
				AOR->pos[ps + 2] = BR->ID;
				AOR->pos[ps + 3] = BR->SN;
				AOR->pos[ps + 4] = 0;
				BR->WideLocalSendTo((x1 << 6) + 32, (y1 << 6) + 32, Prio, 0);
				ps += 5;
				nn++;
			}
		}
	}
	AOR->Message = AO_LST;
	AOR->Prio = 0;
	AOR->Size = sz;
	AOR->ALink = &ArmyLocalSendToLink;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++ CONNECT TO ARMY ++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
char* ACTA_Message = "[AI_Army::ConnectToArmy]";
class ACTA_Order :public ArmyOrder
{
public:
	int ID;
};
void ArmyConnectToArmyLink(AI_Army* ARM)
{
	rando();
	ACTA_Order* ACO = (ACTA_Order*)ARM->AOrder;
	if (ARM->TopPos == -1)
	{
		ARM->Parad();
		return;
	};
	int ID = ACO->ID;
	AI_Army* MAR = ARM->CT->ARMS + ID;
	if (!MAR->Enabled)
	{
		ARM->DeleteAOrder();
		return;
	};
	if (MAR->TopPos == -1)
	{
		MAR->Parad();
		return;
	};
	int MyTop = ARM->TopPos;
	int HisTop = MAR->TopPos;
	int NextTop = MotionLinks[MyTop * NAreas + HisTop];
	if (HisTop == MyTop || NextTop == HisTop)
	{
		//Can connect now
		ARM->ClearAOrders();
		int N = ARM->NExBrigs;
		for (int i = 0; i < N; i++)
		{
			Brigade* BR = ARM->ExBrigs[i].Brig;
			MAR->AddBrigade(BR);
			if (BR->NMemb)
			{
				BR->RemoveObjects(BR->NMemb, &MAR->CT->Defenders);
			};
			BR->DeleteAll();
			BR->Enabled = false;
		};
		ARM->ClearArmy();
		ARM->Enabled = false;
		return;
	}
	else
	{
		if (NextTop != 0xFFFF)
		{
			//Can move nearer
			Area* NXAR = TopMap + NextTop;
			ARM->LocalSendTo(int(NXAR->x) << 6, int(NXAR->y) << 6, 0, 1);
		};
	};
};
void AI_Army::ConnectToArmy(int ID, byte Prio, byte OrdType)
{
	ACTA_Order* AOR = (ACTA_Order*)CreateOrder(OrdType, sizeof(ACTA_Order));
	AOR->Message = ACTA_Message;
	AOR->Prio = Prio;
	AOR->ID = ID;
	AOR->ALink = &ArmyConnectToArmyLink;
	if (TopPos == -1)Parad();
	if (TopPos == -1)
	{
		DeleteAOrder();
	};
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++++++++ PARAD ++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void AI_Army::Parad()
{
	if (!NExBrigs)return;
	int N = 0;
	int xx = 0;
	int yy = 0;
	for (int i = 0; i < NExBrigs; i++)
	{
		Brigade* BR = ExBrigs[i].Brig;
		if (BR)
		{
			int ax, ay;
			if (BR->GetCenter(&ax, &ay))
			{
				xx += ax;
				yy += ay;
				N++;
			};
		};
	};
	if (N)
	{
		xx /= N;
		yy /= N;
		x = xx;
		y = yy;
		int tp = GetTopology(xx, yy);
		if (tp == 0xFFFF)tp = -1;
		else
		{
			if (TopPos == -1)
			{
				LocalSendTo(xx, yy, 0, 1);
			};
		};
		TopPos = tp;
	};
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++  PROCESSING SIMPLE BATTLE ++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class Army_BTL :public ArmyOrder
{
public:
	int xdest;
	int ydest;
	int time;
};
int GetTopDanger(int Top, byte NI)
{
	if (GNFO.EINF[NI])return 0;
	Area* AR = TopMap + Top;
	int MyX = AR->x >> 2;
	int MyY = AR->y >> 2;
	ArmyInfo* AINF = GNFO.EINF[NI]->GAINF.AINF;
	int Na = GNFO.EINF[NI]->GAINF.NArmy;
	int EnForces = 0;
	for (int i = 0; i < Na; i++)
	{
		if (Norma(int(AINF->MaxX) - MyX, int(AINF->MaxY) - MyY) < 10 ||
			Norma(int(AINF->MaxX) - MyX, int(AINF->MinY) - MyY) < 10 ||
			Norma(int(AINF->MinX) - MyX, int(AINF->MaxY) - MyY) < 10 ||
			Norma(int(AINF->MinX) - MyX, int(AINF->MinY) - MyY) < 10)
		{
			int curforce = AINF->N;//>NSword+AINF->NCaval+(AINF->NStrel<<1);
			EnForces += curforce;
		};
		AINF++;
	};
	return EnForces;
};
int GetTopDanger(int Top, int r, int NI)
{
	if (GNFO.EINF[NI])return 0;
	if (!NAreas)return 0;
	Area* AR = TopMap + Top;
	int MyX = AR->x >> 2;
	int MyY = AR->y >> 2;
	ArmyInfo* AINF = GNFO.EINF[NI]->GAINF.AINF;
	int Na = GNFO.EINF[NI]->GAINF.NArmy;
	int EnForces = 0;
	for (int i = 0; i < Na; i++)
	{
		if (Norma(int(AINF->MaxX) - MyX, int(AINF->MaxY) - MyY) < r ||
			Norma(int(AINF->MaxX) - MyX, int(AINF->MinY) - MyY) < r ||
			Norma(int(AINF->MinX) - MyX, int(AINF->MaxY) - MyY) < r ||
			Norma(int(AINF->MinX) - MyX, int(AINF->MinY) - MyY) < r)
		{
			int curforce = AINF->N;//>NSword+AINF->NCaval+(AINF->NStrel<<1);
			EnForces += curforce;
		};
		AINF++;
	};
	return EnForces;
};
/*
#define AS_ARTILLERY 0x10000
#define AS_SHIPS     0x20000
#define AS_TOWERS    0x40000
#define AS_BATTLE    0x80000
byte GetDangValue(int x,int y);
int FastCheckPosDanger(int x,int y);
DWORD AnalyseArmyPosition(City* CT,AI_Army* ARM,int xp,int yp){
	int MaxDang=0;
	int x=xp>>6;
	int y=yp>>6;
	int odis=4;
	int NExBrigs=ARM->NExBrigs;
	int Nx=sqrt(NExBrigs);
	int Ny=Nx;
	if(Nx*Ny<NExBrigs)Nx++;
	if(Nx*Ny<NExBrigs)Ny++;
	int x0=x-(((Nx-1)*odis)>>1);
	int y0=y-(((Ny-1)*odis)>>1);
	int ps=0;
	int nn=0;
	int Dang=0;
	DWORD State=0;
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			if(nn<NExBrigs){
				int x1=x0+ix*odis;
				int y1=y0+iy*odis;
				CheckTopPos(x,y,&x1,&y1);
				byte D=GetDangValue(x1>>1,y1>>1);
				if(D>128)State|=AS_SHIPS;
				else
				if(D)State|=AS_ARTILLERY;
				//int xx=(x1<<6)+32;
				//int yy=(y1<<6)+32;
				int Dang=FastCheckPosDanger(x1-2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1-2,y1+1);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1+2);
				if(Dang>MaxDang)MaxDang=Dang;
				if(Dang)State|=AS_TOWERS;
				ps+=5;
				nn++;
			};
		};
	};
	int MyX=x>>2;
	int MyY=y>>2;
	int EnForces=0;
	int MaxForce=0;
	int MaxForceX;
	int MaxForceY;
	//int DoBitva=false;
	int rmin=4;
	int Endst=8;
	if(ARM->Spec==2){
		Endst=4;
		rmin=2;
	};
	DWORD DNG0=GetEnemyForce(MyX,MyY,0,rmin);
	DWORD DNG=DNG0+GetEnemyForce(MyX,MyY,rmin+1,Endst);
	if(DNG0)State|=AS_BATTLE;
	int cx=CT->CenterX>>1;
	int cy=CT->CenterY>>1;
	int R=Norma(MyX-cx,MyY-cy);
	if(R<10)DNG>>=3;
	else if(R<25)DNG>>=1;
	if(DNG>65000)DNG=65000;
	if(MaxDang>500)MaxDang=200;
	return State|DNG|(MaxDang<<24);
};
*/
word GetNextGreCell(byte NI, int F, int Start);
word GetNextDivCell(byte NI, int F, int Start);
bool HandleArchers(AI_Army* ARM, int R);
bool CheckArmyDanger(AI_Army* ARM)
{
	bool DoBitva = 0;
	ArmyInfo* AINF = GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	int Na = GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX = (ARM->x) >> 8;
	int MyY = (ARM->y) >> 8;
	int EnForces = 0;
	int MaxForce = 0;
	int MaxForceX = 10000000;
	int MaxForceY = 10000000;
	int rmin = 5;
	int Endst = 8;
	if (ARM->Spec == 2)Endst = 4;
	if (ARM->Spec == 2)rmin = 3;
	for (int i = 0; i < Na; i++)
	{
		int rr = Norma(int(AINF->MaxX) - MyX, int(AINF->MaxY) - MyY);
		int rr1 = Norma(int(AINF->MaxX) - MyX, int(AINF->MinY) - MyY);
		if (rr1 < rr)rr = rr1;
		rr1 = Norma(int(AINF->MinX) - MyX, int(AINF->MaxY) - MyY);
		if (rr1 < rr)rr = rr1;
		rr1 = Norma(int(AINF->MinX) - MyX, int(AINF->MinY) - MyY);
		if (rr1 < rr)rr = rr1;

		if (rr < Endst)
		{
			int curforce = AINF->N;//NSword+AINF->NCaval+(AINF->NStrel>>2)+AINF->NStrel;
			if (curforce > MaxForce)
			{
				MaxForce = curforce;
				MaxForceX = (int(AINF->MaxX) + int(AINF->MinX)) >> 1;
				MaxForceY = (int(AINF->MaxY) + int(AINF->MinY)) >> 1;
			};
			EnForces += curforce;
		};
		AINF++;
		if (rr < rmin)DoBitva = true;
	};
	return DoBitva;
};
static const int kArmyDecisionSteps = 32;
void ArmyMakeBattleLink(AI_Army* ARM)
{

	if (PeaceTimeLeft) return;
	if (HandleArchers(ARM, 18)) {
		ARM->Bitva();
		return;
	}
	rando();
	ARM->SendGrenaders();
	Army_BTL* OR1 = (Army_BTL*)ARM->AOrder;
	//Fork: was 64 steps between two decisions, a pause at every step of the march
	if ((tmtmt - OR1->time) < kArmyDecisionSteps && tmtmt >= OR1->time) return;
	OR1->time = tmtmt;
	if (!ARM->NExBrigs) {
		ARM->ClearArmy();
		ARM->Enabled = false;
		return;
	}
	if (ARM->TopPos == -1) {
		ARM->Parad();
		return;
	}
	else {
		ARM->Parad();
	}

	int Dsx = int(ARM->x);
	int Dsy = int(ARM->y);
	City* CT = ARM->CT;
	int Na = CT->NDefArms;
	word* Amid = CT->DefArms;
	int MyTop = ARM->TopPos;
	int MTPM = MyTop * NAreas;
	int NearestGreatArmyTop = -1;
	int MinArmDist = 1600;
	int MySize = ARM->NExBrigs;

	// Проверка соседних армий (sequential for determinism)
	if (MyTop != -1 && MyTop != 0xFFFF) {
		for (int i = 0; i < Na; i++) {
			AI_Army* AIAR = CT->ARMS + Amid[i];
			int Atop = AIAR->TopPos;
			if ((ARM->Spec == AIAR->Spec) && (!AIAR->SpecialOrder) && Atop >= 0 && Atop < NAreas && AIAR != ARM) {
				if (Atop == MyTop || MotionLinks[MTPM + Atop] == Atop) {
					AIAR->ClearAOrders();
					int N = AIAR->NExBrigs;
					for (int j = 0; j < N; j++) {
						Brigade* BR = AIAR->ExBrigs[j].Brig;
						ARM->AddBrigade(BR);
						if (!BR->WarType) {
							if (BR->NMemb) {
								BR->RemoveObjects(BR->NMemb, &ARM->CT->Defenders);
							}
							BR->DeleteAll();
							BR->Enabled = 0;
						}
					}
					AIAR->NExBrigs = 0;
					AIAR->ClearArmy();
					AIAR->Enabled = false;
					if (i < Na - 1) {
						memcpy(Amid + i, Amid + i + 1, (Na - i - 1) << 1);
					}
					CT->NDefArms--;
					return;
				}
				else if (AIAR->NExBrigs >= MySize) {
					int d = LinksDist[MTPM + Atop];
					if (d < MinArmDist) {
						MinArmDist = d;
						NearestGreatArmyTop = Atop;
					}
				}
			}
		}
	}

	// Вычисление силы армии (sequential for determinism)
	int MyForce = 0;
	int NMEM = 0;
	{
		for (int i = 0; i < ARM->NExBrigs; i++) {
			Brigade* BR = ARM->ExBrigs[i].Brig;
			if (BR && BR->NMemb) {
				int N = BR->NMemb;
				word* Mem = BR->Memb;
				word* MemSN = BR->MembSN;
				for (int j = 0; j < N; j++) {
					word MID = Mem[j];
					if (MID != 0xFFFF) {
						OneObject* OB = Group[MID];
						if (OB && OB->Serial == MemSN[j] && !OB->Sdoxlo) {
							MyForce += OB->newMons->Force;
							NMEM++;
						}
					}
				}
			}
		}
	}

	//Fork: a large army does not halt in front of positions covered by towers or guns (below): before, any
	//army of any size stopped there, re-formed or pulled back, and only its mortars fought the towers
	//(soldiers cannot damage them), so 300 soldiers could stand for minutes in front of one tower
	const bool bigArmy = NMEM > 200 || MyForce > 3800;
	if (bigArmy) MyForce = 100000;

	// Анализ вражеских сил
	ArmyInfo* AINF = GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	Na = GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX = (ARM->x) >> 8;
	int MyY = (ARM->y) >> 8;
	int EnForces = 0;
	int MaxForce = 0;
	int MaxForceX = 10000000;
	int MaxForceY = 10000000;
	bool DoBitva = false;
	int rmin = 4;
	int Endst = 7;
	if (ARM->Spec == 2) {
		Endst = 3;
		rmin = 2;
	}

	{
		for (int i = 0; i < Na; i++) {
			int rr = Norma(int(AINF->MaxX) - MyX, int(AINF->MaxY) - MyY);
			int rr1 = Norma(int(AINF->MaxX) - MyX, int(AINF->MinY) - MyY);
			if (rr1 < rr) rr = rr1;
			rr1 = Norma(int(AINF->MinX) - MyX, int(AINF->MaxY) - MyY);
			if (rr1 < rr) rr = rr1;
			rr1 = Norma(int(AINF->MinX) - MyX, int(AINF->MinY) - MyY);
			if (rr1 < rr) rr = rr1;

			if (rr < Endst) {
				int curforce = AINF->N;
				EnForces += curforce;
				if (curforce > MaxForce) {
					MaxForce = curforce;
					MaxForceX = (int(AINF->MaxX) + int(AINF->MinX)) >> 1;
					MaxForceY = (int(AINF->MaxY) + int(AINF->MinY)) >> 1;
				}
			}
			if (rr < rmin) DoBitva = true;
			AINF++;
		}
	}

	int cx = CT->CenterX >> 1;
	int cy = CT->CenterY >> 1;
	int RR1 = Norma(MaxForceX - cx, MaxForceY - cy);
	if (RR1 < 10) MyForce = MyForce << 3;
	else if (RR1 < 25) MyForce = MyForce << 1;

	if (EnForces && EnForces > MyForce) {
		int CurR = Norma(MaxForceX - MyX, MaxForceY - MyY);
		Na = CT->NDefArms;
		Amid = CT->DefArms;
		int Mindst = 1000000;
		int FinTop = 0xFFFF;

		for (int i = 0; i < Na; i++) {
			AI_Army* AIAR = CT->ARMS + Amid[i];
			int AIX = AIAR->x >> 8;
			int AIY = AIAR->y >> 8;
			if (AIAR->TopPos >= 0 && AIAR->TopPos < NAreas && (AIX != MyX || AIY != MyY)) {
				int dst = Norma(AIX - MaxForceX, AIY - MaxForceY);
				int topdst = LinksDist[int(AIAR->TopPos) * NAreas + MyTop];
				if (topdst < Mindst && dst > CurR) {
					Mindst = topdst;
					FinTop = AIAR->TopPos;
				}
			}
		}

		if (FinTop == 0xFFFF) {
			FinTop = GetTopology(int(CT->CenterX << 7), int(CT->CenterY << 7));
		}

		if (FinTop != 0xFFFF) {
			word NextTop = MotionLinks[MyTop * NAreas + FinTop];
			if (NextTop != 0xFFFF) {
				Area* AR = TopMap + NextTop;
				ARM->LocalSendTo((AR->x << 6) + 32, (AR->y << 6) + 32, 128 + 64, 1);
				return;
			}
		}
	}
	else if (DoBitva) {
		ARM->Bitva();
		if (ARM->AOrder && ARM->AOrder->ALink == &A_BitvaLink) return;
	}

	bool FastExit = false;
	if (ARM->Spec == 2) {
		word FTop = GetNextGreCell(ARM->NI, ARM->NExBrigs * 16, MyTop);
		if (FTop != 0xFFFF) {
			Area* AR = TopMap + FTop;
			Dsx = AR->x << 6;
			Dsy = AR->y << 6;
		}
		else {
			if (!FindNearestWall(&Dsx, &Dsy, CT->NI)) {
				if (!GNFO.FindNearestEnemy(ARM->CT->NI, &Dsx, &Dsy, true, 30, ARM->CT->DefenceStage)) FastExit = true;
			}
		}
	}
	else if (ARM->Spec == 1) {
		word FTop = GetNextDivCell(ARM->NI, ARM->NExBrigs * 16, MyTop);
		if (FTop != 0xFFFF) {
			Area* AR = TopMap + FTop;
			Dsx = AR->x << 6;
			Dsy = AR->y << 6;
		}
		else {
			if (!GNFO.FindNearestEnemy(ARM->CT->NI, &Dsx, &Dsy, true, 30, ARM->CT->DefenceStage)) {
				if (!FindNearestWall(&Dsx, &Dsy, CT->NI)) FastExit = true;
			}
		}
	}
	else {
		if (!GNFO.FindNearestEnemy(ARM->CT->NI, &Dsx, &Dsy, true, 30, ARM->CT->DefenceStage)) {
			if (!FindNearestWall(&Dsx, &Dsy, CT->NI)) FastExit = true;
		}
	}

	if (FastExit) {
		Area* AR = TopMap + MyTop;
		int tpx = (int(AR->x) << 6) + 32;
		int tpy = (int(AR->y) << 6) + 32;
		if (ARM->GetArmyDanger(tpx, tpy) > 2) {
			ARM->SendToMostSafePosition();
		}
		return;
	}

	word Top = GetTopology(Dsx, Dsy);
	if (Top == 0xFFFF) {
		return;
	}

	word Top1 = ARM->TopPos;
	if (Top1 == 0xFFFF) {
		ARM->DeleteAOrder();
		return;
	}

	word NextTop = MotionLinks[Top1 * NAreas + Top];
	if (NextTop == 0xFFFF) {
		Area* AR = TopMap + MyTop;
		int tpx = (int(AR->x) << 6) + 32;
		int tpy = (int(AR->y) << 6) + 32;
		if (ARM->GetArmyDanger(tpx, tpy) > 2) {
			ARM->SendToMostSafePosition();
		}
		return;
	}

	if (GetTopDanger(NextTop, Endst, ARM->CT->NI) > MyForce) {
		if (NearestGreatArmyTop != -1) {
			if (NextTop != NearestGreatArmyTop) {
				NextTop = MotionLinks[MTPM + NearestGreatArmyTop];
			}
		}
		else {
			return;
		}
	}

	Area* AR = TopMap + NextTop;
	Area* MAR = TopMap + MyTop;
	int tpx = (int(MAR->x) << 6) + 32;
	int tpy = (int(MAR->y) << 6) + 32;
	if (!bigArmy && ARM->GetArmyDanger(tpx, tpy) > 1) {
		ARM->SendToMostSafePosition();
		return;
	}

	tpx = (int(AR->x) << 6) + 32;
	tpy = (int(AR->y) << 6) + 32;
	if (!bigArmy && ARM->GetArmyDanger(tpx, tpy) > 2) {
		if (rando() < 4096) {
			ARM->Parad();
		}
		for (int n = 0; n < ARM->NExBrigs; n++) {
			if (ARM->ExBrigs[n].BrigadeType == 5) return;
		}
		return;
	}

	if (NextTop == Top) {
		ARM->LocalSendTo(Dsx, Dsy, 128, 1);
	}
	else {
		ARM->LocalSendTo((AR->x << 6) + 32, (AR->y << 6) + 32, 128 + 64, 1);
	}
}
void ArmyMakeDiversiaLink(AI_Army* ARM)
{
	rando();
	ARM->SendGrenaders();
	Army_BTL* OR1 = (Army_BTL*)ARM->AOrder;
	if ((tmtmt - OR1->time) < 64) return;
	OR1->time = tmtmt;

	if (!ARM->NExBrigs) {
		ARM->ClearArmy();
		ARM->Enabled = false;
		return;
	}

	if (ARM->TopPos == -1) {
		ARM->Parad();
		return;
	}
	else {
		ARM->Parad();
	}

	int Dsx = int(ARM->x);
	int Dsy = int(ARM->y);
	City* CT = ARM->CT;
	int Na = CT->NDefArms;
	word* Amid = CT->DefArms;
	int MyTop = ARM->TopPos;
	int HisTop = 0xFFFF;
	int HisMinDis = 0xFFFF;

	if (MyTop != -1)
	{
		// Sequential version (threading removed for determinism)
		for (int i = 0; i < Na; i++)
		{
			AI_Army* AIAR = CT->ARMS + Amid[i];
			int Atop = AIAR->TopPos;
			if ((ARM->Spec == AIAR->Spec) && !AIAR->SpecialOrder && Atop >= 0 && Atop < NAreas && AIAR != ARM)
			{
				if (Atop == MyTop || MotionLinks[MyTop * NAreas + Atop] == Atop)
				{
					AIAR->ClearAOrders();
					int N = AIAR->NExBrigs;
					for (int j = 0; j < N; j++)
					{
						Brigade* BR = AIAR->ExBrigs[j].Brig;
						ARM->AddBrigade(BR);
						if (BR->NMemb) BR->RemoveObjects(BR->NMemb, &ARM->CT->Defenders);
						BR->DeleteAll();
						BR->Enabled = true;
					}
					AIAR->ClearArmy();
					AIAR->Enabled = false;
					if (i < Na - 1)
						memcpy(Amid + i, Amid + i + 1, (Na - i - 1) << 1);
					CT->NDefArms--;
					return;
				}
				else
				{
					int R = LinksDist[MyTop * NAreas + Atop];
					if (R < HisMinDis) {
						HisMinDis = R;
						HisTop = Atop;
					}
				}
			}
		}
	}

	// далее логика без изменений
	ArmyInfo* AINF = GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	Na = GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX = ARM->x >> 8;
	int MyY = ARM->y >> 8;
	bool DoBitva = false;
	int rmin = (ARM->Spec == 2 ? 2 : 4);

	for (int i = 0; i < Na; i++)
	{
		int rr = Norma(AINF->MaxX - MyX, AINF->MaxY - MyY);
		int rr1 = Norma(AINF->MaxX - MyX, AINF->MinY - MyY);
		if (rr1 < rr) rr = rr1;
		rr1 = Norma(AINF->MinX - MyX, AINF->MaxY - MyY);
		if (rr1 < rr) rr = rr1;
		rr1 = Norma(AINF->MinX - MyX, AINF->MinY - MyY);
		if (rr1 < rr) rr = rr1;
		AINF++;
		if (rr < rmin) DoBitva = true;
	}

	if (DoBitva)
	{
		ARM->Bitva();
		if (ARM->AOrder && ARM->AOrder->ALink == &A_BitvaLink) return;
	}

	bool FastExit = false;
	if (HisTop == 0xFFFF)
	{
		if (ARM->Spec == 2)
		{
			if (!FindNearestWall(&Dsx, &Dsy, CT->NI))
				if (!GNFO.FindNearestEnemy(CT->NI, &Dsx, &Dsy, true, 0, 0))
					FastExit = true;
		}
		else
		{
			if (!GNFO.FindNearestEnemy(CT->NI, &Dsx, &Dsy, true, 0, 0))
				if (!FindNearestWall(&Dsx, &Dsy, CT->NI))
					FastExit = true;
		}
	}

	if (FastExit)
	{
		Area* AR = TopMap + MyTop;
		int tpx = (AR->x << 6) + 32;
		int tpy = (AR->y << 6) + 32;
		if (ARM->GetArmyDanger(tpx, tpy) > 2)
			ARM->SendToMostSafePosition();
		return;
	}

	word Top = (HisTop == 0xFFFF ? GetTopology(Dsx, Dsy) : HisTop);
	if (Top == 0xFFFF) return;
	word Top1 = ARM->TopPos;
	if (Top1 == 0xFFFF) { ARM->DeleteAOrder(); return; }
	word NextTop = MotionLinks[Top1 * NAreas + Top];

	if (NextTop == 0xFFFF || NextTop == MyTop)
	{
		Area* AR = TopMap + MyTop;
		int tpx = (AR->x << 6) + 32;
		int tpy = (AR->y << 6) + 32;
		if (ARM->GetArmyDanger(tpx, tpy) > 2)
			ARM->SendToMostSafePosition();
		return;
	}

	Area* AR = TopMap + NextTop;
	Area* MAR = TopMap + MyTop;
	int tpx = (MAR->x << 6) + 32;
	int tpy = (MAR->y << 6) + 32;
	tpx = (AR->x << 6) + 32;
	tpy = (AR->y << 6) + 32;

	if (NextTop == Top)
		ARM->LocalSendTo(Dsx, Dsy, 128, 1);
	else
		ARM->LocalSendTo((AR->x << 6) + 32, (AR->y << 6) + 32, 192, 1);
}


char* ABTL_Mess = "[AI_Army::Make Battle]";
void AI_Army::MakeBattle()
{
	Army_BTL* OR1 = (Army_BTL*)CreateOrder(0, sizeof(Army_BTL));
	OR1->Message = ABTL_Mess;
	OR1->time = -10000;
	OR1->xdest = -1;
	OR1->ydest = -1;
	OR1->Size = sizeof(Army_BTL);
	OR1->ALink = &ArmyMakeBattleLink;
	return;
}

int  AI_Army::GetAmountOfBrigs(byte Type)
{
	int N = 0;
	for (int i = 0; i < NExBrigs; i++)
	{
		if (ExBrigs[i].BrigadeType == Type)
		{
			N++;
		}
	}

	return N;
}

void SHIP_DIVERSIA(AI_Army* ARM)
{
	rando();
	Army_BTL* OR1 = (Army_BTL*)ARM->CreateOrder(0, sizeof(Army_BTL));
	OR1->Message = ABTL_Mess;
	OR1->time = -10000;
	OR1->xdest = -1;
	OR1->ydest = -1;
	OR1->Size = sizeof(Army_BTL);
	OR1->ALink = &ArmyMakeDiversiaLink;
	return;
}

bool FastSearchGreAttObject(int* x, int* y, int r, byte NI);

void AI_Army::SendGrenaders()
{
	int x0 = x >> 6;
	int y0 = y >> 6;
	if (GetAmountOfBrigs(6) && FastSearchGreAttObject(&x0, &y0, 35, CT->NI))
	{
		for (int i = 0; i < NExBrigs; i++)
		{
			if (ExBrigs[i].BrigadeType == 6)
			{
				Brigade* BR = ExBrigs[i].Brig;
				int N = BR->NMemb;
				word* Mem = BR->Memb;
				word* MSN = BR->MembSN;
				for (int j = 0; j < N; j++)
				{
					word MID = Mem[j];
					if (MID != 0xFFFF)
					{
						OneObject* OB = Group[MID];
						if (OB && OB->Serial == MSN[j])
						{
							GrenaderSuperman(OB);
						}
					}
				}
			}
		}
	}
}

//-------------------------------------Bitva-------------------------------------//
char* ABIT_Message = "[Army::Bitva]";
//#define maxen
class Army_Bitva :public ArmyOrder
{
public:
	byte BitMask[1024];
	word Enm[512];
	word EnSN[512];
	byte NDang[512];
	int  NEn;
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	void AddEnemXY(int x, int y, int MyTop, byte msk);
};
word GetTopFast(int x, int y)
{
	if (x >= 0 && y >= 0 && x < TopLx && y < TopLy)
	{
		return TopRef[x + (y << TopSH)];
	}
	else return 0xFFFF;
};
void Army_Bitva::AddEnemXY(int x, int y, int MyTop, byte msk)
{
	// Проверка базовых условий
	if (NEn >= 512 || MyTop < 0 || MyTop >= NAreas) return;

	int x0 = x << 1;
	int y0 = y << 1;

	// Проверка топологии клеток
	int HimTop1 = GetTopFast(x0, y0);
	int HimTop2 = GetTopFast(x0 + 1, y0);
	int HimTop3 = GetTopFast(x0, y0 + 1);
	int HimTop4 = GetTopFast(x0 + 1, y0 + 1);

	if (HimTop1 >= 0xFFFE && HimTop2 >= 0xFFFE && HimTop3 >= 0xFFFE && HimTop4 >= 0xFFFE) return;

	int NTP = MyTop * NAreas;
	// Проверка дистанции для каждой клетки
	if (HimTop1 < 0xFFFE && HimTop1 != MyTop && LinksDist[HimTop1 + NTP] > 50) return;
	if (HimTop2 < 0xFFFE && HimTop2 != MyTop && LinksDist[HimTop2 + NTP] > 50) return;
	if (HimTop3 < 0xFFFE && HimTop3 != MyTop && LinksDist[HimTop3 + NTP] > 50) return;
	if (HimTop4 < 0xFFFE && HimTop4 != MyTop && LinksDist[HimTop4 + NTP] > 50) return;

	// Вычисление клетки
	int cell = x + 1 + ((y + 1) << VAL_SHFCX);
	if (cell <= 0 || cell >= MAXCIOFS) return;

	// Проверка монстров в клетке
	int NMon = MCount[cell];
	if (!NMon) return;

	int ofs1 = cell << SHFCELL;
	word MID;
	for (int i = 0; i < NMon; i++)
	{
		MID = GetNMSL(ofs1 + i);
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && !(OB->NMask & msk || OB->Sdoxlo || OB->newMons->LockType))
			{
				int id = OB->Index;
				int ms = 1 << (id & 7);
				int of = id >> 3;

				//Fork: BitMask covers the first 8192 unit indices only, and units above were skipped: late in a
				//long game every new soldier has a higher index, the enemy list of the battle stayed empty and
				//whole armies stood in contact without fighting (AI log 2026-10-29 12:38, P4 at 118:15: 218 of
				//230 soldiers next to enemies #9020-#10249 idle). Those are looked up in the list instead; the
				//order keeps its size, so battles in saved games still load.
				bool known;
				if (of < int(sizeof(BitMask)))
				{
					known = (BitMask[of] & ms) != 0;
					BitMask[of] |= ms;
				}
				else
				{
					known = false;
					for (int e = 0; e < NEn && !known; e++)known = Enm[e] == id;
				}
				if (!known)
				{
					Enm[NEn] = id;
					EnSN[NEn] = OB->Serial;
					NDang[NEn] = 0;
					NEn++;
					if (NEn >= 512) return;
				}
			}
		}
	}
}
word GetDir(int dx, int dy);
void AttackObjLink(OneObject* OBJ);
void B_BitvaLink(Brigade* BR);
void A_BitvaLink(AI_Army* ARM)
{
	rando();
	Army_Bitva* OR1 = (Army_Bitva*)ARM->AOrder;
	bool ARCHOBJ = HandleArchers(ARM, 10000000);

	// 1. Check range of battle
	if (rando() < 8192)
	{
		int MinX = 10000000;
		int MinY = 10000000;
		int MaxX = 0;
		int MaxY = 0;

		for (int i = 0; i < ARM->NExBrigs; i++)
		{
			Brigade* BR = ARM->ExBrigs[i].Brig;
			if (!BR) continue;
			int N = BR->NMemb;
			word* Mem = BR->Memb;
			word* MSN = BR->MembSN;
			for (int j = 0; j < N; j++)
			{
				word MID = Mem[j];
				if (MID != 0xFFFF)
				{
					OneObject* OB = Group[MID];
					if (OB && OB->Serial == MSN[j])
					{
						int xx = OB->RealX >> 11;
						int yy = OB->RealY >> 11;
						if (xx < MinX) MinX = xx;
						if (yy < MinY) MinY = yy;
						if (xx > MaxX) MaxX = xx;
						if (yy > MaxY) MaxY = yy;
					}
				}
			}
		}
		if (MaxX >= MinX)
		{
			MinX -= 6;
			MaxX += 6;
			MinY -= 6;
			MaxY += 6;
			if (MinX < 0) MinX = 0;
			if (MinY < 0) MinY = 0;
			OR1->MinX = MinX;
			OR1->MaxX = MaxX;
			OR1->MinY = MinY;
			OR1->MaxY = MaxY;
		}
		else
		{
			ARM->DeleteAOrder();
			return;
		}
	}

	// 2. Renew enemy list
	if (rando() < 2048)
	{
		int N = OR1->NEn;
		word* Mem = OR1->Enm;
		word* MSN = OR1->EnSN;
		byte* ATT = OR1->NDang;
		for (int i = 0; i < N; i++)
		{
			OneObject* OB = Group[Mem[i]];
			if (!OB || (OB && (OB->Sdoxlo || OB->Serial != MSN[i])))
			{
				if (i < N - 1)
				{
					memcpy(Mem + i, Mem + i + 1, (N - i - 1) << 1);
					memcpy(MSN + i, MSN + i + 1, (N - i - 1) << 1);
					memcpy(ATT + i, ATT + i + 1, N - i - 1);
				}
				i--;
				N--;
			}
		}
		OR1->NEn = N;
	}

	int MaxAttServ = 64;
	bool InBattle = false;
	bool MorPresent = false;

	// 3. Adding new units to battle
	int MinX = OR1->MinX;
	int MinY = OR1->MinY;
	int Dx = OR1->MaxX - OR1->MinX + 1;
	int Dy = OR1->MaxY - OR1->MinY + 1;
	byte msk = 1 << ARM->CT->NI;
	for (int p = 0; p < 25; p++)
	{
		int xx = ((int(rando()) * Dx) >> 15) + MinX;
		int yy = ((int(rando()) * Dy) >> 15) + MinY;
		OR1->AddEnemXY(xx, yy, ARM->TopPos, msk);
	}

	// 4. Attack service
	// Подсчёт общего количества юнитов
	int totalUnits = 0;
	for (int i = 0; i < ARM->NExBrigs; i++)
	{
		Brigade* BR = ARM->ExBrigs[i].Brig;
		if (BR) totalUnits += BR->NMemb;
	}

	// Sequential processing (threading removed for determinism)
	{
		for (int i = 0; i < ARM->NExBrigs; i++)
		{
			Brigade* BR = ARM->ExBrigs[i].Brig;
			if (!BR) continue;
			int N = BR->NMemb;
			word* Mem = BR->Memb;
			word* MSN = BR->MembSN;
			for (int j = 0; j < N; j++)
			{
				word MID = Mem[j];
				if (MID != 0xFFFF)
				{
					OneObject* OB = Group[MID];
					if (OB && OB->Serial == MSN[j] && OB->newMons && OB->Ref.General && OB->Ref.General->MoreCharacter)
					{
						if (!(OB->newMons->Artilery || OB->newMons->Archer))
						{
							if (!(OB->LocalOrder) || (OB->EnemyID == 0xFFFF))
							{
								NewMonster* NM = OB->newMons;
								AdvCharacter* ADC = OB->Ref.General->MoreCharacter;
								byte mms = NM->KillMask;
								int MinR = ADC->MinR_Attack;
								int MaxR = ADC->MaxR_Attack;
								int myx = OB->RealX;
								int myy = OB->RealY;
								if (MaxR)
								{
									int NearDist = 1000000;
									int ReadyDist = 1000000;
									word NearMID = 0xFFFF;
									word ReadyMID = 0xFFFF;
									int NEn = OR1->NEn;
									word* MemEn = OR1->Enm;
									word* MSNEn = OR1->EnSN;
									byte* DANG = OR1->NDang;
									char mdr = OB->RealDir;
									for (int t = 0; t < NEn; t++)
									{
										word MID = MemEn[t];
										OneObject* EOB = Group[MID];
										int dan = DANG[t];
										if (EOB && EOB->newMons && !EOB->Sdoxlo && EOB->Serial == MSNEn[t])
										{
											NewMonster* ENM = EOB->newMons;
											int Eusage = ENM->Usage;
											if (Eusage == MortiraID) MorPresent = true;
											if (ENM->MathMask & mms)
											{
												int R = Norma(myx - EOB->RealX, myy - EOB->RealY) >> 4;
												if (R > MinR)
												{
													if (R < NearDist)
													{
														if (EOB->InMotion)
														{
															int dr1 = GetDir(EOB->RealX - myx, EOB->RealY - myy);
															char dr = EOB->RealDir - dr1;
															if (abs(dr) > 64)
															{
																NearMID = MID;
																NearDist = R;
															}
														}
														else
														{
															NearMID = MID;
															NearDist = R;
														}
													}
													if (R < MaxR && R < ReadyDist)
													{
														ReadyMID = MID;
														ReadyDist = R;
													}
												}
											}
										}
									}
									if (ReadyMID == 0xFFFF) ReadyMID = NearMID;
									if (ReadyMID != 0xFFFF)
									{
										OB->AttackObj(ReadyMID, 128 + 64);
										if (OB->LocalOrder && OB->LocalOrder->DoLink == &AttackObjLink)
										{
											OB->LocalOrder->DoLink(OB);
											if (OB->LocalOrder && OB->LocalOrder->DoLink == &AttackObjLink)
											{
												InBattle = true;
											}
										}
									}
								}
							}
							else
							{
								InBattle = true;
							}
						}
					}
				}
			}
		}
	}

	//Fork: formations fight in their own battle order (AttackObj hands the target to Brigade::Bitva and gives
	//the unit no attack order), so the loop above saw no fighting unit and ended the army battle in the step
	//it began. The army's march order then pulled the formations back into line, and 64 steps later the
	//battle began again: melee formations marched, stopped for a moment, marched again
	if (!InBattle)
	{
		for (int i = 0; i < ARM->NExBrigs && !InBattle; i++)
		{
			Brigade* BR = ARM->ExBrigs[i].Brig;
			if (BR && BR->BOrder && BR->BOrder->BLink == &B_BitvaLink)InBattle = true;
		}
	}
	if (!(InBattle || ARCHOBJ))
	{
		ARM->DeleteAOrder();
	}
}
void UpdateAttackR(AdvCharacter* ADC)
{
	int MinR = 10000;
	int MaxR = 0;
	int MaxD = 0;
	for (int p = 0; p < NAttTypes; p++)
	{
		int Dam = ADC->MaxDamage[p];
		if (Dam > MaxD)MaxD = Dam;
		int R2 = ADC->AttackRadius2[p];
		if (R2)
		{
			int R1 = ADC->AttackRadius1[p];
			if (R1 < MinR)MinR = R1;
			if (R2 > MaxR)MaxR = R2;
		};
	};
	if (MaxR < MinR)
	{
		MaxR = 0;
		MinR = 0;
	};
	ADC->MinR_Attack = MinR;
	ADC->MaxR_Attack = MaxR;
	ADC->MaxDam = MaxD;
};
void AI_Army::Bitva()
{
	if (SpecialOrder)return;
	if (tmtmt - LastBitvaTime < 64 || (AOrder && AOrder->ALink == &A_BitvaLink))return;
	LastBitvaTime = tmtmt;
	Army_Bitva* OR1 = (Army_Bitva*)CreateOrder(1, sizeof(Army_Bitva));
	OR1->Message = ABIT_Message;
	OR1->Size = sizeof(Army_Bitva);
	memset(OR1->BitMask, 0, sizeof(OR1)->BitMask);
	OR1->NEn = 0;

	int MinX = 10000000;
	int MinY = 10000000;
	int MaxX = 0;
	int MaxY = 0;

	for (int i = 0; i < NExBrigs; i++)
	{
		Brigade* BR = ExBrigs[i].Brig;
		int N = BR->NMemb;
		word* Mem = BR->Memb;
		word* MSN = BR->MembSN;
		for (int j = 0; j < N; j++)
		{
			word MID = Mem[j];
			if (MID != 0xFFFF)
			{
				OneObject* OB = Group[MID];
				if (OB && OB->Serial == MSN[j])
				{
					int xx = OB->RealX >> 11;
					int yy = OB->RealY >> 11;
					if (xx < MinX)MinX = xx;
					if (yy < MinY)MinY = yy;
					if (xx > MaxX)MaxX = xx;
					if (yy > MaxY)MaxY = yy;
				};
			};
		};
	};
	byte msk = 1 << CT->NI;
	if (MaxX >= MinX)
	{
		MinX -= 6;
		MaxX += 6;
		MinY -= 6;
		MaxY += 6;
		if (MinX < 0)MinX = 0;
		if (MinY < 0)MinY = 0;
		for (int ix = MinX; ix <= MaxX; ix++)
		{
			for (int iy = MinY; iy <= MaxY; iy++)
			{
				OR1->AddEnemXY(ix, iy, TopPos, msk);
			};
		};
		OR1->MinX = MinX;
		OR1->MaxX = MaxX;
		OR1->MinY = MinY;
		OR1->MaxY = MaxY;
		OR1->ALink = &A_BitvaLink;
	}
	else
	{
		DeleteAOrder();
		return;
	};
	A_BitvaLink(this);
};
//----------------------------------DIVERSION GROUP------------------------------//
//Problems of diversants:
//1.Erase mines of enemy
//2.Erase small groups of units
//3.Erase any capturable objects
int FindStrongestUnitInCell(int cell, int* MinStrength, byte NAT)
{
	cell += VAL_MAXCX + 1;
	int strength = *MinStrength;
	int NMon = MCount[cell];
	if (!NMon)return 0xFFFF;
	int ofs1 = cell << SHFCELL;
	word MID;
	word BestMID = 0xFFFF;
	int N = 0;
	for (int i = 0; i < NMon; i++)
	{
		MID = GetNMSL(ofs1 + i);
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && (!OB->Sdoxlo) && OB->NNUM == NAT)
			{
				int Dam = OB->Ref.General->MoreCharacter->MaxDam;
				if (strength <= Dam)
				{
					strength = Dam;
					BestMID = MID;
				};
			};
		};
	};
	*MinStrength = strength;
	return BestMID;
};
int FindStrongestUnit(int x, int y, byte NAT)
{
	int cell = (y << (VAL_SHFCX + 1)) + x + x;
	word MID = 0xFFFF;
	if (cell >= 0)
	{
		int Stren = 0;
		int MID1 = FindStrongestUnitInCell(cell, &Stren, NAT);
		if (MID1 != 0xFFFF)MID = MID1;
		MID1 = FindStrongestUnitInCell(cell + 1, &Stren, NAT);
		if (MID1 != 0xFFFF)MID = MID1;
		MID1 = FindStrongestUnitInCell(cell + VAL_MAXCX, &Stren, NAT);
		if (MID1 != 0xFFFF)MID = MID1;
		MID1 = FindStrongestUnitInCell(cell + VAL_MAXCX + 1, &Stren, NAT);
		if (MID1 != 0xFFFF)MID = MID1;
	};
	return MID;
};
char* ADIV_Message = "[AI_Army::Diversion]";
class DiversionOrder :public ArmyOrder
{
public:
	word EnMID;
	byte MotionMethod;
	word NextTop;
	word Prev1Top;
	word Prev2Top;
	word Prev3Top;
	int  LastEnemyTime;
};
int GetAmountOfProtectors(OneObject* OBJ);
word GetNearestDefender(OneObject* OB);
word GetNextSafeCell(byte NI, int F, int start, int Fin);
int LastRND = 0;
void A_DiversiaLink(AI_Army* ARM)
{
	if (PeaceTimeLeft) return;
	LastRND = rando();
	DiversionOrder* OR1 = (DiversionOrder*)ARM->AOrder;
	word EnMID = OR1->EnMID;
	byte MMet = OR1->MotionMethod;

	if (!ARM->NExBrigs)
	{
		ARM->ClearArmy();
		ARM->Enabled = 0;
		return;
	}

	if (MMet < 128)
	{
		ARM->Parad();
		int MyX = ARM->x << 4;
		int MyY = ARM->y << 4;
		int MyTop = GetTopology(ARM->x, ARM->y);

		// 1. Find new target if needed
		bool Fear = false;
		int NearDst = 10000, Enx = 0, Eny = 0;
		if (EnMID == 0xFFFF || tmtmt - OR1->LastEnemyTime > 100)
		{
			word Mid[5] = { 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF };
			int  RMid[5] = { 10000000,10000000,10000000,10000000,10000000 };
			word BestMid = 0xFFFF;

			if (MyTop != 0xFFFF)
			{
				// 1a. Enemy armies
				int NArm = GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
				ArmyInfo* AIN = GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
				int arx = ARM->x >> 8, ary = ARM->y >> 8;
				int NMax = ARM->NExBrigs ? ARM->ExBrigs[0].Brig->NMemb : 0;
				for (int i = 0; i < NArm; i++, AIN++)
				{
					int NCU = AIN->NCaval + AIN->NMort + AIN->NSword + AIN->NStrel;
					int ax = (AIN->MinX + AIN->MaxX) >> 1;
					int ay = (AIN->MinY + AIN->MaxY) >> 1;
					int dst = Norma(arx - ax, ary - ay);
					if (dst < 6 && NCU > NMax) Fear = true;
					if (!Fear && dst < 8 && NCU <= NMax && dst < NearDst)
					{
						NearDst = dst; Enx = ax; Eny = ay;
					}
				}
				// 1b. Capturable units
				byte mask = 1 << ARM->NI;
				for (int MID = 0; MID < MAXOBJECT; MID++)
				{
					OneObject* OB = Group[MID];
					if (!OB || OB->Sdoxlo || (OB->NMask & mask) || !OB->newMons) continue;
					if (!OB->newMons->Capture && OB->Ready) continue;
					int prot = GetAmountOfProtectors(OB);
					if (prot >= 5) continue;
					int tier = prot ? 1 : 0;
					int HimTop = GetTopology(OB->RealX >> 4, OB->RealY >> 4);
					if (HimTop == 0xFFFF) continue;
					int dst = (HimTop == MyTop ? 0 : LinksDist[HimTop * NAreas + MyTop]);
					if (dst < 30)
					{
						if (tier == 0) NearDst = NearDst;  // keep NearDst for fear logic
						if (dst < RMid[tier])
						{
							RMid[tier] = dst;
							Mid[tier] = MID;
						}
					}
				}
			}

			if (RMid[0] < 30) BestMid = Mid[0];
			else if (!Fear && NearDst < 10) BestMid = FindStrongestUnit(Enx, Eny, 0);
			if (BestMid == 0xFFFF)
			{
				if (RMid[1] < 30) BestMid = Mid[1];
				else if (RMid[0] < 80) BestMid = Mid[0];
				else BestMid = (RMid[0] < RMid[1] ? Mid[0] : Mid[1]);
			}
			if (BestMid != 0xFFFF) OR1->EnMID = EnMID = BestMid;
		}

		// 2. Move toward target or reset
		if (EnMID != 0xFFFF)
		{
			OneObject* EOB = Group[EnMID];
			if (EOB && !EOB->Sdoxlo)
			{
				int HimTop = GetTopology(EOB->RealX >> 4, EOB->RealY >> 4);
				if (HimTop != 0xFFFF)
				{
					int NextTop = GetNextSafeCell(ARM->NI, 10, MyTop, HimTop);
					if (HimTop == MyTop) NextTop = MyTop;

					if (NextTop == MyTop)
					{
						word DEF = GetNearestDefender(EOB);
						if (DEF == 0xFFFF)
						{
							// capture
							ARM->ClearZombi();
							bool sent = false;
							for (int i = 0; i < ARM->NExBrigs && !sent; i++)
							{
								Brigade* BR = ARM->ExBrigs[i].Brig;
								int M = BR->NMemb;
								for (int j = 0; j < M && !sent; j++)
								{
									OneObject* OB = Group[BR->Memb[j]];
									if (OB && OB->Serial == BR->MembSN[j])
									{
										OB->NewMonsterSendTo(EOB->RealX, EOB->RealY, 192, 0);
										sent = true;
									}
								}
							}
						}
						else
						{
							// fight defender
							ARM->ClearZombi();
							for (int i = 0; i < ARM->NExBrigs; i++)
							{
								Brigade* BR = ARM->ExBrigs[i].Brig;
								int M = BR->NMemb;
								for (int j = 0; j < M; j++)
								{
									OneObject* OB = Group[BR->Memb[j]];
									if (OB && OB->Serial == BR->MembSN[j] && (!OB->LocalOrder || OB->EnemyID == 0xFFFF))
										OB->AttackObj(DEF, 192);
								}
							}
						}
					}
					else if (NextTop != 0xFFFF)
					{
						Area* AR = TopMap + NextTop;
						ARM->WideLocalSendTo(AR->x << 6, AR->y << 6, 192, 1);
						ARM->SetZombi();
					}
				}
			}
			else OR1->EnMID = 0xFFFF;
		}
		else OR1->EnMID = 0xFFFF;

		// 3. Fallback when no target
		if (OR1->EnMID == 0xFFFF)
		{
			ARM->ClearZombi();
			int MyTopLinks = MyTop;
			int NArm = GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
			ArmyInfo* AIN = GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
			int arx = ARM->x >> 8, ary = ARM->y >> 8;
			for (int j = 0; j < NArm; j++, AIN++)
			{
				int x = (AIN->MinX + AIN->MaxX) >> 1;
				int y = (AIN->MinY + AIN->MaxY) >> 1;
				int NU = AIN->NCaval + AIN->NStrel + AIN->NSword;
				if (NU > 6 && Norma(x - arx, y - ary) < 8)
				{
					int ThemTop = GetTopology(x << 8, y << 8);
					if (ThemTop != 0xFFFF)
					{
						Area* AR = TopMap + MyTopLinks;
						int NL = AR->NLinks;
						int MaxD = 0; word Best = 0xFFFF;
						for (int k = 0; k < NL; k++)
						{
							word L = AR->Link[k + k];
							int d = LinksDist[L + ThemTop * NAreas];
							if (d != 0xFFFF && d > MaxD)
							{
								MaxD = d; Best = L;
							}
						}
						if (Best != 0xFFFF)
						{
							Area* AR2 = TopMap + Best;
							ARM->WideLocalSendTo(AR2->x << 6, AR2->y << 6, 192, 1);
						}
					}
				}
			}
		}
	}
	// else — индивидуальное движение (оставляем без изменений)

	// 4. Check for alive units (parallel if >5000 total)
	// count total members first
	int total = 0;
	for (int i = 0; i < ARM->NExBrigs; i++)
		if (ARM->ExBrigs[i].Brig) total += ARM->ExBrigs[i].Brig->NMemb;

	int NAlive = 0;
	{
		for (int i = 0; i < ARM->NExBrigs; i++)
		{
			Brigade* BR = ARM->ExBrigs[i].Brig;
			if (!BR) continue;
			int M = BR->NMemb;
			word* memb = BR->Memb;
			word* msn = BR->MembSN;
			for (int j = 0; j < M; ++j)
			{
				OneObject* OB = Group[memb[j]];
				if (OB && OB->Serial == msn[j])
					NAlive++;
			}
		}
	}

	if (!NAlive)
	{
		ARM->ClearAOrders();
		ARM->Enabled = false;
	}

	rando();
}

void AI_Army::Diversia()
{
	DiversionOrder* OR1 = (DiversionOrder*)CreateOrder(0, sizeof(DiversionOrder));
	OR1->ALink = &A_DiversiaLink;
	OR1->EnMID = 0xFFFF;
	OR1->LastEnemyTime = tmtmt;
	OR1->Message = ADIV_Message;
	OR1->Size = sizeof(DiversionOrder);
	OR1->MotionMethod = 0;
	SpecialOrder = true;
};

//Fork: raids of fast cavalry (Usage FASTHORSE: hussars, Croats, Sich Cossacks, also the Diplomatic Centre's
//mercenary ones) big enough to matter. Before: 5-15 riders, at most 6 raids a game, and no script trained
//fast cavalry, so raids hardly ever happened. RaidSize() riders form a raid (A_DiversiaLink: peasants, mines,
//small groups, avoiding larger armies); AddRaiderTraining trains them.
static const int kRaidSizeEarly = 100;
static const int kRaidLateSec = 1800;      //from minute 30 (game-step minutes) a random size per raid:
static const int kRaidSizeLateMin = 150;
static const int kRaidSizeLateMax = 300;
static const int kMaxRaidsAtOnce = 2;
static int g_RaidSize[8];                  //0 = choose; not saved, chosen again (deterministically) after a load
extern "C" int GetGameSeconds();

static int RaidSize(City* CT)
{
	int& size = g_RaidSize[CT->NI];
	if (!size)
	{
		size = GetGameSeconds() < kRaidLateSec ? kRaidSizeEarly :
			kRaidSizeLateMin + int(rando()) % (kRaidSizeLateMax - kRaidSizeLateMin + 1);
	}
	return size;
}

static int CountRaiders(City* CT)
{
	int n = 0;
	for (int i = 0; i < CT->Agressors.NMemb; i++)
	{
		OneObject* OB = Group[CT->Agressors.Memb[i]];
		if (OB && OB->Serial == CT->Agressors.MembSN[i] && OB->newMons->Usage == FastHorseID)n++;
	}
	return n;
}

#ifdef AILOG_ENABLED
//Once a minute: where the fast cavalry is. A game made 257 Sich Cossacks in 4 minutes and lost none, yet no
//raid left (AI log 2026-10-28 18:33, P4)
static void LogRaiders(City* CT)
{
	static int loggedAt[8];
	int now = GetGameSeconds();
	if (now / 60 == loggedAt[CT->NI] / 60 && now >= loggedAt[CT->NI])return;
	loggedAt[CT->NI] = now;
	int pool = 0, home = 0, army = 0, other = 0, loose = 0;
	const char* armyOrder = "-";
	const char* otherOrder = "-";
	int N = NtNUnits[CT->NI];
	word* units = NatList[CT->NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || OB->Sdoxlo || OB->newMons->Usage != FastHorseID)continue;
		word br = OB->BrigadeID;
		if (br == 0xFFFF)loose++;
		else if (br == CT->Agressors.ID)pool++;
		else if (br == CT->FreeArmy.ID || br == CT->Guards.ID || br == CT->Defenders.ID)home++;
		else
		{
			Brigade* BR = CT->Brigs + br;
			if (BR->ArmyID < MaxArm && CT->ARMS[BR->ArmyID].Enabled)
			{
				army++;
				AI_Army* A = CT->ARMS + BR->ArmyID;
				if (A->AOrder && A->AOrder->Message)armyOrder = A->AOrder->Message;
			}
			else
			{
				other++;
				if (BR->BOrder && BR->BOrder->Message)otherOrder = BR->BOrder->Message;
			}
		}
	}
	int raids = 0;
	for (int i = 0; i < MaxArm; i++)
	{
		AI_Army* A = CT->ARMS + i;
		if (A->Enabled && A->AOrder && A->AOrder->ALink == &A_DiversiaLink)raids++;
	}
	AILog_Event(CT->NI, "RAIDERS %d waiting (raid at %d), %d in home groups, %d in armies (order %s), %d in other brigades (order %s), %d without group; %d raids out",
		pool, RaidSize(CT), home, army, armyOrder, other, otherOrder, loose, raids);
}
#endif

//Fork: a raid ends after kRaidMaxSteps and its riders return to the pool: raids never ended before (their
//LastEnemyTime is never updated), 164 riders sat in 3 raids from minute 35 to 93, so no new raid could leave
//while 254 riders waited (AI log 2026-10-29 07:18, P6)
static const int kRaidMaxSteps = 40 * 300;//5 minutes
static int g_RaidStartAt[8][MaxArm];
//Fast cavalry that is not on a raid and not fighting returns to the raid pool about every 10 s: it kept joining
//the main armies (150-220 riders) and loose brigades (135), where Sich Cossacks are of little use
static const int kRaiderSweepPeriod = 400;
static int g_RaiderSweepAt[8];
static void EndRaid(City* CT, AI_Army* ARM)
{
	ARM->ClearZombi();
	for (int i = 0; i < ARM->NExBrigs; i++)
	{
		Brigade* BR = ARM->ExBrigs[i].Brig;
		if (BR->NMemb)BR->RemoveObjects(BR->NMemb, &CT->Agressors);
		BR->DeleteAll();
		BR->Enabled = false;
	}
	ARM->ClearArmy();
	ARM->Enabled = false;
}
static void SweepRaiders(City* CT)
{
	if (!AIPeriodDue(g_RaiderSweepAt[CT->NI], kRaiderSweepPeriod))return;
	int moved = 0;
	int N = NtNUnits[CT->NI];
	word* units = NatList[CT->NI];
	for (int i = 0; i < N; i++)
	{
		if (units[i] == 0xFFFF)continue;
		OneObject* OB = Group[units[i]];
		if (!OB || OB->Sdoxlo || OB->Hidden || OB->newMons->Usage != FastHorseID || OB->EnemyID != 0xFFFF)continue;
		word br = OB->BrigadeID;
		if (br == 0xFFFF || br == CT->Agressors.ID || br == CT->FreeArmy.ID || br == CT->Guards.ID || br == CT->Defenders.ID)continue;
		Brigade* BR = CT->Brigs + br;
		if (BR->ArmyID < MaxArm)
		{
			AI_Army* A = CT->ARMS + BR->ArmyID;
			if (A->Enabled && A->AOrder && A->AOrder->ALink == &A_DiversiaLink)continue;//on a raid
		}
		OB->Zombi = false;
		if (BR->RemoveOne(OB->BrIndex, &CT->Agressors))
		{
			OB->DoNotCall = true;
			moved++;
		}
	}
#ifdef AILOG_ENABLED
	if (moved)AILog_Event(CT->NI, "RAIDERS-BACK %d fast riders back to the raid pool", moved);
#endif
}

void CheckDiversants(City* CT)
{
#ifdef AILOG_ENABLED
	LogRaiders(CT);
#endif
	for (int i = 0; i < MaxArm; i++)
	{
		AI_Army* A = CT->ARMS + i;
		if (!(A->Enabled && A->AOrder && A->AOrder->ALink == &A_DiversiaLink))continue;
		int& at = g_RaidStartAt[CT->NI][i];
		if (tmtmt >= at && tmtmt - at < kRaidMaxSteps)continue;
#ifdef AILOG_ENABLED
		AILog_Event(CT->NI, "RAID-END a raid comes back to the raid pool");
#endif
		EndRaid(CT, A);
	}
	SweepRaiders(CT);
	int NDivers = CountRaiders(CT);
	if (NDivers < RaidSize(CT))return;
	int NDivArms = 0;
	for (int i = 0; i < MaxArm; i++)
	{
		AI_Army* AIAR = CT->ARMS + i;
		if (AIAR->Enabled && AIAR->AOrder && AIAR->AOrder->ALink == &A_DiversiaLink)NDivArms++;
	}
	if (NDivArms >= kMaxRaidsAtOnce)return;
	int old = BriMax[2];
	BriMax[2] = NDivers;
	int aid = CT->GetFreeArmy();
	if (aid != -1)
	{
		AI_Army* AIAR = CT->ARMS + aid;
		AIAR->Enabled = true;
		AIAR->CreateMinimalArmyFromBrigade(&CT->Agressors, 2);
		AIAR->Diversia();
		g_RaidStartAt[CT->NI][aid] = tmtmt;
		AIAR->Spec = 5;
		if (CT->NDivr < 255)CT->NDivr++;
#ifdef AILOG_ENABLED
		AILog_Event(CT->NI, "RAID %d riders leave (raid %d)", NDivers, CT->NDivr);
#endif
		g_RaidSize[CT->NI] = 0;//the next raid gets its own size
	}
	BriMax[2] = old;
}

static int CountArmyType(const Brigade* brigade, int armyType);
//Fork: grenadier squads (kGrenSquadSize from the raid pool's grenadiers) attack the nearest enemy mine
static const int kGrenSquadPeriod = 64;//game steps
static OneObject* NearestEnemyMine(City* CT, int x, int y)
{
	OneObject* best = nullptr;
	int bestD = 0;
	for (int e = 0; e < 8; e++)
	{
		if (NATIONS[e].NMask & CT->Nat->NMask)continue;//itself and allies
		int N = NtNUnits[e];
		word* units = NatList[e];
		for (int i = 0; i < N; i++)
		{
			if (units[i] == 0xFFFF)continue;
			OneObject* OB = Group[units[i]];
			if (!OB || OB->Sdoxlo || !OB->NewBuilding || OB->newMons->Usage != MineID)continue;
			int d = Norma((OB->RealX >> 4) - x, (OB->RealY >> 4) - y);
			if (!best || d < bestD)
			{
				best = OB;
				bestD = d;
			}
		}
	}
	return best;
}
static void DisbandGrenSquad(City* CT, int s)
{
	Brigade* BR = CT->Brigs + (g_GrenSquad[CT->NI][s] - 1);
	if (BR->Enabled)
	{
		if (BR->NMemb)BR->RemoveObjects(BR->NMemb, &CT->Agressors);
		BR->DeleteAll();
		BR->Enabled = false;
	}
	g_GrenSquad[CT->NI][s] = 0;
}
static void HandleGrenadierSquads(City* CT)
{
	int NI = CT->NI;
	if (tmtmt < g_GrenSquadAt[NI])memset(g_GrenSquad[NI], 0, sizeof(g_GrenSquad[NI]));//a new or loaded game
	if (!AIPeriodDue(g_GrenSquadAt[NI], kGrenSquadPeriod))return;
	for (int s = 0; s < kMaxGrenSquads; s++)
	{
		if (!g_GrenSquad[NI][s])continue;
		Brigade* BR = CT->Brigs + (g_GrenSquad[NI][s] - 1);
		if (!BR->Enabled || BR->NMemb < kGrenSquadMinAlive)
		{
#ifdef AILOG_ENABLED
			AILog_Event(NI, "GREN-SQUAD %d comes home (%d left)", s + 1, BR->Enabled ? int(BR->NMemb) : 0);
#endif
			DisbandGrenSquad(CT, s);
			continue;
		}
		int sx = 0, sy = 0, n = 0;
		for (int j = 0; j < BR->NMemb; j++)
		{
			OneObject* OB = Group[BR->Memb[j]];
			if (!OB || OB->Serial != BR->MembSN[j] || OB->Sdoxlo)continue;
			sx += OB->RealX >> 4;
			sy += OB->RealY >> 4;
			n++;
		}
		if (!n)continue;
		OneObject* mine = NearestEnemyMine(CT, sx / n, sy / n);
		if (!mine)
		{
#ifdef AILOG_ENABLED
			AILog_Event(NI, "GREN-SQUAD %d comes home (no enemy mine)", s + 1);
#endif
			DisbandGrenSquad(CT, s);
			continue;
		}
		for (int j = 0; j < BR->NMemb; j++)
		{
			OneObject* OB = Group[BR->Memb[j]];
			if (!OB || OB->Serial != BR->MembSN[j] || OB->Sdoxlo)continue;
			if (!OB->LocalOrder || OB->EnemyID != mine->Index)OB->AttackObj(mine->Index, 128 + 16, 0);
		}
	}
	if (!GrenadiersEnabled(CT))return;
	for (int s = 0; s < kMaxGrenSquads; s++)
	{
		if (g_GrenSquad[NI][s])continue;
		if (CountArmyType(&CT->Agressors, kGrenArmyType) < kGrenSquadSize)return;
		int b = CT->GetFreeBrigade();
		if (b == -1)return;
		Brigade* BR = CT->Brigs + b;
		BR->Enabled = true;
		int moved = 0;
		for (int j = CT->Agressors.NMemb - 1; j >= 0 && moved < kGrenSquadSize; j--)
		{
			OneObject* OB = Group[CT->Agressors.Memb[j]];
			if (!OB || OB->Serial != CT->Agressors.MembSN[j] || ArmType[OB->Usage] != kGrenArmyType)continue;
			if (CT->Agressors.RemoveOne(j, BR))moved++;
		}
		g_GrenSquad[NI][s] = b + 1;
#ifdef AILOG_ENABLED
		AILog_Event(NI, "GREN-SQUAD %d of %d grenadiers leaves for the enemy mines", s + 1, moved);
#endif
	}
}

//------------------------------MAIN STRATEGICAL BRAIN---------------------------//
void SHIP_DIVERSIA(AI_Army* ARM);

static int CountArmyType(const Brigade* brigade, int armyType)
{
	int count = 0;
	for (int i = 0; i < brigade->NMemb; i++)
	{
		word member = brigade->Memb[i];
		if (member != 0xFFFF)
		{
			OneObject* object = Group[member];
			if (object && ArmType[object->Usage] == armyType)count++;
		}
	}
	return count;
}

void SmartGamer(City* CT)
{
	//minimal armies creation from Agressors
	//assert(CT->NI==1);
	if (CT->AI_LAND_BATTLE)
	{
		//Insane AI: hold attacks until 180 pikemen + 48 dragoons are ready, then send them together
		bool combinedInsaneAssault = false;
		int AType = -1;
		if (IsInsaneAI(CT->Nat) && !CT->DefenceStage)
		{
			word barracksType = FindInsaneProducerType(CT->Nat, LightInfID, false);
			word stableType = FindInsaneProducerType(CT->Nat, HorseStrelokID, true);
			int pikes = CountArmyType(&CT->Agressors, 0);
			int dragoons = CountArmyType(&CT->Agressors, 4);
			//Dragoons count only once they can be trained (some nations have 18th-century dragoons only;
			//waiting for them before the 18th century held every attack back)
			word dragoonType = FindProducedUnitType(CT->Nat, stableType, HorseStrelokID);
			bool dragoonsTrainable = dragoonType != 0xFFFF && CT->Nat->Mon[dragoonType]->Enabled;
			//or a large army of any mix: dragoons kept dying in fights and the army waited at home for good
			const int kInsaneMassAssault = 300;
			combinedInsaneAssault = barracksType != 0xFFFF && pikes >= 180 &&
				(!dragoonsTrainable || dragoons >= 48 || CT->Agressors.NMemb >= kInsaneMassAssault);
			if (combinedInsaneAssault)AType = 0;
			else if (barracksType == 0xFFFF)AType = CheckMinArmyCreationAbility(&CT->Agressors);
		}
		else
		{
			AType = CheckMinArmyCreationAbility(&CT->Agressors);
		}
		if (AType != -1)
		{
			int Aind = CT->GetFreeArmy();
			if (Aind != -1)
			{
				if (CT->NDefArms >= CT->MaxDefArms)
				{
					CT->MaxDefArms += 64;
					CT->DefArms = (word*)realloc(CT->DefArms, CT->MaxDefArms << 1);
				};
				CT->DefArms[CT->NDefArms] = Aind;
				CT->NDefArms++;
				AI_Army* ARM = CT->ARMS + Aind;
				ARM->Enabled = true;
				ARM->CreateMinimalArmyFromBrigade(&CT->Agressors, AType);
				if (combinedInsaneAssault)ARM->CreateMinimalArmyFromBrigade(&CT->Agressors, 4);
				ARM->Parad();
				ARM->MakeBattle();
			};
		};
		//--------------Settlers---------------
		AType = CheckSuperMinArmyCreationAbility(&CT->Settlers);
		if (AType != -1)
		{
			int Aind = CT->GetFreeArmy();
			if (Aind != -1)
			{
				if (CT->NDefArms >= CT->MaxDefArms)
				{
					CT->MaxDefArms += 64;
					CT->DefArms = (word*)realloc(CT->DefArms, CT->MaxDefArms << 1);
				};
				CT->DefArms[CT->NDefArms] = Aind;
				CT->NDefArms++;
				AI_Army* ARM = CT->ARMS + Aind;
				ARM->Enabled = true;
				ARM->CreateMinimalArmyFromBrigade(&CT->Settlers, AType);
				ARM->Parad();
				SHIP_DIVERSIA(ARM);
			};
		};
	};
	//-------------------------------------
	if (RaidsEnabled(CT))CheckDiversants(CT);
	HandleGrenadierSquads(CT);
	//Fork: the base threat is counted unit by unit (ScanBaseThreat), every kBaseThreatScanPeriod steps
	if (CT->AI_TOWN_DEFENCE && AIPeriodDue(g_BaseScanAt[CT->NI], kBaseThreatScanPeriod))
	{
		int tx, ty, n;
		if (ScanBaseThreat(CT, &tx, &ty, &n))
		{
			//need to protect base;
			CT->FreeArmy.RemoveObjects(CT->FreeArmy.NMemb, &CT->Defenders);
			BaseThreat& T = g_BaseThreat[CT->NI];
#ifdef AILOG_ENABLED
			if (!T.Seen || tmtmt - T.Time >= kBaseThreatSteps)
				AILog_Event(CT->NI, "BASE-THREAT %d enemies at the base, %d px from the Town Centre: the home army answers",
					n, Norma(tx - (CT->CenterX << 7), ty - (CT->CenterY << 7)));
#endif
			T.X = tx;
			T.Y = ty;
			T.Time = tmtmt;
			T.Seen = true;
		}
	};
	if (rando() < 1024)
	{
		//determination of walls to erase

	};

};
void AI_Army::ClearZombi()
{
	for (int i = 0; i < NExBrigs; i++)
	{
		Brigade* BR = ExBrigs[i].Brig;
		word* Mem = BR->Memb;
		word* MSN = BR->MembSN;
		int N = BR->NMemb;
		for (int j = 0; j < N; j++)
		{
			word MID = Mem[j];
			if (MID != 0xFFFF)
			{
				OneObject* OB = Group[MID];
				if (OB)OB->Zombi = false;
			};
		};
	};
};
void AI_Army::SetZombi()
{
	for (int i = 0; i < NExBrigs; i++)
	{
		Brigade* BR = ExBrigs[i].Brig;
		word* Mem = BR->Memb;
		word* MSN = BR->MembSN;
		int N = BR->NMemb;
		for (int j = 0; j < N; j++)
		{
			word MID = Mem[j];
			if (MID != 0xFFFF)
			{
				OneObject* OB = Group[MID];
				if (OB)OB->Zombi = true;
			};
		};
	};
};


//-------------------Processing the danger map------------------//

//--------------------------------------------------------------//



int CheckPositionForDanger(int x, int y, int z)
{
	EnemyInfo* EIN = GNFO.EINF[CURRENTAINATION];
	DangerInfo* DIF = EIN->DINF;
	int MAXR = DIF->MaxR + 128;
	for (int i = 0; i < EIN->NDINF; i++)
	{
		int dst = Norma(DIF->x - x, DIF->y - y) + ((z - DIF->z) << 1);
		if (dst <= MAXR)return DIF->UpgradeLevel;
		DIF++;
	};
	return 0;
};
byte GetDangValue(int x, int y);
int FastCheckPosDanger(int x, int y)
{
	if (x > 0 && y > 0 && x < TopLx && y < TopLx)
	{
		int v = 0;//GetDangValue(x>>1,y>>1);
		if (v > 128)v = (v & 127) + 5; else v = 0;
		if (!GNFO.EINF[CURRENTAINATION])return 0;
		return (GNFO.EINF[CURRENTAINATION]->InflMap[x + (y << TopSH)] + v) & 255;
	}
	else return 0;
};
int AI_Army::GetArmyDanger(int xp, int yp)
{
	int MaxDang = 0;
	int lx = xp >> 6;
	int ly = yp >> 6;
	int odis = 4;
	int Nx = int(sqrt(NExBrigs));
	int Ny = Nx;
	if (Nx * Ny < NExBrigs)Nx++;
	if (Nx * Ny < NExBrigs)Ny++;
	int x0 = lx - (((Nx - 1) * odis) >> 1);
	int y0 = ly - (((Ny - 1) * odis) >> 1);
	int ps = 0;
	int nn = 0;
	int Dang = 0;
	for (int ix = 0; ix < Nx; ix++)
	{
		for (int iy = 0; iy < Ny; iy++)
		{
			if (nn < NExBrigs)
			{
				int x1 = x0 + ix * odis;
				int y1 = y0 + iy * odis;
				CheckTopPos(lx, ly, &x1, &y1);
				//int xx=(x1<<6)+32;
				//int yy=(y1<<6)+32;
				int Dang = FastCheckPosDanger(x1 - 2, y1 - 2);
				if (Dang > MaxDang)MaxDang = Dang;
				Dang = FastCheckPosDanger(x1 - 2, y1 + 1);
				if (Dang > MaxDang)MaxDang = Dang;
				Dang = FastCheckPosDanger(x1 + 2, y1 - 2);
				if (Dang > MaxDang)MaxDang = Dang;
				Dang = FastCheckPosDanger(x1 + 2, y1 + 2);
				if (Dang > MaxDang)MaxDang = Dang;
				ps += 5;
				nn++;
			};
		};
	};
	return MaxDang;
};
void AI_Army::SendToMostSafePosition()
{
	if (TopPos == -1 || TopPos >= 0xFFFE)return;
	Area* AR = TopMap + TopPos;
	int MinDang = 100000;
	int xx = 0, yy = 0;
	int N = AR->NLinks;
	for (int j = 0; j < N; j++)
	{
		int i = (int(rando()) * N) >> 15;
		Area* AR1 = TopMap + AR->Link[i + i];
		int lx = ((AR1->x) << 6) + 32;
		int ly = ((AR1->y) << 6) + 32;
		int dan = GetArmyDanger(lx, ly);
		if (dan <= MinDang)
		{
			xx = lx;
			yy = ly;
			MinDang = dan;
			if (!dan)goto Smps;
		};
	};
Smps:
	if (MinDang < 100000)
	{
		LocalSendTo(xx, yy, 128 + 64, 1);
	};
};
//-----------------------Search object for the grenader's attack----------------------//
int GetTopDistance(int xa, int ya, int xb, int yb)
{
	int Top1 = SafeTopRef(xa, ya);
	int Top2 = SafeTopRef(xb, yb);
	if (Top1 >= 0xFFFE || Top2 >= 0xFFFE)return 0xFFFF;
	if (Top1 == Top2)return Norma(xa - xb, ya - yb);
	int Next1 = MotionLinks[Top1 * NAreas + Top2];
	if (Next1 == 0xFFFF)return 0xFFFF;
	if (Next1 == Top2)return Norma(xa - xb, ya - yb);
	int Next2 = MotionLinks[Top2 * NAreas + Top1];
	if (Next2 == 0xFFFF)return 0xFFFF;
	if (Next2 == Top1)return Norma(xa - xb, ya - yb);
	Area* AR1 = TopMap + Next1;
	Area* AR2 = TopMap + Next2;
	if (Next1 == Next2)return Norma(xa - AR1->x, ya - AR1->y) + Norma(xb - AR2->x, yb - AR2->y);
	return Norma(xa - AR1->x, ya - AR1->y) + Norma(xb - AR2->x, yb - AR2->y) + LinksDist[Next1 * NAreas + Next2];
};


bool SearchGreAttObject(int* x, int* y, int count, byte NI)
{
	int MinTopR = 10000000;
	int xb = 0, yb = 0;
	int x0 = *x;
	int y0 = *y;

	EnemyInfo* EIN = GNFO.EINF[NI];

	if (!EIN)
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		char* xi = Rarr[i].xi;
		char* yi = Rarr[i].yi;
		int N = Rarr[i].N;
		for (int p = 0; p < N; p++)
		{
			int x1 = x0 + xi[p];
			int y1 = y0 + yi[p];
			SafeCellInfo* SCI = EIN->GetCellInfo(x1, y1);

			if (SCI && SCI->BusyIndex == 0xFFFF)
			{
				int r = GetTopDistance(x0, y0, x1, y1);
				if (r < MinTopR)
				{
					MinTopR = r;
					xb = x1;
					yb = y1;
				}
			}
		}
	}

	if (MinTopR < (count << 1))
	{
		*x = xb;
		*y = yb;

		return true;
	}
	else
	{
		return false;
	}
}

bool FastSearchGreAttObject(int* x, int* y, int count, byte NI)
{
	int rr = tmtmt % count;
	int MinTopR = 10000000;
	int xb = 0, yb = 0;
	int x0 = *x;
	int y0 = *y;
	char* xi = Rarr[rr].xi;
	char* yi = Rarr[rr].yi;
	int N = Rarr[rr].N;

	EnemyInfo* EIN = GNFO.EINF[NI];

	if (!EIN)return false;
	for (int p = 0; p < N; p++)
	{
		int x1 = x0 + xi[p];
		int y1 = y0 + yi[p];

		SafeCellInfo* SCI = EIN->GetCellInfo(x1, y1);
		if (SCI)
		{

		}
		if (SCI && SCI->BusyIndex == 0xFFFF)
		{
			int r = GetTopDistance(x0, y0, x1, y1);

			if (r < MinTopR)
			{
				MinTopR = r;
				xb = x1;
				yb = y1;
			}
		}
	}

	if (MinTopR < (count << 1))
	{
		*x = xb;
		*y = yb;

		return true;
	}
	else return false;
}

AI_Army* GetNearestArmy(int x, int y, City* CT)
{
	AI_Army* AAR = CT->ARMS;
	int Abest = -1;
	int minR = 100000;

	for (int i = 0; i < MaxArm; i++)
	{
		if (AAR->Enabled && AAR->TopPos != 0xFFFF)
		{
			int dst = Norma(x - AAR->x, y - AAR->y);
			if (dst < minR)
			{
				minR = dst;
				Abest = i;
			}
		}

		AAR++;
	}

	if (Abest == -1)
	{
		return nullptr;
	}
	else
	{
		return CT->ARMS + Abest;
	}
}

word SearchVictim(OneObject* OBJ, int R0, int R1);

void GrenaderSupermanLink(OneObject* OBJ)
{
	word vic = SearchVictim(OBJ, 0, 250);
	EnemyInfo* EIN = GNFO.EINF[OBJ->NNUM];

	if (!EIN)
	{
		vic = 0xFFFF;
	}

	if (vic != 0xFFFF)
	{
		OBJ->AttackObj(vic, 254, 1);
		return;
	}

	int Attx = OBJ->LocalOrder->info.AttackXY.x;
	int Atty = OBJ->LocalOrder->info.AttackXY.y;

	int x0 = OBJ->RealX / 1024;
	int y0 = OBJ->RealY / 1024;

	SafeCellInfo* SCI = EIN->GetCellInfo(Attx, Atty);
	if (SCI)
	{
		if (x0 == Attx && y0 == Atty)
		{
			OBJ->AttackObj(SCI->Index, 255, 1, 1);
		}
		else
		{
			int xd = Attx << 2;
			int yd = Atty << 2;

			if (x0 < Attx)
			{
				xd += 3;
			}

			if (y0 < Atty)
			{
				yd += 3;
			}

			OBJ->CreatePath(xd, yd);
		}
	}
	else
	{
		if (SearchGreAttObject(&x0, &y0, 40, OBJ->NNUM))
		{
			OBJ->LocalOrder->info.AttackXY.x = x0;
			OBJ->LocalOrder->info.AttackXY.y = y0;

			EIN->RegisterSafePoint(OBJ, x0, y0);
		}
		else
		{
			AI_Army* AAR = GetNearestArmy(OBJ->RealX >> 4, OBJ->RealY >> 4, OBJ->Nat->CITY);
			if (AAR)
			{
				int HimTop = AAR->TopPos;
				if (HimTop != 0xFFFF)
				{
					int MyTop = GetTopology(OBJ->RealX >> 4, OBJ->RealY >> 4);
					if (MyTop != 0xFFFF)
					{
						int NextTop = HimTop;
						if (MyTop != HimTop)
						{
							NextTop = MotionLinks[MyTop * NAreas + HimTop];
						}

						if (NextTop != 0xFFFF)
						{
							if (NextTop == HimTop)
							{
								//connect
								OBJ->ClearOrders();
								City* CT = OBJ->Nat->CITY;
								int id = CT->GetFreeBrigade();
								if (id != -1)
								{
									Brigade* BRR = CT->Brigs + id;
									BRR->Enabled = true;
									BRR->AddObject(OBJ);
									AAR->AddBrigade(BRR);
									BRR->DeleteAll();
									BRR->Enabled = false;
								}

								return;
							}
							else
							{
								//moving to...
								Area* AR1 = TopMap + NextTop;
								OBJ->NewMonsterSendTo(int(AR1->x) << 10, int(AR1->y) << 10, 255, 1);
								return;
							}
						}
					}
				}
			}
		}
	}
}

void GrenaderSuperman(OneObject* OB)
{
	if (OB->UnlimitedMotion)
	{
		return;
	}

	byte use = OB->newMons->Usage;

	if (use != GrenaderID || use != ArcherID)
	{
		return;
	}

	int x0 = OB->RealX >> 10;
	int y0 = OB->RealY >> 10;

	if (OB->EnemyID != 0xFFFF)
	{
		return;
	}

	EnemyInfo* EIN = GNFO.EINF[OB->NNUM];

	if (SearchGreAttObject(&x0, &y0, 30, OB->NNUM))
	{
		if (OB->BrigadeID != 0xFFFF)
		{
			Brigade* BR = OB->Nat->CITY->Brigs + OB->BrigadeID;
			BR->FreeMember(OB->BrIndex);
		}

		Order1* OR1 = OB->CreateOrder(0);

		if (OR1)
		{
			OR1->info.AttackXY.x = x0;
			OR1->info.AttackXY.y = y0;
			OR1->info.AttackXY.wep = 0;
			OR1->info.AttackXY.ox = 0xFFFF;
			OR1->info.AttackXY.oy = 0xFFFF;
			OR1->PrioryLevel = 255;

			OR1->DoLink = &GrenaderSupermanLink;
			OR1->OrderType = 99;
			OR1->OrderTime = 0;
			OB->PrioryLevel = 254;
			EIN->RegisterSafePoint(OB, x0, y0);
		}
	}
}

void DeleteFromGroups(byte NI, word ID)
{
	City* CT = CITY + NI;
	int N = CT->NGroups;
	for (int i = 0; i < N; i++)
	{
		word* GRP = CT->GroupsSet[i];
		int NG = CT->NGroupsInSet[i];
		for (int j = 0; j < NG; j++)
		{
			if (GRP[j] == ID)
			{
				if (j < NG - 1)
				{
					memcpy(GRP + j, GRP + j + 1, (NG - j - 1) << 1);
				}

				NG--;
				j = NG;
				CT->NGroupsInSet[i] = NG;

				if (!NG)
				{
					free(CT->GroupsSet[i]);

					if (i < N - 1)
					{
						memcpy(CT->GroupsSet + i, CT->GroupsSet + i + 1, (N - 1 - i) * sizeof(word*));
					}

					N--;
					i = N;
					CT->NGroups = N;
				}
			}
		}
	}
}

void UnGroupSelectedUnits(byte NI)
{
	word FormsIDS[256];
	int NForms = 0;
	int N = NSL[NI];
	word* SID = Selm[NI];
	word* SNS = SerN[NI];
	City* CT = CITY + NatRefTBL[NI];
	for (int i = 0; i < N; i++)
	{
		word MID = SID[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->Serial == SNS[i] && OB->BrigadeID != 0xFFFF && !OB->Sdoxlo)
			{
				word BID = OB->BrigadeID;
				for (int p = 0; p < NForms; p++)
				{
					if (FormsIDS[p] == BID)BID = 0xFFFF;
				}

				if (BID != 0xFFFF && NForms < 256)
				{
					FormsIDS[NForms] = BID;
					NForms++;
				}
			}
		}
	}

	for (int i = 0; i < NForms; i++)
	{
		DeleteFromGroups(NI, FormsIDS[i]);
	}
}

void GroupSelectedFormations(byte NI)
{
	word FormsIDS[256];
	int NForms = 0;
	int N = NSL[NI];
	word* SID = Selm[NI];
	word* SNS = SerN[NI];
	City* CT = CITY + NatRefTBL[NI];
	for (int i = 0; i < N; i++)
	{
		word MID = SID[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->Serial == SNS[i] && OB->BrigadeID != 0xFFFF && !OB->Sdoxlo)
			{
				word BID = OB->BrigadeID;
				for (int p = 0; p < NForms; p++)
				{
					if (FormsIDS[p] == BID)
					{
						BID = 0xFFFF;
					}
				}

				if (BID != 0xFFFF && NForms < 256)
				{
					FormsIDS[NForms] = BID;
					NForms++;
				}
			}
		}
	}

	for (int i = 0; i < NForms; i++)
	{
		DeleteFromGroups(NI, FormsIDS[i]);
	}

	if (NForms)
	{
		CT->GroupsSet = (word**)realloc(CT->GroupsSet, (CT->NGroups + 1) * sizeof(word*));
		CT->NGroupsInSet = (word*)realloc(CT->NGroupsInSet, CT->NGroups * 2 + 2);
		CT->GroupsSet[CT->NGroups] = new word[NForms];
		memcpy(CT->GroupsSet[CT->NGroups], FormsIDS, NForms * 2);
		CT->NGroupsInSet[CT->NGroups] = NForms;
		CT->NGroups++;
	}
}

void ImSelBrigade(byte NI, byte Type, int ID);

void CorrectImSelectionInGroups(byte NI)
{
	word FormsIDS[256];
	int NForms = 0;
	int N = ImNSL[NI];
	word* SID = ImSelm[NI];
	word* SNS = ImSerN[NI];
	City* CT = CITY + NatRefTBL[NI];
	for (int i = 0; i < N; i++)
	{
		word MID = SID[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->Serial == SNS[i] && OB->BrigadeID != 0xFFFF && !OB->Sdoxlo)
			{
				word BID = OB->BrigadeID;

				for (int p = 0; p < NForms; p++)
				{
					if (FormsIDS[p] == BID)BID = 0xFFFF;
				}

				if (BID != 0xFFFF && NForms < 256)
				{
					FormsIDS[NForms] = BID;
					NForms++;
				}
			}
		}
	}

	int N3 = NForms;
	for (int i = 0; i < N3; i++)
	{
		word ID = FormsIDS[i];
		for (int j = 0; j < CT->NGroups; j++)
		{
			int N1 = CT->NGroupsInSet[j];
			word* IDS = CT->GroupsSet[j];
			for (int p = 0; p < N1; p++)if (IDS[p] == ID)
			{
				for (int v = 0; v < N1; v++)
				{
					word ID1 = IDS[v];

					for (int z = 0; z < NForms; z++)
					{
						if (FormsIDS[z] == ID1)ID1 = 0xFFFF;
					}

					if (ID1 != 0xFFFF && NForms < 256)
					{
						FormsIDS[NForms] = ID1;
						NForms++;
					}
				}
			}
		}
	}

	for (int i = 0; i < NForms; i++)
	{
		ImSelBrigade(NI, 1, FormsIDS[i]);
	}
}

bool CheckGroupPossibility(byte NI)
{
	word* SID = ImSelm[NI];
	word* SNS = ImSerN[NI];
	int N = ImNSL[NI];
	for (int j = 0; j < N; j++)
	{
		word MID = SID[j];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->Serial == SNS[j] && !OB->Sdoxlo)
			{
				if (OB->BrigadeID == 0xFFFF)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool CheckUnGroupPossibility(byte NI)
{
	word FormsIDS[256];
	int NForms = 0;
	int N = ImNSL[NI];
	word* SID = ImSelm[NI];
	word* SNS = ImSerN[NI];
	City* CT = CITY + NatRefTBL[NI];
	for (int i = 0; i < N; i++)
	{
		word MID = SID[i];
		if (MID != 0xFFFF)
		{
			OneObject* OB = Group[MID];
			if (OB && OB->Serial == SNS[i] && OB->BrigadeID != 0xFFFF && !OB->Sdoxlo)
			{
				word BID = OB->BrigadeID;
				for (int p = 0; p < NForms; p++)if (FormsIDS[p] == BID)BID = 0xFFFF;
				if (BID != 0xFFFF && NForms < 256)
				{
					FormsIDS[NForms] = BID;
					NForms++;
				}
			}
		}
	}

	for (int i = 0; i < CT->NGroups; i++)if (NForms == CT->NGroupsInSet[i])
	{
		bool FOUND = 1;
		for (int j = 0; j < NForms; j++)
		{
			word ID = FormsIDS[j];
			word* CIDS = CT->GroupsSet[i];
			int N2 = CT->NGroupsInSet[i];
			bool OK = false;
			for (int p = 0; p < N2; p++)if (CIDS[p] == ID)OK = true;
			if (!OK)FOUND = 0;
		};
		if (FOUND)return true;
	};
	return false;
}

bool CheckWallForKilling(OneObject* OB)
{
	if (OB->Wall && OB->WallX > 0 && OB->WallY > 0 && OB->WallX < TopLx - 1 && OB->WallY < TopLy - 1)
	{

		int LI = GetLI(OB->WallX, OB->WallY);
		int TPL = TopRef[LI - 1];

		if (TPL == 0xFFFF)
			TPL = TopRef[LI - 2];

		int TPR = TopRef[LI + 1];

		if (TPR == 0xFFFF)
			TPR = TopRef[LI + 2];

		int TPU = TopRef[LI - TopLx];
		int TPD = TopRef[LI + TopLx];

		if (TPD == 0xFFFF)
			TPD = TopRef[LI + TopLx + TopLx];

		if ((TPL < NAreas || TPR < NAreas) && (TPU < NAreas || TPD < NAreas))
			return true;

		if ((TPL == TPR && TPL < NAreas) || (TPU == TPD && TPU < NAreas))
			return false;

		if (TPL < NAreas && TPR < NAreas)
		{
			if (LinksDist[TPL * NAreas + TPR] < 35)
				return false;
		}

		if (TPU < NAreas && TPD < NAreas)
		{
			if (LinksDist[TPU * NAreas + TPD] < 35)
				return false;
		}

		return true;
	}
	else return false;
}
int GetBestVictimForArchers(byte NI, int x, int y, int R, int MyTop)
{
	if (MyTop < 0 || MyTop >= NAreas)
		return 0xFFFF;

	char ARRAY[64 * 64];
	word BESTID[64 * 64];
	memset(ARRAY, 0, sizeof(ARRAY));
	memset(BESTID, 0xFF, sizeof(BESTID));

	int minx = (x - 1024 + 64) << 4;
	int miny = (y - 1024 + 64) << 4;
	int maxx = (x + 1024 - 64) << 4;
	int maxy = (y + 1024 - 64) << 4;
	int x0 = (x - 1024) << 4;
	int y0 = (y - 1024) << 4;
	byte Mask = 1 << NI;

	// 1) Scan all objects
	for (int i = 0; i < MAXOBJECT; ++i) {
		OneObject* OB = Group[i];
		if (!OB) continue;
		int rx = OB->RealX, ry = OB->RealY;
		if (rx <= minx || rx >= maxx || ry <= miny || ry >= maxy) continue;
		if (OB->Sdoxlo) continue;

		int ofs = ((rx - x0) >> 9) + (((ry - y0) >> 9) << 6);
		if (ofs < 0 || ofs >= 4096) continue;

		if (OB->newMons->Artilery) continue;

		if (OB->Wall && !(OB->NMask & Mask)) {
			if (!CheckWallForKilling(OB)) continue;
			int life = OB->Life;
			ARRAY[ofs] += (life < 4000 ? 30 : life < 8000 ? 20 : life < 20000 ? 8 : 3);
			BESTID[ofs] = OB->Index;
		}
		else if (OB->NewBuilding && !(OB->NMask & Mask)) {
			byte use = OB->newMons->Usage;
			ARRAY[ofs] += (use == TowerID || use == MineID || use == CenterID
				? 50
				: OB->Life < 4000 ? 30
				: OB->Life < 8000 ? 20
				: OB->Life < 20000 ? 8 : 3);
			BESTID[ofs] = OB->Index;
		}
		else {
			if (OB->NMask & Mask) {
				static const int off9[9] = { 0,1,-1,64,-64,63,-63,65,-65 };
				for (int d : off9) {
					int idx = ofs + d;
					if (idx >= 0 && idx < 4096) ARRAY[idx]--;
				}
			}
			else {
				static const int off9[9] = { 0,1,-1,64,-64,63,-63,65,-65 };
				for (int d : off9) {
					int idx = ofs + d;
					if (idx >= 0 && idx < 4096) ARRAY[idx]++;
				}
				BESTID[ofs] = OB->Index;
			}
		}
	}

	// 2) Find best target
	int tx0 = x >> 6;
	int ty0 = y >> 6;
	int MAXVAL = 0;
	word BESTENID = 0xFFFF;

	for (int i = 0; i < 4096; ++i) {
		word bid = BESTID[i];
		int  v = ARRAY[i];
		if (bid == 0xFFFF || v <= 0) continue;

		int ix = (i & 63) - 32;
		int iy = (i >> 6) - 32;
		int Ndist = Norma(ix, iy);
		if (Ndist < 8) Ndist = 8;
		if (Ndist > R) continue;

		int score = (v << 8) / Ndist;
		if (score <= MAXVAL) continue;

		int tx = tx0 + (ix >> 1);
		int ty = ty0 + (iy >> 1);
		OneObject* OB = Group[bid];

		bool ok = false;
		if (OB->Wall) ok = true;
		else if (tx > 0 && tx < TopLx && ty > 0 && ty < TopLy) {
			int FTOP = TopRef[tx + (ty << TopSH)];
			ok = (FTOP < NAreas) &&
				(FTOP == MyTop || LinksDist[FTOP * NAreas + MyTop] < 40);
		}
		if (ok) {
			MAXVAL = score;
			BESTENID = bid;
		}
	}

	return BESTENID;
}


bool HandleArchers(AI_Army* ARM, int R)
{
	word ARCHID[512];
	int NARCH = 0;
	int N = ARM->NExBrigs;
	int xc = 0;
	int yc = 0;
	int XCT = 0;
	int YCT = 0;
	int NCT = 0;

	for (int i = 0; i < N; i++)
	{
		Brigade* BR = ARM->ExBrigs[i].Brig;
		int NM = BR->NMemb;
		word* MEM = BR->Memb;
		word* MSN = BR->MembSN;

		for (int v = 0; v < NM; v++)
		{
			word MID = MEM[v];
			if (MID != 0xFFFF)
			{
				OneObject* OB = Group[MID];
				if (OB && (!OB->Sdoxlo))
				{
					if (OB->newMons->Archer)
					{
						if (NARCH < 512)
						{
							ARCHID[NARCH] = OB->Index;
							NARCH++;
							xc += OB->RealX >> 4;
							yc += OB->RealY >> 4;
							XCT += OB->RealX >> 4;
							YCT += OB->RealY >> 4;
							NCT++;
						}
					}
					else
					{
						XCT += OB->RealX;
						YCT += OB->RealY;
						NCT += 16;
					}
				}
			}
		}
	}

	if (NARCH)
	{
		XCT /= NCT;
		YCT /= NCT;
		word ID = GetBestVictimForArchers(ARM->NI, XCT, YCT, R, ARM->TopPos);
		if (ID != 0xFFFF)
		{
			for (int i = 0; i < NARCH; i++)
			{
				Group[ARCHID[i]]->AttackObj(ID, 16 + 128, 0, 0);
			}
			return true;
		}
	}
	return false;
}