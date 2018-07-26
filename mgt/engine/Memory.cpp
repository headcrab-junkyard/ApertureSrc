/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2017-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "Memory.hpp"

//============================================================================

/*
========================
Memory_Init
========================
*/
void Memory_Init2(void *buf, int size) // TODO
{
	// TODO
/*
	int zonesize = DYNAMIC_SIZE;

	hunk_base = buf;
	hunk_size = size;
	hunk_low_used = 0;
	hunk_high_used = 0;
	
	Cache_Init();
	
	int p = COM_CheckParm("-zone");
	
	if(p)
	{
		if(p < com_argc - 1)
			zonesize = Q_atoi(com_argv[p + 1]) * 1024;
		else
			Sys_Error("Memory_Init: you must specify a size in KB after -zone");
	};
	
	mainzone = Hunk_AllocName(zonesize, "zone");
	Z_ClearZone(mainzone, zonesize);
*/
};

EXPOSE_SINGLE_INTERFACE(CMemory, IMemory, MGT_MEMORY_INTERFACE_VERSION);

CMemory::CMemory() = default;
CMemory::~CMemory() = default;

void *CMemory::Z_Malloc(size_t anSize)
{
	return Z_Malloc(anSize);
};

void CMemory::Z_Free(void *apData)
{
	Z_Free(apData);
};

void *CMemory::Hunk_AllocName(int size, const char *name)
{
	return Hunk_AllocName(size, name);
};

void *CMemory::Hunk_TempAlloc(int size)
{
	return Hunk_TempAlloc(size);
};