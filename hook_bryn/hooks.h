#pragma once
#include "hook_function.h"

#include "bmuse.h"

//004F4A70 - start?
//004F5274 - end?
//{0x0F3A70, "004F4A70", "004F5274" },
//0x0F3E75

//005A8D6B   

//4F51C0
static const HookFunction g_hookFunctions[] =
{
	{ 0x592C32, testhook },
};

// Line to replace, original value, new value
// Line is near the end of the imuse call, think setting the music to play?
//mov     eax, dword_694070
// 0x0F4667 E8745BFAFF call    sub_49ADE0
//call    something?
static const HookPatchItem g_test[] =
{
	{ 0x0F4667, "E8745BFAFF", "9090909090" },
};


static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("bMuse hook", g_test)
};
