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
int timerLength = 60;
int lasttrack = 0;

int checkflag = 0;

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
	int* musicother = (int*)0x9B6318;
	if (logging) fprintf(fp, "pre music state %d : %d : %d  \n", *musicstate, *musicoverride, *musicother);
	*musicoverride = 0;
	//Leave the hanger music alone
	if (*musicstate != 1102 && *musicstate != 1108 && *musicstate != 1106)
	{
		if (lasttrack == 0)
		{
			lasttrack = *musicstate;
		}
		else
		{
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
				if (*musicstate == 1102 || *musicstate == 1108)
				{
					lasttrack = *musicstate;
				}
				*musicoverride = lasttrack;
			}
		}
		timer++;
	}
	else
	{
		if (logging) fprintf(fp, "Filler Tracks... \n");
	}
	if (logging) fprintf(fp, "post music state %d : %d : %d \n", *musicstate, *musicoverride, *musicother);
	musicstate = musicoverride;
	((void(*)())0x0049ADE0)();//plays music
	if (logging) {
		fclose(fp);
	}

	return 0;
}