/// @file

#include "quakedef.h"

#include "Memory.hpp"
#include "Hunk.hpp"
#include "Cache.hpp"

#include "engine/ISystem.hpp"

//============================================================================

CMemory::CMemory(ISystem *apSystem) : mpSystem(apSystem){}

/*
========================
Memory_Init
========================
*/
void CMemory::Init(void *buf, int size)
{
	mpHunk = std::make_unique<CHunk>(mpSystem, buf, size);

	Cache_Init();
	
	int zonesize{DYNAMIC_SIZE};
	
	int p{COM_CheckParm("-zone")};
	if(p)
	{
		if(p < com_argc - 1)
			zonesize = Q_atoi(com_argv[p + 1]) * 1024;
		else
			mpSystem->Error("Memory_Init: you must specify a size in KB after -zone");
	};
	
	mainzone = reinterpret_cast<memzone_t*>(mpHunk->AllocName(zonesize, "zone"));
	Z_ClearZone(mainzone, zonesize);
};