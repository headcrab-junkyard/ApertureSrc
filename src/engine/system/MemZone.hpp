/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2022 BlackPhrase
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

#pragma once

#include "CommonTypes.hpp"

interface ISystem;
class CMemHunk;

class CMemZone
{
public:
	CMemZone(ISystem *apSystem, CMemHunk *apMemHunk, int anSize);
	
	void *Malloc(int size); // returns 0 filled memory
	void *TagMalloc(int size, int tag);

	void Free(void *ptr);

	void CheckHeap();
private:
	void Clear(int anSize);
private:
	ISystem *mpSystem{nullptr};
	
	struct memblock_t
	{
		int size; ///< including the header and possibly tiny fragments
		int tag;  ///< a tag of 0 is a free block
		int id;   ///< should be ZONEID
		memblock_t *next, *prev;
		int pad; ///< pad to 64 bit boundary
	};

	struct memzone_t
	{
		int size;             ///< total bytes malloced, including header
		memblock_t blocklist; ///< start / end cap for linked list
		memblock_t *rover;
	};
	
	memzone_t *mpZone{nullptr};
};