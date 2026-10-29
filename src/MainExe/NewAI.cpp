
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "fog.h"
#include "Megapolis.h"

#include <assert.h>
#include "walls.h"
#include "mode.h"
#include "GSound.h"
#include "MapSprites.h"
#include "NewMon.h"
#include "Math.h"
#include "GP_Draw.h"
#include "RealWater.h"
#include "NewUpgrade.h"
#include "ZBuffer.h"
#include "Path.h"
#include "Transport.h"
#include "3DBars.h"
#include "CDirSnd.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "Danger.h"
#include "StrategyResearch.h"
#include "Safety.h"
#include "EinfoClass.h"
class BuildingAI
{
public:
	word ID;
	word SN;
	short sx;
	short sy;
	int MoreInfo;
};
class BuildingProject
{
public:
	short apprX;
	short apprY;
	byte Stage;//0-search place,1-removing units,2-search for builders
	int Time;
	int PrecX;
	int PrecY;
	int PushOutRadius;
	int Razbros;
};
class Kingdom
{
public:
	word NSklad;
	word MaxSklad;
	BuildingAI* Sklads;
	word NFarms;
	word MaxFarm;
	word OnLandPeasants[1024];
	word NPeasants;
	bool BuildProjectPresent;
	BuildingProject BPR;
};
//01-wood(trees>2)
//02-stone(stones>5)
//04-full empty
//08-empty from locking
//16-plane zone
//32-high plate
byte* InfoMap;
//01-sklad
//02-farm
//04-tower
//08-building
//16-melnica
byte* CantBuild;

int* MineList;
word NMines;
word MaxMine;
void CreateInfoMap()
{
	memset(CantBuild, 0, VAL_SPRNX * VAL_SPRNX);
	int mmax = msx >> 2;
	int mmay = msy >> 2;
	for (int y = 0; y <= mmay; y++)
	{
		for (int x = 0; x <= mmax; x++)
		{
			byte ms = 0;
			if (!CheckBar(x << 3, y << 3, 8, 8))ms |= 8;
			int x0 = x << 7;
			int y0 = y << 7;
			int H1 = GetHeight(x0, y0);
			int H2 = GetHeight(x0 + 128, y0);
			int H3 = GetHeight(x0, y0 + 128);
			int H4 = GetHeight(x0, y0 + 128);
			int HH = (H1 + H2 + H3 + H4) >> 2;
			if (abs(H1 - HH) < 20 && abs(H2 - HH) < 20 && abs(H3 - HH) < 20 && abs(H4 - HH) < 20)ms |= 16;
			//resource checking;
			int ofst = x + y * VAL_SPRNX;
			int N = NSpri[ofst];
			int NTrees = 0;
			int NStones = 0;
			if ((ms & 8) && (ms & 16) && !N)ms |= 4;
			int* spr = SpRefs[ofst];
			for (int i = 0; i < N; i++)
			{
				word SID = spr[i];
				OneSprite* OS = &Sprites[SID];
				SprGroup* SG = OS->SG;
				ObjCharacter* OC = &SG->ObjChar[OS->SGIndex];
				if (OC->ResType == TreeID)NTrees++;
				if (OC->ResType == StoneID)NStones++;
			};
			if (NTrees > 2)ms |= 1;
			if (NStones > 5)ms |= 2;
			if (NTrees || NStones)ms &= ~4;
			InfoMap[ofst] = ms;
		};
	};
	if (MineList)free(MineList);
	NMines = 0;
	MineList = new int[256];
	MaxMine = 256;
	for (int i = 0; i < MaxSprt; i++)
	{
		OneSprite* OS = &Sprites[i];
		if (OS->Enabled)
		{
			SprGroup* SG = OS->SG;
			ObjCharacter* OC = &SG->ObjChar[OS->SGIndex];
			if (OC->IntResType < 8)
			{
				if (NMines >= MaxMine)
				{
					MaxMine += 256;
					MineList = (int*)realloc(MineList, MaxMine << 2);
				};
				MineList[NMines] = i;
				NMines++;
			};
		};
	};
};
//x,y-pixel coordinates
void CHKS();
void RenewInfoMap(int x, int y)
{
	x = (x >> 7) - 1;
	y = (y >> 7) - 1;
	for (int dx = 0; dx < 3; dx++)
	{
		for (int dy = 0; dy < 3; dy++)
		{
			int sx = x + dx;
			int sy = y + dy;
			if (sx >= 0 && sy >= 0)
			{
				int ofst = sx + sy * VAL_SPRNX;
				byte ms = InfoMap[ofst];
				ms &= 16;
				if (!CheckBar(sx << 3, sy << 3, 8, 8))ms |= 8;
				int N = NSpri[ofst];
				int NTrees = 0;
				int NStones = 0;
				if ((ms & 8) && (ms & 16) && !N)ms |= 4;
				int* spr = SpRefs[ofst];
				for (int i = 0; i < N; i++)
				{
					word SID = spr[i];
					OneSprite* OS = &Sprites[SID];
					SprGroup* SG = OS->SG;
					ObjCharacter* OC = &SG->ObjChar[OS->SGIndex];
					if (OC->ResType == TreeID)NTrees++;
					if (OC->ResType == StoneID)NStones++;
				};
				if (NTrees)ms |= 1;
				if (NStones)ms |= 2;
				InfoMap[ofst] = ms;
			};
		};
	};
};
void FreeInfoMap()
{
	free(MineList);
};
//special search procedures
//1.Melnica&field

