#include "targetver.h"
#include "bmuse.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>

#pragma pack(push, 1)

struct S0x08B94E0_B48
{
	char Unk0000[65];
	short IsExteriorView;
	char Unk0043[10];
};

static_assert(sizeof(S0x08B94E0_B48) == 77, "size of S0x08B94E0_B48 must be 77");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk004[19];
	unsigned char HyperspacePhase;
	char Unk018[13];
	short XwaPlayer_m025;
	char Unk027[61];
	unsigned char IsHudVisible;
	unsigned char XwaPlayer_m065;
	unsigned char XwaPlayer_m066;
	unsigned char IsHudMfd1Visible;
	unsigned char IsHudMfd2Visible;
	unsigned char XwaPlayer_m069;
	unsigned char XwaPlayer_m06A;
	char Unk06B[402];
	short MousePositionX;
	short MousePositionY;
	char Unk201[2375];
	S0x08B94E0_B48 XwaPlayer_mB48;
	char UnkB95[58];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct XwaObject
{
	char unk00[2];
	unsigned short ModelIndex;
	char unk04[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hooks.ini", "hook_hangars");

		if (lines.empty())
		{
			lines = GetFileLines("hook_hangars.cfg");
		}

		//this->SelectionMode = GetFileKeyValue(lines, "SelectionMode");

		FILE* fp;
		errno_t err;
		if ((err = fopen_s(&fp, "log.txt", "w+")) != 0)
		{
			printf("File was not opened\n");
		}
		else
		{
			fprintf(fp, "Logging from bMuse config \n");
		}
		fclose(fp);
	}
};

Config g_config;

int timer = 0;
int timerLength = 30;
int lasttrack = 0;
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
		if (logging) fprintf(fp, "music state %d \n", *musicoverride);
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
	((void(*)())0x0049ADE0)();//plays music
	if (logging) {
		fclose(fp);
	}

	return 0;
}