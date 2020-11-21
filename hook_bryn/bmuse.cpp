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

int testhook(int* params)
{
	//const char* argOpt = (char*)params[0];
//	const unsigned short argModelIndex = (unsigned short)params[0x60];

//	const auto OptLoad = (int(*)(const char*))0x004CC940;
	FILE* fp;
	errno_t err;
	if ((err = fopen_s(&fp, "hooklog.txt", "a")) != 0)
	{
		printf("File was not opened\n");
	}

	fprintf(fp, "Logging from testhook \n");

	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	const int currentPlayerId = *(int*)0x08C1CC8;
	const char* xwaMissionFileName = (const char*)0x06002E8;

	int* musicval = (int*)0x694070;
	*musicval = 1135;
//	fprintf(fp, "playing music %d \n", *musicval);
	((void(*)())0x0049ADE0)();//plays music
	fclose(fp);
	return 0;
}