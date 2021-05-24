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
#include "ModelLoaderBSP.hpp"

/*
===============================================================================

					BRUSHMODEL LOADING

===============================================================================
*/

CModelLoaderBSP::CModelLoaderBSP(ISystem *apSystem) : mpSystem(apSystem){}

/*
=================
Mod_LoadTextures
=================
*/
void CModelLoaderBSP::LoadTextures (lump_t *l)
{
	int		i, j, pixels, num, max, altmax;
	miptex_t	*mt;
	texture_t	*tx, *tx2;
	texture_t	*anims[10];
	texture_t	*altanims[10];

	if (!l->filelen)
	{
		loadmodel->textures = nullptr;
		return;
	};
	auto m = (dmiptexlump_t *)(mod_base + l->fileofs);
	
	m->nummiptex = LittleLong (m->nummiptex);
	
	loadmodel->numtextures = m->nummiptex;
	loadmodel->textures = (texture_t**)mpMemory->GetHunk()->AllocName (m->nummiptex * sizeof(*loadmodel->textures) , loadname);

	for (i=0 ; i<m->nummiptex ; i++)
	{
		m->dataofs[i] = LittleLong(m->dataofs[i]);
		if (m->dataofs[i] == -1)
			continue;
		mt = (miptex_t *)((byte *)m + m->dataofs[i]);
		mt->width = LittleLong (mt->width);
		mt->height = LittleLong (mt->height);
		for (j=0 ; j<MIPLEVELS ; j++)
			mt->offsets[j] = LittleLong (mt->offsets[j]);
		
		if ( (mt->width & 15) || (mt->height & 15) )
			mpSystem->Error ("Texture %s is not 16 aligned", mt->name);
		pixels = mt->width*mt->height/64*85;
		tx = (texture_t*)mpMemory->GetHunk()->AllocName (sizeof(texture_t) +pixels, loadname );
		loadmodel->textures[i] = tx;

		Q_memcpy (tx->name, mt->name, sizeof(tx->name));
		tx->width = mt->width;
		tx->height = mt->height;
		for (j=0 ; j<MIPLEVELS ; j++)
			tx->offsets[j] = mt->offsets[j] + sizeof(texture_t) - sizeof(miptex_t);
		// the pixels immediately follow the structures
		Q_memcpy ( tx+1, mt+1, pixels);
		
		// TODO: client-side only?
		//if (!Q_strncmp(mt->name,"sky",3))
			//R_InitSky (tx);
	};

//
// sequence the animations
//
	for (i=0 ; i<m->nummiptex ; i++)
	{
		tx = loadmodel->textures[i];
		if (!tx || tx->name[0] != '+')
			continue;
		if (tx->anim_next)
			continue;	// allready sequenced

	// find the number of frames in the animation
		Q_memset (anims, 0, sizeof(anims));
		Q_memset (altanims, 0, sizeof(altanims));

		max = tx->name[1];
		altmax = 0;
		if (max >= 'a' && max <= 'z')
			max -= 'a' - 'A';
		if (max >= '0' && max <= '9')
		{
			max -= '0';
			altmax = 0;
			anims[max] = tx;
			max++;
		}
		else if (max >= 'A' && max <= 'J')
		{
			altmax = max - 'A';
			max = 0;
			altanims[altmax] = tx;
			altmax++;
		}
		else
			mpSystem->Error ("Bad animating texture %s", tx->name);

		for (j=i+1 ; j<m->nummiptex ; j++)
		{
			tx2 = loadmodel->textures[j];
			if (!tx2 || tx2->name[0] != '+')
				continue;
			if (Q_strcmp (tx2->name+2, tx->name+2))
				continue;

			num = tx2->name[1];
			if (num >= 'a' && num <= 'z')
				num -= 'a' - 'A';
			if (num >= '0' && num <= '9')
			{
				num -= '0';
				anims[num] = tx2;
				if (num+1 > max)
					max = num + 1;
			}
			else if (num >= 'A' && num <= 'J')
			{
				num = num - 'A';
				altanims[num] = tx2;
				if (num+1 > altmax)
					altmax = num+1;
			}
			else
				mpSystem->Error ("Bad animating texture %s", tx->name);
		};
		
#define	ANIM_CYCLE	2
	// link them all together
		for (j=0 ; j<max ; j++)
		{
			tx2 = anims[j];
			if (!tx2)
				mpSystem->Error ("Missing frame %i of %s",j, tx->name);
			tx2->anim_total = max * ANIM_CYCLE;
			tx2->anim_min = j * ANIM_CYCLE;
			tx2->anim_max = (j+1) * ANIM_CYCLE;
			tx2->anim_next = anims[ (j+1)%max ];
			if (altmax)
				tx2->alternate_anims = altanims[0];
		};
		for (j=0 ; j<altmax ; j++)
		{
			tx2 = altanims[j];
			if (!tx2)
				mpSystem->Error ("Missing frame %i of %s",j, tx->name);
			tx2->anim_total = altmax * ANIM_CYCLE;
			tx2->anim_min = j * ANIM_CYCLE;
			tx2->anim_max = (j+1) * ANIM_CYCLE;
			tx2->anim_next = altanims[ (j+1)%altmax ];
			if (max)
				tx2->alternate_anims = anims[0];
		};
	};
};