int SsMaxX;
int SsMaxY;
#define FieldSX 2
//Fork: free cells (128 px) around a mill; the wide check is tried first so that its fields are not hemmed in
//by houses and other buildings (a house beside a mill left room for half its fields)
static const int kMillWideFieldCells = 3;
static bool CheckMelnicaR(int x, int y, int R)
{
	if (x <= R || y <= R || x >= SsMaxX - R || y >= SsMaxY - R)return false;
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Melnica)return false;
	ofst -= R + VAL_SPRNX * R;
	for (int dy = -R; dy <= R; dy++)
	{
		for (int dx = -R; dx <= R; dx++)
		{
			if (!(InfoMap[ofst] & 4))return false;
			ofst++;
		};
		ofst += VAL_SPRNX - R - R - 1;
	};
	return true;
};
bool CheckMelnica(int x, int y)
{
	return CheckMelnicaR(x, y, FieldSX);
};
bool CheckMelnicaWide(int x, int y)
{
	return CheckMelnicaR(x, y, kMillWideFieldCells);
};
bool CheckStoneSklad(int x, int y)
{
	if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)return false;
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Sklad)return false;
	if (!(InfoMap[ofst] & 4))return false;
	if ((InfoMap[ofst - VAL_SPRNX] & 2) || (InfoMap[ofst + VAL_SPRNX] & 2) ||
		(InfoMap[ofst + 1] & 2) || (InfoMap[ofst - 1] & 2) ||
		(InfoMap[ofst - VAL_SPRNX - 1] & 2) || (InfoMap[ofst - VAL_SPRNX + 1] & 2) ||
		(InfoMap[ofst + VAL_SPRNX - 1] & 2) || (InfoMap[ofst - VAL_SPRNX + 1] & 2))return true;
	return false;
};
bool CheckWoodSklad(int x, int y)
{
	if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)return false;
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Sklad)return false;
	if (!(InfoMap[ofst] & 4))return false;
	if ((InfoMap[ofst - VAL_SPRNX] & 1) || (InfoMap[ofst + VAL_SPRNX] & 1) ||
		(InfoMap[ofst + 1] & 1) || (InfoMap[ofst - 1] & 1) ||
		(InfoMap[ofst - VAL_SPRNX - 1] & 1) || (InfoMap[ofst - VAL_SPRNX + 1] & 1) ||
		(InfoMap[ofst + VAL_SPRNX - 1] & 1) || (InfoMap[ofst - VAL_SPRNX + 1] & 1))return true;
	return false;
};
bool CheckStoneWoodSklad(int x, int y)
{
	if (x <= 2 || y <= 2 || x >= SsMaxX - 2 || y >= SsMaxY - 2)return false;
	int ofst = (x - 2) + (y - 2) * VAL_SPRNX;
	int of1 = ofst + 2 + (2 << SprShf);
	if (CantBuild[of1] & CB_Sklad)return false;
	if (!(InfoMap[of1] & 4))return false;
	bool tpresent = false;
	for (int ix = 0; ix < 5; ix++)
	{
		for (int iy = 0; iy < 5; iy++)
		{
			if (InfoMap[ofst + ix + (iy << SprShf)] & 2)tpresent = true;
		};
	};
	if (!tpresent)return false;
	for (int ix = 0; ix < 5; ix++)
	{
		for (int iy = 0; iy < 5; iy++)
		{
			if (InfoMap[ofst + ix + (iy << SprShf)] & 1)return true;
		};
	};
	/*
	if(!((InfoMap[ofst-VAL_SPRNX]&2)||(InfoMap[ofst+VAL_SPRNX]&2)||
	   (InfoMap[ofst+1]&2)||(InfoMap[ofst-1]&2)||
	   (InfoMap[ofst-VAL_SPRNX-1]&2)||(InfoMap[ofst-VAL_SPRNX+1]&2)||
	   (InfoMap[ofst+VAL_SPRNX-1]&2)||(InfoMap[ofst-VAL_SPRNX+1]&2)))return false;
	if((InfoMap[ofst-VAL_SPRNX]&1)||(InfoMap[ofst+VAL_SPRNX]&1)||
	   (InfoMap[ofst+1]&1)||(InfoMap[ofst-1]&1)||
	   (InfoMap[ofst-VAL_SPRNX-1]&1)||(InfoMap[ofst-VAL_SPRNX+1]&1)||
	   (InfoMap[ofst+VAL_SPRNX-1]&1)||(InfoMap[ofst-VAL_SPRNX+1]&1))return true;
	*/
	return false;
}

