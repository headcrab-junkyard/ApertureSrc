/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2017-2021 BlackPhrase
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
/// @brief memory manager interface

#pragma once

#include <cstring>

#include "CommonTypes.hpp"
#include "tier1/interface.h"

constexpr auto OGS_MEMORY_INTERFACE_VERSION{"OGSMemory002"};

typedef struct cache_user_s
{
	void *data;
} cache_user_t;

/*
interface IZone
{
};
*/

interface IMemory : public IBaseInterface
{
	/// Allocate anNum*anSize bytes and zero it
	/// @return pointer to allocated memory
	virtual void *Calloc(int anNum, size_t anSize) = 0;
	
	///
	//virtual void *Z_Malloc(size_t anSize) = 0;
	
	///
	//virtual void Z_Free(void *apData) = 0;
	
	///
	//virtual void *Hunk_AllocName(int size, const char *name) = 0;
	
	///
	//virtual void *Hunk_TempAlloc(int size) = 0;
	
	///
	//virtual void Hunk_Check() = 0;
	
	///
	//virtual void *Cache_Alloc(cache_user_t *c, int size, const char *name) = 0;
	
	///
	virtual void *CheckCache(cache_user_t *c) = 0;
	
	///
	//virtual void *Cache_Report() = 0;
};