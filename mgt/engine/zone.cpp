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
void Z_ClearZone(memzone_t *zone, int size);

/*
========================
Z_ClearZone
========================
*/
void Z_ClearZone(memzone_t *zone, int size)
{
	memblock_t *block;

	// set the entire zone to one free block

	zone->blocklist.next = zone->blocklist.prev = block =
	(memblock_t *)((byte *)zone + sizeof(memzone_t));
	zone->blocklist.tag = 1; // in use block
	zone->blocklist.id = 0;
	zone->blocklist.size = 0;
	zone->rover = block;

	block->prev = block->next = &zone->blocklist;
	block->tag = 0; // free block
	block->id = ZONEID;
	block->size = size - sizeof(memzone_t);
};

/*
========================
Z_Print
========================
*/
void Z_Print(memzone_t *zone)
{
	memblock_t *block;

	gpSystem->Printf("zone size: %i  location: %p\n", mainzone->size, mainzone);

	for(block = zone->blocklist.next;; block = block->next)
	{
		gpSystem->Printf("block:%p    size:%7i    tag:%3i\n",
		           block, block->size, block->tag);

		if(block->next == &zone->blocklist)
			break; // all blocks have been hit
		if((byte *)block + block->size != (byte *)block->next)
			gpSystem->Printf("ERROR: block size does not touch the next block\n");
		if(block->next->prev != block)
			gpSystem->Printf("ERROR: next block doesn't have proper back link\n");
		if(!block->tag && !block->next->tag)
			gpSystem->Printf("ERROR: two consecutive free blocks\n");
	};
};

//============================================================================

/*
========================
Memory_Init
========================
*/
void Memory_Init(void *buf, int size)
{
	int p;
	int zonesize = DYNAMIC_SIZE;

	hunk_base = (byte*)buf;
	hunk_size = size;
	hunk_low_used = 0;
	hunk_high_used = 0;

	Cache_Init();
	p = COM_CheckParm("-zone");
	if(p)
	{
		if(p < com_argc - 1)
			zonesize = Q_atoi(com_argv[p + 1]) * 1024;
		else
			gpSystem->Error("Memory_Init: you must specify a size in KB after -zone");
	};
	mainzone = (memzone_t*)Hunk_AllocName(zonesize, "zone");
	Z_ClearZone(mainzone, zonesize);
};