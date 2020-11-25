/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief cache allocator

#pragma once

#include "engine/IMemory.hpp"

/////////////////////////////////////////////////

/*
typedef struct cache_user_s
{
	void *data{nullptr};
} cache_user_t;
*/

///
void Cache_Flush();

/// Returns the cached data, and moves to the head of the LRU list
/// if present, otherwise returns nullptr
void *Cache_Check(cache_user_t *c);

///
void Cache_Free(cache_user_t *c);

/// Returns nullptr if all purgable data was tossed and there still
/// wasn't enough room
void *Cache_Alloc(cache_user_t *c, int size, const char *name);

///
void Cache_Report();

void Cache_FreeHigh(int new_high_hunk);
void Cache_FreeLow(int new_low_hunk);

/*
class Cache
{
public:
	Cache();
	~Cache();
	
	/// Returns nullptr if all purgable data was tossed and there still
	/// wasn't enough room
	void *Alloc(cache_user_t *c, int size, const char *name);

	///
	void Free(cache_user_t *c);

	/// Returns the cached data, and moves to the head of the LRU list
	/// if present, otherwise returns nullptr
	void *Check(cache_user_t *c);

	void Flush();

	///
	void Report();
};
*/