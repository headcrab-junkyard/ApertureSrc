#include "quakedef.h"

#include "WADCache.hpp"

void CWADCache::LoadWadFile(const char *filename)
{
	W_LoadWadFile(filename);
};

void CWADCache::CleanupName(const char *in, char *out)
{
	W_CleanupName(in, out);
};

lumpinfo_t *CWADCache::GetLumpInfo(const char *name)
{
	return W_GetLumpinfo(name);
};

void *CWADCache::GetLumpName(const char *name)
{
	return W_GetLumpName(name);
};

void *CWADCache::GetLumpNum(int num)
{
	return W_GetLumpNum(num);
};