#include "targetver.h"
#include "bmuse.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>

#pragma pack(push, 1)

struct XwaCraft
{
	char unk000[4];
	unsigned short CraftIndex;
	int LeaderObjectIndex;
	char unk00A[29];
	char SFoilsState;
	char unk028[355];
	unsigned char m18B;
	char unk18C[212];
	unsigned char MeshRotationAngles[50];
	char unk292[359];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[10];
	int PositionX;
	int PositionY;
	int PositionZ;
	char Unk0016[127];
	unsigned short ModelIndex;
	char Unk0097[2];
	unsigned char Markings;
	char Unk009A[67];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	short m00;
	unsigned short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short m17;
	char unk19[6];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

class Config
{
public:
	Config()
	{

	}
};

Config g_config;

int timer = 0;
int timerLength = 30;
int lasttrack = 0;

int lastplayerPositionX = 0;
int loadtriggerdistance = 750; //How far the player needs to have moved before I start thinking they may have teleported.

int playerID = -1;

bool logging = false;

int testhook(int* params)
{
	FILE* fp = 0;
	errno_t err;
	if (logging)
	{
		if ((err = fopen_s(&fp, "hooklog.txt", "a")) != 0)
		{
			printf("File was not opened\n");
		}
	}

	int* musicoverride = (int*)0x694070;
	int* musicstate = (int*)0x9B631C;
	if (lasttrack == 0)
	{
		lasttrack = *musicstate;
	}
	else
	{
		//if (logging) fprintf(fp, "music state %d \n", *musicstate);
		if (timer == timerLength)
		{
			//When timer is hit we let iMuse choose the next track
			*musicoverride = 0;
		}
		else if (timer > timerLength)
		{
			//Then we set the track to whatever iMuse chose
			lasttrack = *musicstate;
			if (logging) fprintf(fp, "Timer hit, playing music %d \n", *musicstate);
			timer = 0;
		}
		else
		{
			//While we're in the timer don't let iMuse change the music. This should smooth out the track changes
			*musicoverride = lasttrack;
		}
	}
	timer++;
	// check for large changes in player position, assuming this means that a load has taken place
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	if (playerID == -1)
	{
		for (int i = 0; i < 500; i++)
		{
			if (xwaObjects[i].PlayerIndex != -1)
			{
				if (logging) fprintf(fp, " %i .PlayerIndex != -1 \n", i);
				playerID = i;
				break;
			}
		}
	}
	if (logging) fprintf(fp, "pos: %i %i \n", xwaObjects[playerID].PositionX, xwaObjects[playerID].PositionY);	
	if (lastplayerPositionX == 0)
	{
		if (xwaObjects[playerID].pMobileObject)
		{
			lastplayerPositionX = xwaObjects[playerID].pMobileObject->PositionX;
		}
	}
	else
	{
		int playerPositionX = xwaObjects[playerID].pMobileObject->PositionX;
		int distance = (playerPositionX - lastplayerPositionX);
		distance = distance / 100; //Shrink it, don't need that much accuracy.
		//Square the distance to (makes it always positive)
		distance = distance * distance;
		if (logging) fprintf(fp, "distance : %i \n", distance);
		if (distance > loadtriggerdistance)
		{
			if (logging) fprintf(fp, "distance > loadtriggerdistance \n");
			timer = timerLength;
			//Make sure the player hasn't changed position in the list, refresh it.
			for (int i = 0; i < 500; i++)
			{
				if (xwaObjects[i].PlayerIndex != -1)
				{
					if (logging) fprintf(fp, " %i .PlayerIndex != -1 \n", i);
					playerID = i;
					break;
				}
			}
		}
		lastplayerPositionX = xwaObjects[playerID].pMobileObject->PositionX;
	}
	
	((void(*)())0x0049ADE0)();//plays music
	if (logging) {
		fclose(fp);
	}

	return 0;
}