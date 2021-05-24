/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2015-2019, 2021 BlackPhrase
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
#include "ModelLoaderSPR.hpp"

//=============================================================================

CModelLoaderSPR::CModelLoaderSPR(ISystem *apSystem) : mpSystem(apSystem){}

/*
=================
Mod_LoadSpriteFrame
=================
*/
void * CModelLoaderSPR::LoadSpriteFrame (void * pin, mspriteframe_t **ppframe)
{
	auto pinframe = (dspriteframe_t *)pin;

	int width = LittleLong (pinframe->width);
	int height = LittleLong (pinframe->height);
	int size = width * height;

	auto pspriteframe = (mspriteframe_t*)mpMemory->GetHunk()->AllocName (sizeof (mspriteframe_t) + size*r_pixbytes, loadname);

	Q_memset (pspriteframe, 0, sizeof (mspriteframe_t) + size);
	*ppframe = pspriteframe;

	pspriteframe->width = width;
	pspriteframe->height = height;
	
	int origin[2];
	
	origin[0] = LittleLong (pinframe->origin[0]);
	origin[1] = LittleLong (pinframe->origin[1]);

	pspriteframe->up = origin[1];
	pspriteframe->down = origin[1] - height;
	pspriteframe->left = origin[0];
	pspriteframe->right = width + origin[0];

	if (r_pixbytes == 1)
		Q_memcpy (&pspriteframe->pixels[0], (byte *)(pinframe + 1), size);
	else if (r_pixbytes == 2)
	{
		auto ppixin = (byte *)(pinframe + 1);
		auto ppixout = (unsigned short *)&pspriteframe->pixels[0];

		for (int i=0 ; i<size ; i++)
			ppixout[i] = d_8to16table[ppixin[i]];
	}
	else
		mpSystem->Error ("Mod_LoadSpriteFrame: driver set invalid r_pixbytes: %d\n", r_pixbytes);

	return (void *)((byte *)pinframe + sizeof (dspriteframe_t) + size);
};

/*
=================
Mod_LoadSpriteGroup
=================
*/
void * CModelLoaderSPR::LoadSpriteGroup (void * pin, mspriteframe_t **ppframe)
{
	int					i;

	auto pingroup = (dspritegroup_t *)pin;

	int numframes = LittleLong (pingroup->numframes);

	auto pspritegroup = (mspritegroup_t*)mpMemory->GetHunk()->AllocName (sizeof (mspritegroup_t) +
				(numframes - 1) * sizeof (pspritegroup->frames[0]), loadname);

	pspritegroup->numframes = numframes;

	*ppframe = (mspriteframe_t *)pspritegroup;

	auto pin_intervals = (dspriteinterval_t *)(pingroup + 1);

	auto poutintervals = (float*)mpMemory->GetHunk()->AllocName (numframes * sizeof (float), loadname);

	pspritegroup->intervals = poutintervals;

	for (i=0 ; i<numframes ; i++)
	{
		*poutintervals = LittleFloat (pin_intervals->interval);
		if (*poutintervals <= 0.0)
			mpSystem->Error ("Mod_LoadSpriteGroup: interval<=0");

		poutintervals++;
		pin_intervals++;
	};

	auto ptemp = (void *)pin_intervals;

	for (i=0 ; i<numframes ; i++)
		ptemp = LoadSpriteFrame (ptemp, &pspritegroup->frames[i]);

	return ptemp;
};

/*
=================
Mod_LoadSpriteModel
=================
*/
void CModelLoaderSPR::LoadSpriteModel (model_t *mod, void *buffer)
{
	int					i;
	int					version;
	dsprite_t			*pin;
	msprite_t			*psprite;
	int					numframes;
	int					size;
	dspriteframetype_t	*pframetype;
	
	pin = (dsprite_t *)buffer;

	version = LittleLong (pin->version);
	if (version != SPRITE_VERSION)
		mpSystem->Error ("%s has wrong version number "
				 "(%i should be %i)", mod->name, version, SPRITE_VERSION);

	numframes = LittleLong (pin->numframes);

	size = sizeof (msprite_t) +	(numframes - 1) * sizeof (psprite->frames);

	psprite = (msprite_t*)mpMemory->GetHunk()->AllocName (size, loadname);

	mod->cache.data = psprite;

	psprite->type = LittleLong (pin->type);
	psprite->maxwidth = LittleLong (pin->width);
	psprite->maxheight = LittleLong (pin->height);
	psprite->beamlength = LittleFloat (pin->beamlength);
	mod->synctype = (synctype_t)LittleLong (pin->synctype);
	psprite->numframes = numframes;

	mod->mins[0] = mod->mins[1] = -psprite->maxwidth/2;
	mod->maxs[0] = mod->maxs[1] = psprite->maxwidth/2;
	mod->mins[2] = -psprite->maxheight/2;
	mod->maxs[2] = psprite->maxheight/2;
	
//
// load the frames
//
	if (numframes < 1)
		mpSystem->Error ("Mod_LoadSpriteModel: Invalid # of frames: %d\n", numframes);

	mod->numframes = numframes;
	mod->flags = 0;

	pframetype = (dspriteframetype_t *)(pin + 1);

	for (i=0 ; i<numframes ; i++)
	{
		spriteframetype_t	frametype;

		frametype = (spriteframetype_t)LittleLong (pframetype->type);
		psprite->frames[i].type = frametype;

		if (frametype == SPR_SINGLE)
		{
			pframetype = (dspriteframetype_t *)
					LoadSpriteFrame (pframetype + 1,
										 &psprite->frames[i].frameptr);
		}
		else
		{
			pframetype = (dspriteframetype_t *)
					LoadSpriteGroup (pframetype + 1,
										 &psprite->frames[i].frameptr);
		};
	};

	mod->type = mod_sprite;
};

bool CModelLoaderSPR::IsExtSupported(const char *asExt) const
{
	if(!Q_strcmp(asExt, "spr"))
		return true;
	return false;
};

IModel *CModelLoaderSPR::TryLoad(const char *asName)
{
	LoadSpriteModel(TODO);
	return nullptr;
};