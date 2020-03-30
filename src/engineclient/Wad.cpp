#include "quakedef.h"
#include "Wad.hpp"

void CWAD::LoadWadFile(const char *filename)
{
	W_LoadWadFile(filename);
};

void CWAD::CleanupName(const char *in, char *out)
{
	W_CleanupName(in, out);
};

lumpinfo_t *CWAD::GetLumpInfo(const char *name)
{
	return W_GetLumpinfo(name);
};

void *CWAD::GetLumpName(const char *name)
{
	return W_GetLumpName(name);
};

void *CWAD::GetLumpNum(int num)
{
	return W_GetLumpNum(num);
};