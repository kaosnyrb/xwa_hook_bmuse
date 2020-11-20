#include "targetver.h"
#include "bmuse.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>

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
/*	FILE* fp;
	errno_t err;
	if ((err = fopen_s(&fp, "hooklog.txt", "w+")) != 0)
	{
		printf("File was not opened\n");
	}
	else
	{
		fprintf(fp, "Logging from testhook \n");
	}
	fclose(fp);*/
	return 0;// OptLoad(opt.c_str());
}