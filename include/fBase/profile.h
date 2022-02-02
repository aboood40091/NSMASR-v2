#pragma once

#include <kamek.h>
#include <fBase/fBase.h>
#include <profileid.h>

// Documentation courtesy of AboodXD

struct SpriteData {
    u16 profileId;
    s32 xoffs;
    s32 yoffs;
    s32 spawnRangeOffset[2];
    s32 spawnRangeSize[2];
    u16 _1C[2];
    u16 _20[2];
    u16 flags;
};

typedef fBase_c* (*buildFunc) (void);

class Profile {
	public:
		Profile(buildFunc func, u32 spriteId, const SpriteData* spriteData, u16 executeOrderProfileId, u16 drawOrderProfileId, u32 unk, const char* name, const char** files=NULL);

		buildFunc func;
		u16 executeOrderProfileId;
		u16 drawOrderProfileId;
		u32 _8; // This field is only present in a few profiles, but having it here shouldn't hurt
};
