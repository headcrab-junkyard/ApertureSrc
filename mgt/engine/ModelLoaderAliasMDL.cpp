/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2015-2019 BlackPhrase
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

#include "quakedef.h"
#include "ModelLoaderAliasMDL.hpp"

/*
==============================================================================

ALIAS MODELS

==============================================================================
*/

/*
=================
Mod_LoadAliasFrame
=================
*/
void * Mod_LoadAliasFrame (void * pin, int *pframeindex, int numv, trivertx_t *pbboxmin, trivertx_t *pbboxmax, aliashdr_t *pheader, char *name)
{
	trivertx_t		*pframe, *pinframe;
	int				i, j;
	daliasframe_t	*pdaliasframe;

	pdaliasframe = (daliasframe_t *)pin;

	Q_strcpy (name, pdaliasframe->name);

	for (i=0 ; i<3 ; i++)
	{
	// these are byte values, so we don't have to worry about
	// endianness
		pbboxmin->v[i] = pdaliasframe->bboxmin.v[i];
		pbboxmax->v[i] = pdaliasframe->bboxmax.v[i];
	};

	pinframe = (trivertx_t *)(pdaliasframe + 1);
	pframe = (trivertx_t*)Hunk_AllocName (numv * sizeof(*pframe), loadname);

	*pframeindex = (byte *)pframe - (byte *)pheader;

	for (j=0 ; j<numv ; j++)
	{
		int		k;

	// these are all byte values, so no need to deal with endianness
		pframe[j].lightnormalindex = pinframe[j].lightnormalindex;

		for (k=0 ; k<3 ; k++)
			pframe[j].v[k] = pinframe[j].v[k];
	};

	pinframe += numv;

	return (void *)pinframe;
};

/*
=================
Mod_LoadAliasGroup
=================
*/
void * CModelLoaderAliasMDL::Mod_LoadAliasGroup (void * pin, int *pframeindex, int numv, trivertx_t *pbboxmin, trivertx_t *pbboxmax, aliashdr_t *pheader, const char *name)
{
	daliasgroup_t		*pingroup;
	maliasgroup_t		*paliasgroup;
	int					i, numframes;
	daliasinterval_t	*pin_intervals;
	float				*poutintervals;
	void				*ptemp;
	
	pingroup = (daliasgroup_t *)pin;

	numframes = LittleLong (pingroup->numframes);

	paliasgroup = (maliasgroup_t*)Hunk_AllocName (sizeof (maliasgroup_t) +
			(numframes - 1) * sizeof (paliasgroup->frames[0]), loadname);

	paliasgroup->numframes = numframes;

	for (i=0 ; i<3 ; i++)
	{
	// these are byte values, so we don't have to worry about endianness
		pbboxmin->v[i] = pingroup->bboxmin.v[i];
		pbboxmax->v[i] = pingroup->bboxmax.v[i];
	};

	*pframeindex = (byte *)paliasgroup - (byte *)pheader;

	pin_intervals = (daliasinterval_t *)(pingroup + 1);

	poutintervals = (float*)Hunk_AllocName (numframes * sizeof (float), loadname);

	paliasgroup->intervals = (byte *)poutintervals - (byte *)pheader;

	for (i=0 ; i<numframes ; i++)
	{
		*poutintervals = LittleFloat (pin_intervals->interval);
		if (*poutintervals <= 0.0)
			mpSystem->Error ("Mod_LoadAliasGroup: interval<=0");

		poutintervals++;
		pin_intervals++;
	};

	ptemp = (void *)pin_intervals;

	for (i=0 ; i<numframes ; i++)
	{
		ptemp = Mod_LoadAliasFrame (ptemp,
									&paliasgroup->frames[i].frame,
									numv,
									&paliasgroup->frames[i].bboxmin,
									&paliasgroup->frames[i].bboxmax,
									pheader, name);
	};

	return ptemp;
};

/*
=================
Mod_LoadAliasSkin
=================
*/
void * CModelLoaderAliasMDL::Mod_LoadAliasSkin (void * pin, int *pskinindex, int skinsize, aliashdr_t *pheader)
{
	int		i;
	byte	*pskin, *pinskin;
	unsigned short	*pusskin;

	pskin = (byte*)Hunk_AllocName (skinsize * r_pixbytes, loadname);
	pinskin = (byte *)pin;
	*pskinindex = (byte *)pskin - (byte *)pheader;

	if (r_pixbytes == 1)
		Q_memcpy (pskin, pinskin, skinsize);
	else if (r_pixbytes == 2)
	{
		pusskin = (unsigned short *)pskin;

		for (i=0 ; i<skinsize ; i++)
			pusskin[i] = d_8to16table[pinskin[i]];
	}
	else
		mpSystem->Error ("Mod_LoadAliasSkin: driver set invalid r_pixbytes: %d\n", r_pixbytes);

	pinskin += skinsize;

	return ((void *)pinskin);
};