/*
=================
Mod_LoadLighting
=================
*/
void CModelLoaderBSP::LoadLighting (lump_t *l)
{
	if (!l->filelen)
	{
		loadmodel->lightdata = nullptr;
		return;
	};
	loadmodel->lightdata = (byte*)mpMemory->GetHunk()->AllocName ( l->filelen, loadname);	
	Q_memcpy (loadmodel->lightdata, mod_base + l->fileofs, l->filelen);
};

/*
=================
Mod_LoadVisibility
=================
*/
void CModelLoaderBSP::LoadVisibility (lump_t *l)
{
	if (!l->filelen)
	{
		loadmodel->visdata = nullptr;
		return;
	};
	loadmodel->visdata = (byte*)mpMemory->GetHunk()->AllocName ( l->filelen, loadname);	
	Q_memcpy (loadmodel->visdata, mod_base + l->fileofs, l->filelen);
};

/*
=================
Mod_LoadEntities
=================
*/
void CModelLoaderBSP::LoadEntities (lump_t *l)
{
	if (!l->filelen)
	{
		loadmodel->entities = nullptr;
		return;
	};
	loadmodel->entities = (char*)mpMemory->GetHunk()->AllocName ( l->filelen, loadname);	
	Q_memcpy (loadmodel->entities, mod_base + l->fileofs, l->filelen);
};

