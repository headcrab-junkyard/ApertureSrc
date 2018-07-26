/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
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
/// @brief memory module

#pragma once

/**
 memory allocation


H_??? The hunk manages the entire memory block given to quake.  It must be
contiguous.  Memory can be allocated from either the low or high end in a
stack fashion.  The only way memory is released is by resetting one of the
pointers.

Hunk allocations should be given a name, so the Hunk_Print () function
can display usage.

Hunk allocations are guaranteed to be 16 byte aligned.

The video buffers are allocated high to avoid leaving a hole underneath
server allocations when changing to a higher video mode.


Z_??? Zone memory functions used for small, dynamic allocations like text
strings from command input.  There is only about 48K for it, allocated at
the very bottom of the hunk.

Cache_??? Cache memory is for objects that can be dynamically loaded and
can usefully stay persistant between levels.  The size of the cache
fluctuates from level to level.

To allocate a cachable object


Temp_??? Temp memory is used for file loading and surface caching.  The size
of the cache memory is adjusted so that there is a minimum of 512k remaining
for temp memory.


------ Top of Memory -------

high hunk allocations

<--- high hunk reset point held by vid

video buffer

z buffer

surface cache

<--- high hunk used

cachable memory

<--- low hunk used

client and server low hunk allocations

<-- low hunk reset point held by host

startup hunk allocations

Zone block

----- Bottom of Memory -----



*/

#include "engine/IMemory.hpp"
//#include "core/MemoryTypes.hpp"
#include "Zone.hpp"
#include "Hunk.hpp"
#include "Cache.hpp"

//struct CMemoryPool;

class CMemory final : public IMemory
{
public:
	CMemory();
	~CMemory();

	//void Init();
	//void Shutdown();
	
	void *Z_Malloc(size_t anSize) override;
	void Z_Free(void *apData) override;
	
	void *Hunk_AllocName(int size, const char *name) override;
	void *Hunk_TempAlloc(int size) override;
	
	//void *Alloc(byte *apPool, size_t anSize, const char *asFileName, int anFileLine);
	//void Free(void *apData, const char *asFileName, int anFileLine);
	
	//void *Realloc(byte *apPool, void *memptr, size_t size, const char *asFileName, int anFileLine);
	
	//byte *AllocPool(const char *asPoolName, const char *asFileName, int anFileLine);
	//void FreePool(byte **apPool, const char *asFileName, int anFileLine);
	//void EmptyPool(byte *apPool, const char *asFileName, int anFileLine);

/*	
	void Check(const char *asFileName, int asFileLine);
	
	bool IsAllocated(byte *apPool, void *apData);
	
	void PrintList(size_t minallocationsize);
	void PrintStats();
private:
	void FreeBlock();
	const char *CheckFileName(const char *asName);
	//bool CheckAlloc(CMemoryPool *apPool, void *apData);
	void CheckHeaderSentinels(void *apData, const char *asFileName, int anFileLine);
	void CheckClumpSentinels();
	
	//CMemoryPool *mpPoolChain{nullptr}; // Critical stuff
*/
};

/*
constexpr void Mem_SafeFree(void *pData)
{
	if(pData)
	{
		Mem_Free(pData);
		pData = nullptr;
	};
};
*/