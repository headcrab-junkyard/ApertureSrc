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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
//#include <cstring>
//#include "common.hpp"
//#include "Memory.hpp"
//#include "CmdRegistry.hpp"

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

cache_system_t *Cache_TryAlloc (int size, bool nobottom);

cache_system_t	cache_head;

/*
===========
Cache_Move
===========
*/
void Cache_Move ( cache_system_t *c)
{
	cache_system_t		*pnew;

// we are clearing up space at the bottom, so only allocate it late
	pnew = Cache_TryAlloc (c->size, true);
	if (pnew)
	{
//		Con_Printf ("cache_move ok\n");

		Q_memcpy ( pnew+1, c+1, c->size - sizeof(cache_system_t) );
		pnew->user = c->user;
		Q_memcpy (pnew->name, c->name, sizeof(pnew->name));
		Cache_Free (c->user);
		pnew->user->data = (void *)(pnew+1);
	}
	else
	{
//		Con_Printf ("cache_move failed\n");

		Cache_Free (c->user);		// tough luck...
	};
};

/*
============
Cache_FreeLow

Throw things out until the hunk can be expanded to the given point
============
*/
void Cache_FreeLow (int new_low_hunk)
{
	cache_system_t	*c;
	
	while (1)
	{
		c = cache_head.next;
		if (c == &cache_head)
			return;		// nothing in cache at all
		if ((byte *)c >= hunk_base + new_low_hunk)
			return;		// there is space to grow the hunk
		Cache_Move ( c );	// reclaim the space
	};
};

/*
============
Cache_FreeHigh

Throw things out until the hunk can be expanded to the given point
============
*/
void Cache_FreeHigh (int new_high_hunk)
{
	cache_system_t	*c, *prev;
	
	prev = nullptr;
	while (1)
	{
		c = cache_head.prev;
		if (c == &cache_head)
			return;		// nothing in cache at all
		if ( (byte *)c + c->size <= hunk_base + hunk_size - new_high_hunk)
			return;		// there is space to grow the hunk
		if (c == prev)
			Cache_Free (c->user);	// didn't move out of the way
		else
		{
			Cache_Move (c);	// try to move it
			prev = c;
		};
	};
};

void Cache_UnlinkLRU (cache_system_t *cs)
{
	if (!cs->lru_next || !cs->lru_prev)
		gpSystem->Error ("Cache_UnlinkLRU: NULL link");

	cs->lru_next->lru_prev = cs->lru_prev;
	cs->lru_prev->lru_next = cs->lru_next;
	
	cs->lru_prev = cs->lru_next = nullptr;
};

void Cache_MakeLRU (cache_system_t *cs)
{
	if (cs->lru_next || cs->lru_prev)
		gpSystem->Error ("Cache_MakeLRU: active link");

	cache_head.lru_next->lru_prev = cs;
	cs->lru_next = cache_head.lru_next;
	cs->lru_prev = &cache_head;
	cache_head.lru_next = cs;
};

/*
============
Cache_TryAlloc

Looks for a free block of memory between the high and low hunk marks
Size should already include the header and padding
============
*/
cache_system_t *Cache_TryAlloc (int size, bool nobottom)
{
	cache_system_t	*cs, *pnew;
	
// is the cache completely empty?

	if (!nobottom && cache_head.prev == &cache_head)
	{
		if (hunk_size - hunk_high_used - hunk_low_used < size)
			gpSystem->Error ("Cache_TryAlloc: %i is greater then free hunk", size);

		pnew = (cache_system_t *) (hunk_base + hunk_low_used);
		Q_memset (pnew, 0, sizeof(*pnew));
		pnew->size = size;

		cache_head.prev = cache_head.next = pnew;
		pnew->prev = pnew->next = &cache_head;
		
		Cache_MakeLRU (pnew);
		return pnew;
	};
	
// search from the bottom up for space

	pnew = (cache_system_t *) (hunk_base + hunk_low_used);
	cs = cache_head.next;
	
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
				
				Cache_MakeLRU (pnew);
	
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
		
		Cache_MakeLRU (pnew);

		return pnew;
	};
	
	return nullptr;		// couldn't allocate
};

/*
============
Cache_Flush

Throw everything out, so new data will be demand cached
============
*/
void Cache_Flush ()
{
	while (cache_head.next != &cache_head)
		Cache_Free ( cache_head.next->user );	// reclaim the space
};

/*
============
Cache_Print

============
*/
void Cache_Print ()
{
	cache_system_t	*cd;

	for (cd = cache_head.next ; cd != &cache_head ; cd = cd->next)
		gpSystem->Printf ("%8i : %s\n", cd->size, cd->name);
};

/*
============
Cache_Report

============
*/
void Cache_Report ()
{
	gpSystem->DevPrintf ("%4.1f megabyte data cache\n", (hunk_size - hunk_high_used - hunk_low_used) / (float)(1024*1024) );
};

/*
============
Cache_Compact

============
*/
void Cache_Compact ()
{
};

/*
============
Cache_Init

============
*/
void Cache_Init ()
{
	cache_head.next = cache_head.prev = &cache_head;
	cache_head.lru_next = cache_head.lru_prev = &cache_head;

	gpCmdRegistry->Add("flush", Cache_Flush);
};

/*
==============
Cache_Free

Frees the memory and removes it from the LRU list
==============
*/
void Cache_Free (cache_user_t *c)
{
	cache_system_t	*cs;

	if (!c->data)
		gpSystem->Error ("Cache_Free: not allocated");

	cs = ((cache_system_t *)c->data) - 1;

	cs->prev->next = cs->next;
	cs->next->prev = cs->prev;
	cs->next = cs->prev = nullptr;

	c->data = nullptr;

	Cache_UnlinkLRU (cs);
};

/*
==============
Cache_Check
==============
*/
void *Cache_Check (cache_user_t *c)
{
	cache_system_t	*cs;

	if (!c->data)
		return nullptr;

	cs = ((cache_system_t *)c->data) - 1;

// move to head of LRU
	Cache_UnlinkLRU (cs);
	Cache_MakeLRU (cs);
	
	return c->data;
};

/*
==============
Cache_Alloc
==============
*/
void *Cache_Alloc (cache_user_t *c, int size, const char *name)
{
	cache_system_t	*cs;

	if (c->data)
		gpSystem->Error ("Cache_Alloc: allready allocated");
	
	if (size <= 0)
		gpSystem->Error ("Cache_Alloc: size %i", size);

	size = (size + sizeof(cache_system_t) + 15) & ~15;

// find memory for it	
	while (1)
	{
		cs = Cache_TryAlloc (size, false);
		if (cs)
		{
			strncpy (cs->name, name, sizeof(cs->name)-1);
			c->data = (void *)(cs+1);
			cs->user = c;
			break;
		};
	
	// free the least recently used cahedat
		if (cache_head.lru_prev == &cache_head)
			gpSystem->Error ("Cache_Alloc: out of memory");
													// not enough memory at all
		Cache_Free ( cache_head.lru_prev->user );
	};
	
	return Cache_Check (c);
};