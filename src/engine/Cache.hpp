/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2021 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief cache allocator

#pragma once

//#include "CommonTypes.hpp"

#include "engine/IMemory.hpp"

/////////////////////////////////////////////////

interface ISystem;

using cache_system_t = struct cache_system_s;

/*
typedef struct cache_user_s
{
	void *data{nullptr};
} cache_user_t;
*/

void Cache_FreeHigh(int new_high_hunk);
void Cache_FreeLow(int new_low_hunk);

class CCache
{
public:
	CCache(ISystem *apSystem);
	//~CCache();
	
	/// @return nullptr if all purgable data was tossed and there still
	/// wasn't enough room
	void *Alloc(cache_user_t *c, int size, const char *name);
	
	///
	void Free(cache_user_t *c);
	
	/// @return the cached data, and moves to the head of the LRU list
	/// if present, otherwise returns nullptr
	void *Check(cache_user_t *c);
	
	///
	void Report();
	
	///
	void Flush();
private:
	cache_system_t *TryAlloc(int size, bool nobottom);
	
	void MakeLRU(cache_system_t *cs);
	void UnlinkLRU(cache_system_t *cs);
private:
	ISystem *mpSystem{nullptr};
};