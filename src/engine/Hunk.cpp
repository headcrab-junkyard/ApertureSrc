/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2021 BlackPhrase
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

#include "quakedef.h"
#include "Hunk.hpp"
//#include "common.h"
//#include "Memory.hpp"

//============================================================================

#define HUNK_SENTINEL 0x1df001ed

struct hunk_t
{
	int sentinal;
	int size; // including sizeof(hunk_t), -1 = not allocated
	char name[8];
};

byte *hunk_base;
int hunk_size;

int hunk_low_used;
int hunk_high_used;

bool hunk_tempactive;
int hunk_tempmark;

void Cache_FreeLow(int new_low_hunk);
void Cache_FreeHigh(int new_high_hunk);

/*
===================
Hunk_Alloc
===================
*/
void *CHunk::Alloc(int size)
{
	return AllocName(size, "unknown");
};

/*
===================
Hunk_AllocName
===================
*/
void *CHunk::AllocName(int size, const char *name)
{
#ifdef PARANOID
	Check();
#endif

	if(size < 0)
		mpSystem->Error("Hunk_Alloc: bad size: %i", size);

	size = sizeof(hunk_t) + ((size + 15) & ~15);

	if(hunk_size - hunk_low_used - hunk_high_used < size)
		mpSystem->Error("Hunk_Alloc: failed on %i bytes", size);

	auto h{(hunk_t *)(hunk_base + hunk_low_used)};
	hunk_low_used += size;

	Cache_FreeLow(hunk_low_used);

	Q_memset(h, 0, size);

	h->size = size;
	h->sentinal = HUNK_SENTINEL;
	Q_strncpy(h->name, name, 8);

	return (void *)(h + 1);
};

/*
===================
Hunk_HighAllocName
===================
*/
void *CHunk::HighAllocName(int size, const char *name)
{
	hunk_t *h;

	if(size < 0)
		mpSystem->Error("Hunk_HighAllocName: bad size: %i", size);

	if(hunk_tempactive)
	{
		FreeToHighMark(hunk_tempmark);
		hunk_tempactive = false;
	};

#ifdef PARANOID
	Check();
#endif

	size = sizeof(hunk_t) + ((size + 15) & ~15);

	if(hunk_size - hunk_low_used - hunk_high_used < size)
	{
		mpSystem->Printf("Hunk_HighAlloc: failed on %i bytes\n", size);
		return nullptr;
	};

	hunk_high_used += size;
	Cache_FreeHigh(hunk_high_used);

	h = (hunk_t *)(hunk_base + hunk_size - hunk_high_used);

	Q_memset(h, 0, size);
	h->size = size;
	h->sentinal = HUNK_SENTINEL;
	Q_strncpy(h->name, name, 8);

	return (void *)(h + 1);
};

/*
=================
Hunk_TempAlloc

Return space from the top of the hunk
=================
*/
void *CHunk::TempAlloc(int size)
{
	size = (size + 15) & ~15;

	if(hunk_tempactive)
	{
		FreeToHighMark(hunk_tempmark);
		hunk_tempactive = false;
	};

	hunk_tempmark = GetHighMark();

	void *buf = HighAllocName(size, "temp");

	hunk_tempactive = true;

	return buf;
};

void CHunk::FreeToHighMark(int mark)
{
	if(hunk_tempactive)
	{
		hunk_tempactive = false;
		FreeToHighMark(hunk_tempmark);
	};
	if(mark < 0 || mark > hunk_high_used)
		mpSystem->Error("Hunk_FreeToHighMark: bad mark %i", mark);
	Q_memset(hunk_base + hunk_size - hunk_high_used, 0, hunk_high_used - mark);
	hunk_high_used = mark;
};

void CHunk::FreeToLowMark(int mark)
{
	if(mark < 0 || mark > hunk_low_used)
		mpSystem->Error("Hunk_FreeToLowMark: bad mark %i", mark);
	Q_memset(hunk_base + mark, 0, hunk_low_used - mark);
	hunk_low_used = mark;
};

/*
==============
Hunk_Check

Run consistancy and sentinel trashing checks
==============
*/
void CHunk::Check()
{
	hunk_t *h;

	for(h = (hunk_t *)hunk_base; (byte *)h != hunk_base + hunk_low_used;)
	{
		if(h->sentinal != HUNK_SENTINEL)
			mpSystem->Error("Hunk_Check: trashed sentinel");
		if(h->size < 16 || h->size + (byte *)h - hunk_base > hunk_size)
			mpSystem->Error("Hunk_Check: bad size");
		h = (hunk_t *)((byte *)h + h->size);
	};
};

int	CHunk::GetHighMark()
{
	if(hunk_tempactive)
	{
		hunk_tempactive = false;
		FreeToHighMark(hunk_tempmark);
	};

	return hunk_high_used;
};

int	CHunk::GetLowMark() const
{
	return hunk_low_used;
};