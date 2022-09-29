/// @file

#include "quakedef.h"

#include "Memory.hpp"
#include "MemHunk.hpp"
#include "MemCache.hpp"
#include "MemZone.hpp"

#include "engine/ISystem.hpp"
#include "engine/ICmdArgs.hpp"

//============================================================================

constexpr auto DYNAMIC_SIZE{0xc000};

CMemory::CMemory(ISystem *apSystem, int anHeapSize) : mpSystem(apSystem)
{
	mpMemBase = malloc(anHeapSize);
	
	Init(anHeapSize);
};

CMemory::~CMemory()
{
	free(mpMemBase);
};

void *CMemory::Calloc(int anNum, size_t anSize)
{
	return calloc(anNum, anSize);
};

void *CMemory::CheckCache(cache_user_t *apUser)
{
	return mpCache->Check(apUser);
};

/*
========================
Memory_Init
========================
*/
void CMemory::Init(int anHeapSize)
{	
	mpHunk = std::make_unique<CMemHunk>(mpSystem, mpMemBase, anHeapSize);

	mpCache = std::make_unique<CMemCache>(mpSystem);
	
	//mpCache->Init(); // TODO
	
	int nZoneSize{DYNAMIC_SIZE};
	
	auto pStartupArgs{mpSystem->GetStartupArgs()};
	int p{pStartupArgs->CheckParm("-zone")};
	if(p)
	{
		if(p < pStartupArgs->GetCount() - 1)
			nZoneSize = Q_atoi(pStartupArgs->GetByIndex(p + 1)) * 1024;
		else
			mpSystem->Error("Memory_Init: you must specify a size in KB after -zone");
	};
	
	mpMainZone = std::make_unique<CMemZone>(mpSystem, mpHunk.get(), nZoneSize);
};