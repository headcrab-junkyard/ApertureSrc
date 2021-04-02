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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "Cache.hpp"

#include "engine/ISystem.hpp"

/*
===============================================================================

CACHE MEMORY

===============================================================================
*/

typedef struct cache_system_s
{
	int						size;		// including this header
	cache_user_t			*user;
	char					name[16];
	struct cache_system_s	*prev, *next;
	struct cache_system_s	*lru_prev, *lru_next;	// for LRU flushing	
} cache_system_t;

cache_system_t	cache_head;

CCache::CCache(ISystem *apSystem) : mpSystem(apSystem){}

/*
==============
Cache_Alloc
==============
*/
void *CCache::Alloc(cache_user_t *c, int size, const char *name)
{
	cache_system_t	*cs;

	if (c->data)
		mpSystem->Error ("Cache_Alloc: already allocated");
	
	if (size <= 0)
		mpSystem->Error ("Cache_Alloc: size %i", size);

	size = (size + sizeof(cache_system_t) + 15) & ~15;

// find memory for it	
	while (1)
	{
		cs = TryAlloc (size, false);
		if (cs)
		{
			strncpy (cs->name, name, sizeof(cs->name)-1);
			c->data = (void *)(cs+1);
			cs->user = c;
			break;
		};
	
		// free the least recently used cahedat
		if (cache_head.lru_prev == &cache_head)
			mpSystem->Error ("Cache_Alloc: out of memory");
		// not enough memory at all
		Free( cache_head.lru_prev->user );
	};
	
	return Check(c);
};

/*
==============
Cache_Free

Frees the memory and removes it from the LRU list
==============
*/
void CCache::Free(cache_user_t *c)
{
	if(!c->data)
		mpSystem->Error("Cache_Free: not allocated");

	cache_system_t *cs = ((cache_system_t *)c->data) - 1;

	cs->prev->next = cs->next;
	cs->next->prev = cs->prev;
	cs->next = cs->prev = nullptr;

	c->data = nullptr;

	UnlinkLRU(cs);
};

/*
==============
Cache_Check
==============
*/
void *CCache::Check(cache_user_t *c)
{
	if (!c->data)
		return nullptr;

	cache_system_t *cs = ((cache_system_t *)c->data) - 1;

// move to head of LRU
	UnlinkLRU (cs);
	MakeLRU (cs);
	
	return c->data;
};

/*
============
Cache_Report

============
*/
void CCache::Report()
{
	mpSystem->DevPrintf ("%4.1f megabyte data cache\n", (hunk_size - hunk_high_used - hunk_low_used) / (float)(1024*1024) );
};

/*
============
Cache_Flush

Throw everything out, so new data will be demand cached
============
*/
void CCache::Flush()
{
	while (cache_head.next != &cache_head)
		Free( cache_head.next->user );	// reclaim the space
};

/*
============
Cache_TryAlloc

Looks for a free block of memory between the high and low hunk marks
Size should already include the header and padding
============
*/
cache_system_t *CCache::TryAlloc(int size, bool nobottom)
{
	cache_system_t *pnew{nullptr};
	
// is the cache completely empty?

	if (!nobottom && cache_head.prev == &cache_head)
	{
		if (hunk_size - hunk_high_used - mpHunk->GetLowUsed() < size)
			mpSystem->Error ("Cache_TryAlloc: %i is greater then free hunk", size);

		pnew = (cache_system_t *) (hunk_base + mpHunk->GetLowUsed());
		Q_memset (pnew, 0, sizeof(*pnew));
		pnew->size = size;

		cache_head.prev = cache_head.next = pnew;
		pnew->prev = pnew->next = &cache_head;
		
		MakeLRU (pnew);
		return pnew;
	};
	
// search from the bottom up for space

	pnew = (cache_system_t *) (hunk_base + mpHunk->GetLowUsed());
	cache_system_t *cs = cache_head.next;
	
	do
	{
		if (!nobottom || cs != cache_head.next)
		{
			if ( (byte *)cs - (byte *)pnew >= size)
			{	// found space
				Q_memset (pnew, 0, sizeof(*pnew));
				pnew->size = size;
				
				pnew->next = cs;
				pnew->prev = cs->prev;
				cs->prev->next = pnew;
				cs->prev = pnew;
				
				MakeLRU (pnew);
	
				return pnew;
			};
		};

	// continue looking		
		pnew = (cache_system_t *)((byte *)cs + cs->size);
		cs = cs->next;

	}
	while (cs != &cache_head);
	
// try to allocate one at the very end
	if ( hunk_base + hunk_size - hunk_high_used - (byte *)pnew >= size)
	{
		Q_memset (pnew, 0, sizeof(*pnew));
		pnew->size = size;
		
		pnew->next = &cache_head;
		pnew->prev = cache_head.prev;
		cache_head.prev->next = pnew;
		cache_head.prev = pnew;
		
		MakeLRU (pnew);

		return pnew;
	};
	
	return nullptr;		// couldn't allocate
};

void CCache::MakeLRU (cache_system_t *cs)
{
	if (cs->lru_next || cs->lru_prev)
		mpSystem->Error ("Cache_MakeLRU: active link");

	cache_head.lru_next->lru_prev = cs;
	cs->lru_next = cache_head.lru_next;
	cs->lru_prev = &cache_head;
	cache_head.lru_next = cs;
};

void CCache::UnlinkLRU (cache_system_t *cs)
{
	if (!cs->lru_next || !cs->lru_prev)
		mpSystem->Error ("Cache_UnlinkLRU: NULL link");

	cs->lru_next->lru_prev = cs->lru_prev;
	cs->lru_prev->lru_next = cs->lru_next;
	
	cs->lru_prev = cs->lru_next = nullptr;
};