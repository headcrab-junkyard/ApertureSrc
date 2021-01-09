/// @file

#include "quakedef.h"
#include "ModelCache.hpp"

void CModelCache::Init()
{
	Mod_Init();
};

void CModelCache::ClearAll()
{
	Mod_ClearAll();
};

model_t *CModelCache::GetForName(const char *name, bool crash) const
{
	return Mod_ForName(name, crash);
};

void CModelCache::TouchModel(const char *name)
{
	Mod_TouchModel(name);
};

model_t *CModelCache::FindName(const char *name) const
{
	return Mod_FindName(name);
};