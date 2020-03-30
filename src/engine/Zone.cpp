#include "quakedef.h"
#include "Zone.hpp"

void *CZone::Malloc(int size)
{
	return Z_Malloc(size);
};

void *CZone::TagMalloc(int size, int tag)
{
	return Z_TagMalloc(size, tag);
};

void CZone::Free(void *ptr)
{
	Z_Free(ptr);
};

void CZone::CheckHeap()
{
	Z_CheckHeap();
};

void CZone::DumpHeap()
{
	Z_DumpHeap();
};

int CZone::FreeMemory()
{
	return Z_FreeMemory();
};