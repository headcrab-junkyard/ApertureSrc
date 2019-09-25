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
/// @brief world query functions

#include "quakedef.h"

int SV_HullPointContents(hull_t *hull, int num, vec3_t p);

/*
===============================================================================

HULL BOXES

===============================================================================
*/

static hull_t box_hull;
static dclipnode_t box_clipnodes[6];

/*
===================
SV_InitBoxHull

Set up the planes and clipnodes so that the six floats of a bounding box
can just be stored out and get a proper hull_t structure.
===================
*/
void SV_InitBoxHull()
{
	int i;
	int side;

	box_hull.clipnodes = box_clipnodes;
	box_hull.planes = box_planes;
	box_hull.firstclipnode = 0;
	box_hull.lastclipnode = 5;

	for(i = 0; i < 6; i++)
	{
		box_clipnodes[i].planenum = i;

		side = i & 1;

		box_clipnodes[i].children[side] = CONTENTS_EMPTY;
		if(i != 5)
			box_clipnodes[i].children[side ^ 1] = i + 1;
		else
			box_clipnodes[i].children[side ^ 1] = CONTENTS_SOLID;

		box_planes[i].type = i >> 1;
		box_planes[i].normal[i >> 1] = 1;
	};
};

/*
===============================================================================

ENTITY AREA CHECKING

===============================================================================
*/

areanode_t sv_areanodes[AREA_NODES];
static int sv_numareanodes;

/*
===============
SV_CreateAreaNode

===============
*/
areanode_t *SV_CreateAreaNode(int depth, vec3_t mins, vec3_t maxs)
{
	areanode_t *anode;
	vec3_t size;
	vec3_t mins1, maxs1, mins2, maxs2;

	anode = &sv_areanodes[sv_numareanodes];
	sv_numareanodes++;

	ClearLink(&anode->trigger_edicts);
	ClearLink(&anode->solid_edicts);

	if(depth == AREA_DEPTH)
	{
		anode->axis = -1;
		anode->children[0] = anode->children[1] = nullptr;
		return anode;
	};

	VectorSubtract(maxs, mins, size);
	if(size[0] > size[1])
		anode->axis = 0;
	else
		anode->axis = 1;

	anode->dist = 0.5 * (maxs[anode->axis] + mins[anode->axis]);
	VectorCopy(mins, mins1);
	VectorCopy(mins, mins2);
	VectorCopy(maxs, maxs1);
	VectorCopy(maxs, maxs2);

	maxs1[anode->axis] = mins2[anode->axis] = anode->dist;

	anode->children[0] = SV_CreateAreaNode(depth + 1, mins2, maxs2);
	anode->children[1] = SV_CreateAreaNode(depth + 1, mins1, maxs1);

	return anode;
};

/*
===============
SV_ClearWorld

===============
*/
void SV_ClearWorld()
{
	SV_InitBoxHull();

	memset(sv_areanodes, 0, sizeof(sv_areanodes));
	sv_numareanodes = 0;
	SV_CreateAreaNode(0, sv.worldmodel->mins, sv.worldmodel->maxs);
};

/*
===============================================================================

POINT TESTING IN HULLS

===============================================================================
*/

//#if !id386 // TODO: check for non-Intel?

/*
==================
SV_HullPointContents

==================
*/
int SV_HullPointContents(hull_t *hull, int num, vec3_t p)
{
	float d;
	dclipnode_t *node;
	mplane_t *plane;

	while(num >= 0)
	{
		if(num < hull->firstclipnode || num > hull->lastclipnode)
			gpSystem->Error("SV_HullPointContents: bad node number");

		node = hull->clipnodes + num;
		plane = hull->planes + node->planenum;

		if(plane->type < 3)
			d = p[plane->type] - plane->dist;
		else
			d = DotProduct(plane->normal, p) - plane->dist;
		if(d < 0)
			num = node->children[1];
		else
			num = node->children[0];
	};

	return num;
};

//#endif // !id386

/*
==================
SV_PointContents

==================
*/
int SV_PointContents(vec3_t p)
{
	int cont;

	cont = SV_HullPointContents(&sv.worldmodel->hulls[0], 0, p);
	if(cont <= CONTENTS_CURRENT_0 && cont >= CONTENTS_CURRENT_DOWN)
		cont = CONTENTS_WATER;
	return cont;
};

int SV_TruePointContents(vec3_t p)
{
	return SV_HullPointContents(&sv.worldmodel->hulls[0], 0, p);
};