/*
=================
Mod_LoadAliasSkinGroup
=================
*/
void * CModelLoaderAliasMDL::Mod_LoadAliasSkinGroup (void * pin, int *pskinindex, int skinsize, aliashdr_t *pheader)
{
	daliasskingroup_t		*pinskingroup;
	maliasskingroup_t		*paliasskingroup;
	int						i, numskins;
	daliasskininterval_t	*pinskinintervals;
	float					*poutskinintervals;
	void					*ptemp;

	pinskingroup = (daliasskingroup_t *)pin;

	numskins = LittleLong (pinskingroup->numskins);

	paliasskingroup = (maliasskingroup_t*)Hunk_AllocName (sizeof (maliasskingroup_t) +
			(numskins - 1) * sizeof (paliasskingroup->skindescs[0]),
			loadname);

	paliasskingroup->numskins = numskins;

	*pskinindex = (byte *)paliasskingroup - (byte *)pheader;

	pinskinintervals = (daliasskininterval_t *)(pinskingroup + 1);

	poutskinintervals = (float*)Hunk_AllocName (numskins * sizeof (float),loadname);

	paliasskingroup->intervals = (byte *)poutskinintervals - (byte *)pheader;

	for (i=0 ; i<numskins ; i++)
	{
		*poutskinintervals = LittleFloat (pinskinintervals->interval);
		if (*poutskinintervals <= 0)
			mpSystem->Error ("Mod_LoadAliasSkinGroup: interval<=0");

		poutskinintervals++;
		pinskinintervals++;
	};

	ptemp = (void *)pinskinintervals;

	for (i=0 ; i<numskins ; i++)
	{
		ptemp = Mod_LoadAliasSkin (ptemp,
				&paliasskingroup->skindescs[i].skin, skinsize, pheader);
	};

	return ptemp;
};