/*
=================
Mod_LoadVertexes
=================
*/
void CModelLoaderBSP::LoadVertexes (lump_t *l)
{
	int			i;

	auto in = (dvertex_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (mvertex_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->vertexes = out;
	loadmodel->numvertexes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		out->position[0] = LittleFloat (in->point[0]);
		out->position[1] = LittleFloat (in->point[1]);
		out->position[2] = LittleFloat (in->point[2]);
	};
};

/*
=================
Mod_LoadSubmodels
=================
*/
void CModelLoaderBSP::LoadSubmodels (lump_t *l)
{
	int			i, j;

	auto in = (dmodel_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (dmodel_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->submodels = out;
	loadmodel->numsubmodels = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{	// spread the mins / maxs by a pixel
			out->mins[j] = LittleFloat (in->mins[j]) - 1;
			out->maxs[j] = LittleFloat (in->maxs[j]) + 1;
			out->origin[j] = LittleFloat (in->origin[j]);
		};
		for (j=0 ; j<MAX_MAP_HULLS ; j++)
			out->headnode[j] = LittleLong (in->headnode[j]);
		out->visleafs = LittleLong (in->visleafs);
		out->firstface = LittleLong (in->firstface);
		out->numfaces = LittleLong (in->numfaces);
	};
};

/*
=================
Mod_LoadEdges
=================
*/
void CModelLoaderBSP::LoadEdges (lump_t *l)
{
	int 	i;

	auto in = (dedge_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (medge_t*)mpMemory->GetHunk()->AllocName ( (count + 1) * sizeof(*out), loadname);	

	loadmodel->edges = out;
	loadmodel->numedges = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		out->v[0] = (unsigned short)LittleShort(in->v[0]);
		out->v[1] = (unsigned short)LittleShort(in->v[1]);
	};
};

/*
=================
Mod_LoadTexinfo
=================
*/
void CModelLoaderBSP::LoadTexinfo (lump_t *l)
{
	int 	i, j;
	int		miptex;
	float	len1, len2;

	auto in = (texinfo_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (mtexinfo_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->texinfo = out;
	loadmodel->numtexinfo = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<8 ; j++)
			out->vecs[0][j] = LittleFloat (in->vecs[0][j]);
		len1 = Length (out->vecs[0]);
		len2 = Length (out->vecs[1]);
		len1 = (len1 + len2)/2;
		if (len1 < 0.32)
			out->mipadjust = 4;
		else if (len1 < 0.49)
			out->mipadjust = 3;
		else if (len1 < 0.99)
			out->mipadjust = 2;
		else
			out->mipadjust = 1;
#if 0
		if (len1 + len2 < 0.001)
			out->mipadjust = 1;		// don't crash
		else
			out->mipadjust = 1 / floor( (len1+len2)/2 + 0.1 );
#endif

		miptex = LittleLong (in->miptex);
		out->flags = LittleLong (in->flags);
	
		if (!loadmodel->textures)
		{
			out->texture = r_notexture_mip;	// checkerboard texture
			out->flags = 0;
		}
		else
		{
			if (miptex >= loadmodel->numtextures)
				mpSystem->Error ("miptex >= loadmodel->numtextures");
			out->texture = loadmodel->textures[miptex];
			if (!out->texture)
			{
				out->texture = r_notexture_mip; // texture not found
				out->flags = 0;
			};
		};
	};
};

/*
================
CalcSurfaceExtents

Fills in s->texturemins[] and s->extents[]
================
*/
void CModelLoaderBSP::CalcSurfaceExtents (msurface_t *s)
{
	float	mins[2], maxs[2], val;
	int		i,j, e;
	mvertex_t	*v;
	mtexinfo_t	*tex;
	int		bmins[2], bmaxs[2];

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -99999;

	tex = s->texinfo;
	
	for (i=0 ; i<s->numedges ; i++)
	{
		e = loadmodel->surfedges[s->firstedge+i];
		if (e >= 0)
			v = &loadmodel->vertexes[loadmodel->edges[e].v[0]];
		else
			v = &loadmodel->vertexes[loadmodel->edges[-e].v[1]];
		
		for (j=0 ; j<2 ; j++)
		{
			val = v->position[0] * tex->vecs[j][0] + 
				v->position[1] * tex->vecs[j][1] +
				v->position[2] * tex->vecs[j][2] +
				tex->vecs[j][3];
			if (val < mins[j])
				mins[j] = val;
			if (val > maxs[j])
				maxs[j] = val;
		};
	};

	for (i=0 ; i<2 ; i++)
	{	
		bmins[i] = floor(mins[i]/16);
		bmaxs[i] = ceil(maxs[i]/16);

		s->texturemins[i] = bmins[i] * 16;
		s->extents[i] = (bmaxs[i] - bmins[i]) * 16;
		if ( !(tex->flags & TEX_SPECIAL) && s->extents[i] > 256)
			mpSystem->Error ("Bad surface extents");
	};
};

/*
=================
Mod_LoadFaces
=================
*/
void CModelLoaderBSP::LoadFaces (lump_t *l)
{
	int			i, surfnum;
	int			planenum, side;

	auto in = (dface_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (msurface_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->surfaces = out;
	loadmodel->numsurfaces = count;

	for ( surfnum=0 ; surfnum<count ; surfnum++, in++, out++)
	{
		out->firstedge = LittleLong(in->firstedge);
		out->numedges = LittleShort(in->numedges);		
		out->flags = 0;

		planenum = LittleShort(in->planenum);
		side = LittleShort(in->side);
		if (side)
			out->flags |= SURF_PLANEBACK;			

		out->plane = loadmodel->planes + planenum;

		out->texinfo = loadmodel->texinfo + LittleShort (in->texinfo);

		CalcSurfaceExtents (out);
				
	// lighting info

		for (i=0 ; i<MAXLIGHTMAPS ; i++)
			out->styles[i] = in->styles[i];
		i = LittleLong(in->lightofs);
		if (i == -1)
			out->samples = nullptr;
		else
			out->samples = loadmodel->lightdata + i;
		
	// set the drawing flags flag
		
		if (!Q_strncmp(out->texinfo->texture->name,"sky",3))	// sky
		{
			out->flags |= (SURF_DRAWSKY | SURF_DRAWTILED);
			continue;
		};
		
		if (!Q_strncmp(out->texinfo->texture->name,"*",1))		// turbulent
		{
			out->flags |= (SURF_DRAWTURB | SURF_DRAWTILED);
			for (i=0 ; i<2 ; i++)
			{
				out->extents[i] = 16384;
				out->texturemins[i] = -8192;
			};
			continue;
		};
	};
};

/*
=================
Mod_SetParent
=================
*/
void Mod_SetParent (mnode_t *node, mnode_t *parent)
{
	node->parent = parent;
	if (node->contents < 0)
		return;
	Mod_SetParent (node->children[0], node);
	Mod_SetParent (node->children[1], node);
};

/*
=================
Mod_LoadNodes
=================
*/
void CModelLoaderBSP::LoadNodes (lump_t *l)
{
	int			i, j, p;

	auto in = (dnode_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (mnode_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->nodes = out;
	loadmodel->numnodes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->minmaxs[j] = LittleShort (in->mins[j]);
			out->minmaxs[3+j] = LittleShort (in->maxs[j]);
		};
	
		p = LittleLong(in->planenum);
		out->plane = loadmodel->planes + p;

		out->firstsurface = LittleShort (in->firstface);
		out->numsurfaces = LittleShort (in->numfaces);
		
		for (j=0 ; j<2 ; j++)
		{
			p = LittleShort (in->children[j]);
			if (p >= 0)
				out->children[j] = loadmodel->nodes + p;
			else
				out->children[j] = (mnode_t *)(loadmodel->leafs + (-1 - p));
		};
	};
	
	Mod_SetParent (loadmodel->nodes, nullptr);	// sets nodes and leafs
};

/*
=================
Mod_LoadLeafs
=================
*/
void CModelLoaderBSP::LoadLeafs (lump_t *l)
{
	int			i, j, p;

	auto in = (dleaf_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (mleaf_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->leafs = out;
	loadmodel->numleafs = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		for (j=0 ; j<3 ; j++)
		{
			out->minmaxs[j] = LittleShort (in->mins[j]);
			out->minmaxs[3+j] = LittleShort (in->maxs[j]);
		};

		p = LittleLong(in->contents);
		out->contents = p;

		out->firstmarksurface = loadmodel->marksurfaces +
			LittleShort(in->firstmarksurface);
		out->nummarksurfaces = LittleShort(in->nummarksurfaces);
		
		p = LittleLong(in->visofs);
		if (p == -1)
			out->compressed_vis = nullptr;
		else
			out->compressed_vis = loadmodel->visdata + p;
		out->efrags = nullptr;
		
		for (j=0 ; j<4 ; j++)
			out->ambient_sound_level[j] = in->ambient_level[j];
	};
};

/*
=================
Mod_LoadClipnodes
=================
*/
void CModelLoaderBSP::LoadClipnodes (lump_t *l)
{
	int			i;
	hull_t		*hull;

	auto in = (dclipnode_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (dclipnode_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->clipnodes = out;
	loadmodel->numclipnodes = count;

	hull = &loadmodel->hulls[1];
	hull->clipnodes = out;
	hull->firstclipnode = 0;
	hull->lastclipnode = count-1;
	hull->planes = loadmodel->planes;
	hull->clip_mins[0] = -16;
	hull->clip_mins[1] = -16;
	hull->clip_mins[2] = -24;
	hull->clip_maxs[0] = 16;
	hull->clip_maxs[1] = 16;
	hull->clip_maxs[2] = 32;

	hull = &loadmodel->hulls[2];
	hull->clipnodes = out;
	hull->firstclipnode = 0;
	hull->lastclipnode = count-1;
	hull->planes = loadmodel->planes;
	hull->clip_mins[0] = -32;
	hull->clip_mins[1] = -32;
	hull->clip_mins[2] = -24;
	hull->clip_maxs[0] = 32;
	hull->clip_maxs[1] = 32;
	hull->clip_maxs[2] = 64;

	for (i=0 ; i<count ; i++, out++, in++)
	{
		out->planenum = LittleLong(in->planenum);
		out->children[0] = LittleShort(in->children[0]);
		out->children[1] = LittleShort(in->children[1]);
	};
};

/*
=================
Mod_MakeHull0

Deplicate the drawing hull structure as a clipping hull
=================
*/
void CModelLoaderBSP::MakeHull0 ()
{
	mnode_t		*in, *child;
	dclipnode_t *out;
	int			i, j, count;
	hull_t		*hull;
	
	hull = &loadmodel->hulls[0];	
	
	in = loadmodel->nodes;
	count = loadmodel->numnodes;
	out = (dclipnode_t*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	hull->clipnodes = out;
	hull->firstclipnode = 0;
	hull->lastclipnode = count-1;
	hull->planes = loadmodel->planes;

	for (i=0 ; i<count ; i++, out++, in++)
	{
		out->planenum = in->plane - loadmodel->planes;
		for (j=0 ; j<2 ; j++)
		{
			child = in->children[j];
			if (child->contents < 0)
				out->children[j] = child->contents;
			else
				out->children[j] = child - loadmodel->nodes;
		};
	};
};

/*
=================
Mod_LoadMarksurfaces
=================
*/
void CModelLoaderBSP::LoadMarksurfaces (lump_t *l)
{	
	int		i, j;
	
	auto in = (short *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (msurface_t**)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->marksurfaces = out;
	loadmodel->nummarksurfaces = count;

	for ( i=0 ; i<count ; i++)
	{
		j = LittleShort(in[i]);
		if (j >= loadmodel->numsurfaces)
			mpSystem->Error ("Mod_ParseMarksurfaces: bad surface number");
		out[i] = loadmodel->surfaces + j;
	};
};

/*
=================
Mod_LoadSurfedges
=================
*/
void CModelLoaderBSP::LoadSurfedges (lump_t *l)
{	
	int		i;
	
	auto in = (int *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (int*)mpMemory->GetHunk()->AllocName ( count*sizeof(*out), loadname);	

	loadmodel->surfedges = out;
	loadmodel->numsurfedges = count;

	for ( i=0 ; i<count ; i++)
		out[i] = LittleLong (in[i]);
};

/*
=================
Mod_LoadPlanes
=================
*/
void CModelLoaderBSP::LoadPlanes (lump_t *l)
{
	int			i, j;
	int			bits;
	
	auto in = (dplane_t *)(mod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		mpSystem->Error ("MOD_LoadBmodel: funny lump size in %s",loadmodel->name);
	int count = l->filelen / sizeof(*in);
	auto out = (mplane_t*)mpMemory->GetHunk()->AllocName ( count*2*sizeof(*out), loadname);	
	
	loadmodel->planes = out;
	loadmodel->numplanes = count;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		bits = 0;
		for (j=0 ; j<3 ; j++)
		{
			out->normal[j] = LittleFloat (in->normal[j]);
			if (out->normal[j] < 0)
				bits |= 1<<j;
		};

		out->dist = LittleFloat (in->dist);
		out->type = LittleLong (in->type);
		out->signbits = bits;
	};
};

/*
=================
RadiusFromBounds
=================
*/
float RadiusFromBounds (vec3_t mins, vec3_t maxs)
{
	vec3_t	corner;

	for (int i=0 ; i<3 ; i++)
		corner[i] = fabs(mins[i]) > fabs(maxs[i]) ? fabs(mins[i]) : fabs(maxs[i]);

	return Length (corner);
};

/*
=================
Mod_LoadBrushModel
=================
*/
void CModelLoaderBSP::LoadBrushModel (model_t *mod, void *buffer)
{
	int			i, j;
	dmodel_t 	*bm;
	
	loadmodel = mod;
	
	loadmodel->type = mod_brush;
	
	auto header = (dheader_t *)buffer;

	i = LittleLong (header->version);
	if (i != BSPVERSION)
		mpSystem->Error ("Mod_LoadBrushModel: %s has wrong version number (%i should be %i)", mod->name, i, BSPVERSION);

// swap all the lumps
	mod_base = (byte *)header;

	for (i=0 ; i<sizeof(dheader_t)/4 ; i++)
		((int *)header)[i] = LittleLong ( ((int *)header)[i]);

// load into heap
	
	LoadVertexes (&header->lumps[LUMP_VERTEXES]);
	LoadEdges (&header->lumps[LUMP_EDGES]);
	LoadSurfedges (&header->lumps[LUMP_SURFEDGES]);
	LoadTextures (&header->lumps[LUMP_TEXTURES]);
	LoadLighting (&header->lumps[LUMP_LIGHTING]);
	LoadPlanes (&header->lumps[LUMP_PLANES]);
	LoadTexinfo (&header->lumps[LUMP_TEXINFO]);
	LoadFaces (&header->lumps[LUMP_FACES]);
	LoadMarksurfaces (&header->lumps[LUMP_MARKSURFACES]);
	LoadVisibility (&header->lumps[LUMP_VISIBILITY]);
	LoadLeafs (&header->lumps[LUMP_LEAFS]);
	LoadNodes (&header->lumps[LUMP_NODES]);
	LoadClipnodes (&header->lumps[LUMP_CLIPNODES]);
	LoadEntities (&header->lumps[LUMP_ENTITIES]);
	LoadSubmodels (&header->lumps[LUMP_MODELS]);

	MakeHull0 ();
	
	mod->numframes = 2;		// regular and alternate animation
	mod->flags = 0;
	
//
// set up the submodels (FIXME: this is confusing)
//
	for (i=0 ; i<mod->numsubmodels ; i++)
	{
		bm = &mod->submodels[i];

		mod->hulls[0].firstclipnode = bm->headnode[0];
		for (j=1 ; j<MAX_MAP_HULLS ; j++)
		{
			mod->hulls[j].firstclipnode = bm->headnode[j];
			mod->hulls[j].lastclipnode = mod->numclipnodes-1;
		};
		
		mod->firstmodelsurface = bm->firstface;
		mod->nummodelsurfaces = bm->numfaces;
		
		VectorCopy (bm->maxs, mod->maxs);
		VectorCopy (bm->mins, mod->mins);
		mod->radius = RadiusFromBounds (mod->mins, mod->maxs);
		
		mod->numleafs = bm->visleafs;

		if (i < mod->numsubmodels-1)
		{
			// duplicate the basic information
			char name[10];

			sprintf (name, "*%i", i+1);
			loadmodel = Mod_FindName (name);
			*loadmodel = *mod;
			Q_strcpy (loadmodel->name, name);
			mod = loadmodel;
		};
	};
};

bool CModelLoaderBSP::IsExtSupported(const char *asExt) const
{
	if(!Q_strcmp(asExt, "bsp"))
		return true;
	return false;
};

IModel *CModelLoaderBSP::TryLoad(const char *asName)
{
	LoadBrushModel(TODO);
	return nullptr;
};