bool CheckFarm(int x, int y)
{
	if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)
	{
		return false;
	}
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Farm)
	{
		return false;
	}
	bool retval = (InfoMap[ofst] & 4) != 0;
	return retval;
}

bool CheckBuilding(int x, int y)
{
	if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)return false;
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Building)return false;
	return (InfoMap[ofst] & 4) && (InfoMap[ofst + 1] & 4) && (InfoMap[ofst + VAL_SPRNX] & 4) && (InfoMap[ofst + VAL_SPRNX + 1] & 4);
}

byte NPORTS;
short PORTSX[32];
short PORTSY[32];
bool CheckTower(int x, int y)
{
	for (int i = 0; i < NPORTS; i++)
	{
		if (Norma(int(PORTSX[i]) - x, int(PORTSY[i]) - y) < 9)return false;
	};
	if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)return false;
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Building)return false;
	return (InfoMap[ofst] & 4) && (InfoMap[ofst + 1] & 4) && (InfoMap[ofst + VAL_SPRNX] & 4) && (InfoMap[ofst + VAL_SPRNX + 1] & 4);
};
bool CheckPort(int x, int y)
{
	if (x <= 5 || y <= 5 || x >= SsMaxX - 5 || y >= SsMaxY - 5)return false;
	for (int i = 0; i < NPORTS; i++)
	{
		if (Norma(int(PORTSX[i]) - x, int(PORTSY[i]) - y) < 9)return false;
	};
	int ofst = x + y * VAL_SPRNX;
	if (CantBuild[ofst] & CB_Port)return false;
	x <<= 2;
	y <<= 2;
	int ofsw = x + 2 + (y + 2) * (MaxWX);
	int DY = MAPSX * 10;
	int DX = 20;
	return !(WaterDeep[ofsw - DY] < 128 && WaterDeep[ofsw + DY] < 128 && WaterDeep[ofsw - DX] < 128 && WaterDeep[ofsw + DX] < 128 &&
		WaterDeep[ofsw - DY + DX] < 128 && WaterDeep[ofsw - DY - DX] < 128 && WaterDeep[ofsw + DY - DX] < 128 && WaterDeep[ofsw + DY + DX] < 128);
};
extern int CURRENTAINATION;
int GetTopology(int x, int y);
bool SearchPlace(int* xx1, int* yy1, SearchFunction* SFN, int r)
{
	SsMaxX = msx >> 2;
	SsMaxY = msy >> 2;
	int x = *xx1;
	int y = *yy1;
	if (SFN(x, y))
	{
		//assert(y<17648);
		return true;
	};
	int MyTop = GetTopology(x << 7, y << 7);
	if (MyTop >= 0xFFFE)return false;
	int REALMYTOP = MyTop;
	MyTop *= NAreas;
	int PrevTopDst = 0;
	for (int i = 1; i < r; i++)
	{
		char* xi = Rarr[i].xi;
		char* yi = Rarr[i].yi;
		int N = Rarr[i].N;
		for (int j = 0; j < N; j++)
		{
			int xx = x + xi[j];
			int yy = y + yi[j];
			if (xx > 0 && yy > 0 && xx < SsMaxX && yy < SsMaxY)
			{
				if (!GNFO.EINF[CURRENTAINATION]->GetSafeVal(xx, yy))
				{
					int top = TopRef[xx + xx + (yy << (TopSH + 1))];
					if (top < 0xFFFE)
					{
						int TDST = LinksDist[MyTop + top];
						if (top == REALMYTOP)TDST = 0;
						if (TDST - PrevTopDst < 24)
						{
							if (TDST > PrevTopDst)PrevTopDst = TDST;
							if (SFN(xx, yy))
							{
								*xx1 = xx;
								*yy1 = yy;
								return true;
							};
						};
					};
				};
			};
		};
	};
	return false;
};
//Fork: storehouse place for the AI. A free spot at the edge of a forest or stone field (unserved wood or stone
//within kSkladAdjacent) with the most wood and stone within kSkladReach that no storehouse or Town Centre of the
//player serves yet (within kSkladServed), nearer cells weighing more, less its distance from the start point.
//Before, the first spot next to any tree or stone on a spiral from the Town Centre was taken, so new storehouses
//went next to old ones or wherever there was room; without the edge rule a spot halfway between two forests
//won because it reached both. Cells are InfoMap cells (128 px).
static const int kSkladReach = 5;        //wood and stone this close to the spot count
static const int kSkladAdjacent = 2;     //the spot touches a forest or stone field: unserved wood/stone this close
static const int kSkladServed = 6;       //wood and stone this close to a storehouse or Town Centre are served already
static const int kSkladMinCells = 6;     //fewer unserved wood/stone cells: not worth a storehouse
static const int kSkladWeightFactor = 2; //a cell weighs kSkladReach + 1 - its distance; times this vs cells of distance
static const int kSkladMaxRadius = 40;
static const int kSkladGrid = 2 * (kSkladMaxRadius + kSkladReach) + 1;
bool SearchSkladPlace(int* xx1, int* yy1, int r, const int* depX, const int* depY, int nDep, int resources, int* resMask)
{
	if (r > kSkladMaxRadius)r = kSkladMaxRadius;
	SsMaxX = msx >> 2;
	SsMaxY = msy >> 2;
	int x0 = *xx1;
	int y0 = *yy1;
	int MyTop = GetTopology(x0 << 7, y0 << 7);
	if (MyTop >= 0xFFFE)return false;
	//unserved wood (1) and stone (2) per cell around the start point
	static byte unserved[kSkladGrid * kSkladGrid];
	int R = r + kSkladReach;
	int w = R + R + 1;
	for (int iy = 0; iy < w; iy++)
	{
		for (int ix = 0; ix < w; ix++)
		{
			int cx = x0 - R + ix;
			int cy = y0 - R + iy;
			byte v = 0;
			if (cx > 0 && cy > 0 && cx < SsMaxX && cy < SsMaxY)
			{
				v = InfoMap[cx + cy * VAL_SPRNX] & resources;
				for (int d = 0; d < nDep && v; d++)
				{
					if (Norma(cx - depX[d], cy - depY[d]) <= kSkladServed)v = 0;
				}
			}
			unserved[ix + iy * w] = v;
		}
	}
	int bestScore = 0;
	int bestX = -1;
	int bestY = -1;
	int bestMask = 0;
	for (int dy = -r; dy <= r; dy++)
	{
		for (int dx = -r; dx <= r; dx++)
		{
			int dist = Norma(dx, dy);
			if (dist > r)continue;
			int x = x0 + dx;
			int y = y0 + dy;
			if (x <= 1 || y <= 1 || x >= SsMaxX - 1 || y >= SsMaxY - 1)continue;
			int ofst = x + y * VAL_SPRNX;
			if (CantBuild[ofst] & CB_Sklad)continue;
			if (!((InfoMap[ofst] & 4) && (InfoMap[ofst + 1] & 4) && (InfoMap[ofst + VAL_SPRNX] & 4) && (InfoMap[ofst + VAL_SPRNX + 1] & 4)))continue;
			if (GNFO.EINF[CURRENTAINATION]->GetSafeVal(x, y))continue;
			int top = TopRef[x + x + (y << (TopSH + 1))];
			if (top >= 0xFFFE)continue;
			if (top != MyTop && LinksDist[MyTop * NAreas + top] >= 0xFFFF)continue;//not reachable on foot
			int cells = 0;
			int weight = 0;
			int nearest = kSkladReach + 1;
			int mask = 0;
			for (int ry = -kSkladReach; ry <= kSkladReach; ry++)
			{
				for (int rx = -kSkladReach; rx <= kSkladReach; rx++)
				{
					int d = Norma(rx, ry);
					if (d > kSkladReach)continue;
					byte v = unserved[(dx + rx + R) + (dy + ry + R) * w];
					if (v)
					{
						cells++;
						weight += kSkladReach + 1 - d;
						if (d < nearest)nearest = d;
						mask |= v;
					}
				}
			}
			if (cells < kSkladMinCells || nearest > kSkladAdjacent)continue;
			int score = weight * kSkladWeightFactor - dist;
			if (bestX == -1 || score > bestScore)
			{
				bestScore = score;
				bestX = x;
				bestY = y;
				bestMask = mask;
			}
		}
	}
	if (bestX == -1)return false;
	*xx1 = bestX;
	*yy1 = bestY;
	*resMask = bestMask;
	return true;
}