/*
=================
Mod_LoadAliasModel
=================
*/
void CModelLoaderAliasMDL::Mod_LoadAliasModel (model_t *mod, void *buffer)
{
	int					i;
	mdl_t				*pmodel, *pinmodel;
	stvert_t			*pstverts, *pinstverts;
	aliashdr_t			*pheader;
	mtriangle_t			*ptri;
	dtriangle_t			*pintriangles;
	int					version, numframes, numskins;
	int					size;
	daliasframetype_t	*pframetype;
	daliasskintype_t	*pskintype;
	maliasskindesc_t	*pskindesc;
	int					skinsize;
	int					start, end, total;
	
	start = Hunk_LowMark ();

	pinmodel = (mdl_t *)buffer;

	version = LittleLong (pinmodel->version);
	if (version != ALIAS_VERSION)
		mpSystem->Error ("%s has wrong version number (%i should be %i)",
				 mod->name, version, ALIAS_VERSION);

//
// allocate space for a working header, plus all the data except the frames,
// skin and group info
//
	size = 	sizeof (aliashdr_t) + (LittleLong (pinmodel->numframes) - 1) *
			 sizeof (pheader->frames[0]) +
			sizeof (mdl_t) +
			LittleLong (pinmodel->numverts) * sizeof (stvert_t) +
			LittleLong (pinmodel->numtris) * sizeof (mtriangle_t);

	pheader = (aliashdr_t*)Hunk_AllocName (size, loadname);
	pmodel = (mdl_t *) ((byte *)&pheader[1] +
			(LittleLong (pinmodel->numframes) - 1) *
			 sizeof (pheader->frames[0]));
	
//	mod->cache.data = pheader;
	mod->flags = LittleLong (pinmodel->flags);

//
// endian-adjust and copy the data, starting with the alias model header
//
	pmodel->boundingradius = LittleFloat (pinmodel->boundingradius);
	pmodel->numskins = LittleLong (pinmodel->numskins);
	pmodel->skinwidth = LittleLong (pinmodel->skinwidth);
	pmodel->skinheight = LittleLong (pinmodel->skinheight);

	if (pmodel->skinheight > MAX_LBM_HEIGHT)
		mpSystem->Error ("model %s has a skin taller than %d", mod->name,
				   MAX_LBM_HEIGHT);

	pmodel->numverts = LittleLong (pinmodel->numverts);

	if (pmodel->numverts <= 0)
		mpSystem->Error ("model %s has no vertices", mod->name);

	if (pmodel->numverts > MAXALIASVERTS)
		mpSystem->Error ("model %s has too many vertices", mod->name);

	pmodel->numtris = LittleLong (pinmodel->numtris);

	if (pmodel->numtris <= 0)
		mpSystem->Error ("model %s has no triangles", mod->name);

	pmodel->numframes = LittleLong (pinmodel->numframes);
	pmodel->size = LittleFloat (pinmodel->size) * ALIAS_BASE_SIZE_RATIO;
	mod->synctype = (synctype_t)LittleLong (pinmodel->synctype);
	mod->numframes = pmodel->numframes;

	for (i=0 ; i<3 ; i++)
	{
		pmodel->scale[i] = LittleFloat (pinmodel->scale[i]);
		pmodel->scale_origin[i] = LittleFloat (pinmodel->scale_origin[i]);
		pmodel->eyeposition[i] = LittleFloat (pinmodel->eyeposition[i]);
	};

	numskins = pmodel->numskins;
	numframes = pmodel->numframes;

	if (pmodel->skinwidth & 0x03)
		mpSystem->Error ("Mod_LoadAliasModel: skinwidth not multiple of 4");

	pheader->model = (byte *)pmodel - (byte *)pheader;

//
// load the skins
//
	skinsize = pmodel->skinheight * pmodel->skinwidth;

	if (numskins < 1)
		mpSystem->Error ("Mod_LoadAliasModel: Invalid # of skins: %d\n", numskins);

	pskintype = (daliasskintype_t *)&pinmodel[1];

	pskindesc = (maliasskindesc_t*)Hunk_AllocName (numskins * sizeof (maliasskindesc_t),
								loadname);

	pheader->skindesc = (byte *)pskindesc - (byte *)pheader;

	for (i=0 ; i<numskins ; i++)
	{
		aliasskintype_t	skintype;

		skintype = (aliasskintype_t)LittleLong (pskintype->type);
		pskindesc[i].type = skintype;

		if (skintype == ALIAS_SKIN_SINGLE)
		{
			pskintype = (daliasskintype_t *)
					Mod_LoadAliasSkin (pskintype + 1,
									   &pskindesc[i].skin,
									   skinsize, pheader);
		}
		else
		{
			pskintype = (daliasskintype_t *)
					Mod_LoadAliasSkinGroup (pskintype + 1,
											&pskindesc[i].skin,
											skinsize, pheader);
		};
	};

//
// set base s and t vertices
//
	pstverts = (stvert_t *)&pmodel[1];
	pinstverts = (stvert_t *)pskintype;

	pheader->stverts = (byte *)pstverts - (byte *)pheader;

	for (i=0 ; i<pmodel->numverts ; i++)
	{
		pstverts[i].onseam = LittleLong (pinstverts[i].onseam);
	// put s and t in 16.16 format
		pstverts[i].s = LittleLong (pinstverts[i].s) << 16;
		pstverts[i].t = LittleLong (pinstverts[i].t) << 16;
	};

//
// set up the triangles
//
	ptri = (mtriangle_t *)&pstverts[pmodel->numverts];
	pintriangles = (dtriangle_t *)&pinstverts[pmodel->numverts];

	pheader->triangles = (byte *)ptri - (byte *)pheader;

	for (i=0 ; i<pmodel->numtris ; i++)
	{
		int		j;

		ptri[i].facesfront = LittleLong (pintriangles[i].facesfront);

		for (j=0 ; j<3 ; j++)
		{
			ptri[i].vertindex[j] =
					LittleLong (pintriangles[i].vertindex[j]);
		};
	};

//
// load the frames
//
	if (numframes < 1)
		mpSystem->Error ("Mod_LoadAliasModel: Invalid # of frames: %d\n", numframes);

	pframetype = (daliasframetype_t *)&pintriangles[pmodel->numtris];

	for (i=0 ; i<numframes ; i++)
	{
		aliasframetype_t	frametype;

		frametype = (aliasframetype_t)LittleLong (pframetype->type);
		pheader->frames[i].type = frametype;

		if (frametype == ALIAS_SINGLE)
		{
			pframetype = (daliasframetype_t *)
					Mod_LoadAliasFrame (pframetype + 1,
										&pheader->frames[i].frame,
										pmodel->numverts,
										&pheader->frames[i].bboxmin,
										&pheader->frames[i].bboxmax,
										pheader, pheader->frames[i].name);
		}
		else
		{
			pframetype = (daliasframetype_t *)
					Mod_LoadAliasGroup (pframetype + 1,
										&pheader->frames[i].frame,
										pmodel->numverts,
										&pheader->frames[i].bboxmin,
										&pheader->frames[i].bboxmax,
										pheader, pheader->frames[i].name);
		};
	};

	mod->type = mod_alias;

// FIXME: do this right
	mod->mins[0] = mod->mins[1] = mod->mins[2] = -16;
	mod->maxs[0] = mod->maxs[1] = mod->maxs[2] = 16;

//
// move the complete, relocatable alias model to the cache
//	
	end = Hunk_LowMark ();
	total = end - start;
	
	Cache_Alloc (&mod->cache, total, loadname);
	if (!mod->cache.data)
		return;
	Q_memcpy (mod->cache.data, pheader, total);

	Hunk_FreeToLowMark (start);
};

bool CModelLoaderAliasMDL::IsExtSupported(const char *asExt) const
{
	if(!Q_strcmp(asExt, "mdl"))
		return true;
	return false;
};

IModel *CModelLoaderAliasMDL::TryLoad(const char *asName)
{
	Mod_LoadAliasModel(TODO);
	return nullptr;
};