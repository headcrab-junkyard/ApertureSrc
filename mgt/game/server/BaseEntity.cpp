/// @file

#include "BaseEntity.hpp"

void CBaseEntity::SetModel(const char *name)
{
	gpEngine->pfnSetModel(self, name);
};

void CBaseEntity::SetSize(float *mins, float *maxs)
{
	gpEngine->pfnSetSize(self, mins, maxs);
};