/*
===============================================================================

LINE TESTING IN HULLS

===============================================================================
*/

// 1/32 epsilon to keep floating point happy
#define DIST_EPSILON (0.03125)

/*
==================
SV_RecursiveHullCheck

==================
*/
qboolean SV_RecursiveHullCheck(hull_t *hull, int num, float p1f, float p2f, vec3_t p1, vec3_t p2, trace_t *trace)
{
	dclipnode_t *node;
	mplane_t *plane;
	float t1, t2;
	float frac;
	int i;
	vec3_t mid;
	int side;
	float midf;

	// check for empty
	if(num < 0)
	{
		if(num != CONTENTS_SOLID)
		{
			trace->allsolid = false;
			if(num == CONTENTS_EMPTY)
				trace->inopen = true;
			else
				trace->inwater = true;
		}
		else
			trace->startsolid = true;
		return true; // empty
	};

	if(num < hull->firstclipnode || num > hull->lastclipnode)
		gpSystem->Error("SV_RecursiveHullCheck: bad node number");

	//
	// find the point distances
	//
	node = hull->clipnodes + num;
	plane = hull->planes + node->planenum;

	if(plane->type < 3)
	{
		t1 = p1[plane->type] - plane->dist;
		t2 = p2[plane->type] - plane->dist;
	}
	else
	{
		t1 = DotProduct(plane->normal, p1) - plane->dist;
		t2 = DotProduct(plane->normal, p2) - plane->dist;
	};

#if 1
	if(t1 >= 0 && t2 >= 0)
		return SV_RecursiveHullCheck(hull, node->children[0], p1f, p2f, p1, p2, trace);
	if(t1 < 0 && t2 < 0)
		return SV_RecursiveHullCheck(hull, node->children[1], p1f, p2f, p1, p2, trace);
#else
	if((t1 >= DIST_EPSILON && t2 >= DIST_EPSILON) || (t2 > t1 && t1 >= 0))
		return SV_RecursiveHullCheck(hull, node->children[0], p1f, p2f, p1, p2, trace);
	if((t1 <= -DIST_EPSILON && t2 <= -DIST_EPSILON) || (t2 < t1 && t1 <= 0))
		return SV_RecursiveHullCheck(hull, node->children[1], p1f, p2f, p1, p2, trace);
#endif

	// put the crosspoint DIST_EPSILON pixels on the near side
	if(t1 < 0)
		frac = (t1 + DIST_EPSILON) / (t1 - t2);
	else
		frac = (t1 - DIST_EPSILON) / (t1 - t2);
	if(frac < 0)
		frac = 0;
	if(frac > 1)
		frac = 1;

	midf = p1f + (p2f - p1f) * frac;
	for(i = 0; i < 3; i++)
		mid[i] = p1[i] + frac * (p2[i] - p1[i]);

	side = (t1 < 0);

	// move up to the node
	if(!SV_RecursiveHullCheck(hull, node->children[side], p1f, midf, p1, mid, trace))
		return false;

#ifdef PARANOID
	if(SV_HullPointContents(sv_hullmodel, mid, node->children[side]) == CONTENTS_SOLID)
	{
		gpSystem->Printf("mid PointInHullSolid\n");
		return false;
	};
#endif

	if(SV_HullPointContents(hull, node->children[side ^ 1], mid) != CONTENTS_SOLID)
		// go past the node
		return SV_RecursiveHullCheck(hull, node->children[side ^ 1], midf, p2f, mid, p2, trace);

	if(trace->allsolid)
		return false; // never got out of the solid area

	//==================
	// the other side of the node is solid, this is the impact point
	//==================
	if(!side)
	{
		VectorCopy(plane->normal, trace->plane.normal);
		trace->plane.dist = plane->dist;
	}
	else
	{
		VectorSubtract(vec3_origin, plane->normal, trace->plane.normal);
		trace->plane.dist = -plane->dist;
	};

	while(SV_HullPointContents(hull, hull->firstclipnode, mid) == CONTENTS_SOLID)
	{ // shouldn't really happen, but does occasionally
		frac -= 0.1;
		if(frac < 0)
		{
			trace->fraction = midf;
			VectorCopy(mid, trace->endpos);
			gpSystem->DevPrintf("backup past 0\n");
			return false;
		};
		midf = p1f + (p2f - p1f) * frac;
		for(i = 0; i < 3; i++)
			mid[i] = p1[i] + frac * (p2[i] - p1[i]);
	};

	trace->fraction = midf;
	VectorCopy(mid, trace->endpos);

	return false;
};