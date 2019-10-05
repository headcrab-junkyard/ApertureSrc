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
/// @brief model loading and caching

// models are the only shared resource between a client and server running on the same machine

#include "quakedef.h"
//#include "r_local.h"

/// normalizing factor so player model works out to about 1 pixel per triangle
#define ALIAS_BASE_SIZE_RATIO (1.0 / 11.0)

constexpr auto MAX_LBM_HEIGHT{480};

// TODO: mostly unused, legacy from software render
int r_pixbytes{1};

// TODO: mostly unused, legacy from software render
texture_t gr_notexture_mip; // TODO: hack to skip R_InitTextures
texture_t *r_notexture_mip = &gr_notexture_mip;

// TODO: mostly unused, legacy from software render
unsigned short d_8to16table[256];

model_t *loadmodel{nullptr};
char loadname[32]{}; // for hunk tags

byte mod_novis[MAX_MAP_LEAFS/8]{}; // TODO: client-side only?

const int MAX_KNOWN_MODELS{512};
model_t	mod_known[MAX_KNOWN_MODELS]{};
int mod_numknown{0};

/// values for model_t's needload
enum
{
	NL_PRESENT = 0,
	NL_NEEDS_LOADED,
	NL_UNREFERENCED
};

void Mod_LoadSpriteModel(model_t *mod, void *buffer);
void Mod_LoadBrushModel(model_t *mod, void *buffer);
void Mod_LoadAliasModel(model_t *mod, void *buffer);

/*
===============
Mod_Init
===============
*/
void Mod_Init()
{
	memset(mod_novis, 0xff, sizeof(mod_novis)); // TODO: client-side only?
};

/*
===============
Mod_PointInLeaf
===============
*/
// TODO: also used on client-side
mleaf_t *Mod_PointInLeaf (vec3_t p, model_t *model)
{
	mnode_t		*node;
	float		d;
	mplane_t	*plane;
	
	if (!model || !model->nodes)
		gpSystem->Error ("Mod_PointInLeaf: bad model");

	node = model->nodes;
	while (1)
	{
		if (node->contents < 0)
			return (mleaf_t *)node;
		plane = node->plane;
		d = DotProduct (p,plane->normal) - plane->dist;
		if (d > 0)
			node = node->children[0];
		else
			node = node->children[1];
	};
	
	return nullptr;	// never reached
};

/*
===================
Mod_DecompressVis
===================
*/
byte *Mod_DecompressVis(byte *in, model_t *model)
{
	static byte	decompressed[MAX_MAP_LEAFS/8];
	int		c;
	byte	*out;
	int		row;

	row = (model->numleafs+7)>>3;	
	out = decompressed;

#if 0
	Q_memcpy (out, in, row);
#else
	if (!in)
	{
		// no vis info, so make all visible
		while (row)
		{
			*out++ = 0xff;
			row--;
		};
		
		return decompressed;		
	};

	do
	{
		if (*in)
		{
			*out++ = *in++;
			continue;
		};
	
		c = in[1];
		in += 2;
		while (c)
		{
			*out++ = 0;
			c--;
		};
	}
	while (out - decompressed < row);
#endif
	
	return decompressed;
};

byte *Mod_LeafPVS(mleaf_t *leaf, model_t *model)
{
	if(leaf == model->leafs)
		return mod_novis;
	return Mod_DecompressVis(leaf->compressed_vis, model);
};

/*
===================
Mod_ClearAll
===================
*/
void Mod_ClearAll()
{
	int i;
	model_t	*mod;

	for(i = 0, mod=mod_known; i < mod_numknown; i++, mod++)
	{
		mod->needload = NL_UNREFERENCED;
		
		// FIX FOR CACHE_ALLOC ERRORS:
		if(mod->type == mod_sprite)
			mod->cache.data = nullptr;
	};
};

/*
==================
Mod_FindName

==================
*/
model_t *Mod_FindName(const char *name)
{
	int		i;
	model_t	*mod;
	model_t	*avail = nullptr;

	if (!name[0])
		gpSystem->Error ("Mod_ForName: NULL name");
		
	//
	// search the currently loaded models
	//
	for (i=0 , mod=mod_known ; i<mod_numknown ; i++, mod++)
	{
		if (!Q_strcmp (mod->name, name) )
			break;
		if (mod->needload == NL_UNREFERENCED)
			if (!avail || mod->type != mod_alias)
				avail = mod;
	};
			
	if (i == mod_numknown)
	{
		if (mod_numknown == MAX_KNOWN_MODELS)
		{
			if (avail)
			{
				mod = avail;
				if (mod->type == mod_alias)
					if (Cache_Check (&mod->cache))
						Cache_Free (&mod->cache);
			}
			else
				gpSystem->Error ("mod_numknown == MAX_KNOWN_MODELS");
		}
		else
			mod_numknown++;
		
		Q_strcpy (mod->name, name);
		mod->needload = NL_NEEDS_LOADED;
	};

	return mod;
};

/*
==================
Mod_LoadModel

Loads a model into the cache
==================
*/
model_t *Mod_LoadModel (model_t *mod, qboolean crash)
{
	unsigned int *buf;
	byte stackbuf[1024]{}; // avoid dirtying the cache heap

	if (mod->type == mod_alias)
	{
		if (Cache_Check (&mod->cache))
		{
			mod->needload = NL_PRESENT;
			return mod;
		};
	}
	else
		if (mod->needload == NL_PRESENT)
			return mod;

//
// because the world is so huge, load it one piece at a time
//
	
//
// load the file
//
	buf = (unsigned *)COM_LoadStackFile (mod->name, stackbuf, sizeof(stackbuf));
	if (!buf)
	{
		if (crash)
			gpSystem->Error ("Mod_NumForName: %s not found", mod->name);
		return nullptr;
	};
	
//
// allocate a new model
//
	COM_FileBase (mod->name, loadname);
	
	loadmodel = mod;

//
// fill it in
//

// call the apropriate loader
	mod->needload = NL_PRESENT;

	switch (LittleLong(*(unsigned *)buf))
	{
	case IDPOLYHEADER:
		Mod_LoadAliasModel (mod, buf);
		break;
		
	case IDSPRITEHEADER:
		Mod_LoadSpriteModel (mod, buf);
		break;
	
	default:
		Mod_LoadBrushModel (mod, buf);
		break;
	};

	return mod;
};

/*
==================
Mod_ForName

Loads in a model for the given name
==================
*/
model_t *Mod_ForName(const char *name, qboolean crash)
{
	model_t	*mod{Mod_FindName(name)};

	return Mod_LoadModel(mod, crash);
};

//=============================================================================

/*
================
Mod_Print
================
*/
void Mod_Print ()
{
	model_t	*mod{mod_known};

	gpSystem->Printf ("Cached models:\n");
	for (int i=0; i < mod_numknown ; i++, mod++)
	{
		gpSystem->Printf ("%8p : %s",mod->cache.data, mod->name);
		if (mod->needload & NL_UNREFERENCED)
			gpSystem->Printf (" (!R)");
		if (mod->needload & NL_NEEDS_LOADED)
			gpSystem->Printf (" (!P)");
		gpSystem->Printf ("\n");
	};
};

void Mod_Print_f(const ICmdArgs &apArgs)
{
	Mod_Print();
};