//Fork: towers on rings from rMin outwards (the edge of the base), facing the enemy first (toEnemy: xe, ye is its
//side), then any side. Before: the Town Centre's own spot first, from the second tower on the first free spot
//anywhere near the Town Centre, and the "enemy" was player 0 only (else the map corner): towers clumped inside
//the base and their fire hit its own buildings.
//Fork: of the valid tower places in kTowerHighGroundRings rings from the first one found, the highest one
//(towers see and shoot further from a hill)
static const int kTowerHighGroundRings = 4;//128 px cells
int GetHeight(int x, int y);
bool SearchTowerPlace(int* xx1, int* yy1, SearchFunction* SFN, int r, int rMin, int xc, int yc, int xe, int ye, bool toEnemy)
{
	SsMaxX = msx >> 2;
	SsMaxY = msy >> 2;
	int x = *xx1;
	int y = *yy1;
	if (r > RRad)r = RRad;
	int MyTop = GetTopology(x << 7, y << 7);
	if (MyTop >= 0xFFFE)return false;
	MyTop *= NAreas;
	for (int pass = toEnemy ? 0 : 1; pass < 2; pass++)
	{
	int PrevTopDst = 0;
	int bestX = -1;
	int bestY = -1;
	int bestH = 0;
	int bandEnd = r;
	for (int i = rMin; i < r && i < bandEnd; i++)
	{
		char* xi = Rarr[i].xi;
		char* yi = Rarr[i].yi;
		int N = Rarr[i].N;
		for (int j = 0; j < N; j++)
		{
			int xx = x + xi[j];
			int yy = y + yi[j];
			if ((pass || (xx - xc) * (xe - xc) + (yy - yc) * (ye - yc) > 0) && xx > 0 && yy > 0 && xx < SsMaxX && yy < SsMaxY)
			{
				if (!GNFO.EINF[CURRENTAINATION]->GetSafeVal(xx, yy))
				{
					int top = TopRef[xx + xx + (yy << (TopSH + 1))];
					if (top < 0xFFFE)
					{
						int TDST = LinksDist[MyTop + top];
						if (TDST - PrevTopDst < 14)
						{
							if (TDST > PrevTopDst)PrevTopDst = TDST;
							if (SFN(xx, yy))
							{
								int h = GetHeight((xx << 7) + 64, (yy << 7) + 64);
								if (bestX == -1)bandEnd = i + kTowerHighGroundRings;
								if (bestX == -1 || h > bestH)
								{
									bestX = xx;
									bestY = yy;
									bestH = h;
								}
							};
						};
					};
				};
			};
		};
	};
	if (bestX != -1)
	{
		*xx1 = bestX;
		*yy1 = bestY;
		return true;
	}
	}
	return false;
};
bool FindCenter(int* xx, int* yy, byte NI)
{
	int N = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < MAXOBJECT; i++)
	{
		OneObject* OB = Group[i];
		if (OB && OB->NNUM == NI && OB->newMons->Peasant)
		{
			x += OB->RealX >> 4;
			y += OB->RealY >> 4;
			N++;
		};
	};
	if (N)
	{
		x = (div(x, N).quot) >> 7;
		y = (div(y, N).quot) >> 7;
		*xx = x;
		*yy = y;
		return true;
	}
	else return false;
};
void SetUnusable(int x, int y, byte Mask)
{
	if (x < 0 || y < 0 || x >= SsMaxX || y >= SsMaxY)return;
	CantBuild[x + y * VAL_SPRNX] |= Mask;
};
