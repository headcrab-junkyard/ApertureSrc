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

//#include "quakedef.h"

// TODO: temp
#include "EntityEventDispatcher.hpp"
extern CEntityEventDispatcher *gpEntityEventDispatcher;

/**

entities never clip against themselves, or their owner

line of sight checks trace->crosscontent, but bullets don't

*/

struct moveclip_t
{
	vec3_t boxmins, boxmaxs; // enclose the test object along entire move
	/*TODO const*/ float *mins, *maxs;      // size of the moving object
	vec3_t mins2, maxs2;     // size when clipping against mosnters
	float *start, *end;
	trace_t trace;
	int type;
	edict_t *passedict;
};

/*
===============================================================================

HULL BOXES

===============================================================================
*/

static mplane_t box_planes[6];

/*
===================
SV_HullForBox

To keep everything totally uniform, bounding boxes are turned into small
BSP trees instead of being compared directly.
===================
*/
hull_t *SV_HullForBox(vec3_t mins, vec3_t maxs)
{
	box_planes[0].dist = maxs[0];
	box_planes[1].dist = mins[0];
	box_planes[2].dist = maxs[1];
	box_planes[3].dist = mins[1];
	box_planes[4].dist = maxs[2];
	box_planes[5].dist = mins[2];

	return &box_hull;
};

/*
================
SV_HullForEntity

Returns a hull that can be used for testing or clipping an object of mins/maxs
size.
Offset is filled in to contain the adjustment that must be added to the
testing object's origin to get a point to use with the returned hull.
================
*/
hull_t *SV_HullForEntity(edict_t *ent, vec3_t mins, vec3_t maxs, vec3_t offset)
{
	model_t *model;
	vec3_t size;
	vec3_t hullmins, hullmaxs;
	hull_t *hull;

	// decide which clipping hull to use, based on the size
	if(ent->v.solid == SOLID_BSP)
	{ // explicit hulls in the BSP model
		if(ent->v.movetype != MOVETYPE_PUSH)
			gpSystem->Error("SOLID_BSP without MOVETYPE_PUSH");

		model = sv.models[(int)ent->v.modelindex];

		if(!model || model->type != mod_brush)
			gpSystem->Error("MOVETYPE_PUSH with a non bsp model");

		VectorSubtract(maxs, mins, size);
		if(size[0] < 3)
			hull = &model->hulls[0];
		else if(size[0] <= 32)
			hull = &model->hulls[1];
		else
			hull = &model->hulls[2];

		// calculate an offset value to center the origin
		VectorSubtract(hull->clip_mins, mins, offset);
		VectorAdd(offset, ent->v.origin, offset);
	}
	else
	{
		// create a temp hull from bounding box sizes
		VectorSubtract(ent->v.mins, maxs, hullmins);
		VectorSubtract(ent->v.maxs, mins, hullmaxs);
		hull = SV_HullForBox(hullmins, hullmaxs);

		VectorCopy(ent->v.origin, offset);
	};

	return hull;
};

/*
===============================================================================

ENTITY AREA CHECKING

===============================================================================
*/

/*
===============
SV_UnlinkEdict

===============
*/
void SV_UnlinkEdict(edict_t *ent)
{
	if(!ent->area.prev)
		return; // not linked in anywhere
	RemoveLink(&ent->area);
	ent->area.prev = ent->area.next = nullptr;
};

/*
====================
SV_TouchLinks
====================
*/
void SV_TouchLinks(edict_t *ent, areanode_t *node)
{
	link_t *l, *next;
	edict_t *touch;

	// touch linked edicts
	for(l = node->trigger_edicts.next; l != &node->trigger_edicts; l = next)
	{
		next = l->next;
		touch = EDICT_FROM_AREA(l);
		if(touch == ent)
			continue;
		if(/* !touch->v.touch || */ touch->v.solid != SOLID_TRIGGER) // TODO
			continue;
		if(ent->v.absmin[0] > touch->v.absmax[0] || ent->v.absmin[1] > touch->v.absmax[1] || ent->v.absmin[2] > touch->v.absmax[2] || ent->v.absmax[0] < touch->v.absmin[0] || ent->v.absmax[1] < touch->v.absmin[1] || ent->v.absmax[2] < touch->v.absmin[2])
			continue;

		gGlobalVariables.time = sv.time;
		gpEntityEventDispatcher->DispatchTouch(touch, ent);
	};

	// recurse down both sides
	if(node->axis == -1)
		return;

	if(ent->v.absmax[node->axis] > node->dist)
		SV_TouchLinks(ent, node->children[0]);
	if(ent->v.absmin[node->axis] < node->dist)
		SV_TouchLinks(ent, node->children[1]);
};

/*
===============
SV_FindTouchedLeafs

===============
*/
void SV_FindTouchedLeafs(edict_t *ent, mnode_t *node)
{
	mplane_t *splitplane;
	mleaf_t *leaf;
	int sides;
	int leafnum;

	if(node->contents == CONTENTS_SOLID)
		return;

	// add an efrag if the node is a leaf

	if(node->contents < 0)
	{
		if(ent->num_leafs == MAX_ENT_LEAFS)
			return;

		leaf = (mleaf_t *)node;
		leafnum = leaf - sv.worldmodel->leafs - 1;

		ent->leafnums[ent->num_leafs] = leafnum;
		ent->num_leafs++;
		return;
	};

	// NODE_MIXED

	splitplane = node->plane;
	sides = BOX_ON_PLANE_SIDE(ent->v.absmin, ent->v.absmax, splitplane);

	// recurse down the contacted sides
	if(sides & 1)
		SV_FindTouchedLeafs(ent, node->children[0]);

	if(sides & 2)
		SV_FindTouchedLeafs(ent, node->children[1]);
};

/*
===============
SV_LinkEdict

===============
*/
void SV_LinkEdict(edict_t *ent, bool touch_triggers)
{
	areanode_t *node;

	if(ent->area.prev)
		SV_UnlinkEdict(ent); // unlink from old position

	if(ent == sv.edicts)
		return; // don't add the world

	if(ent->free)
		return;

// set the abs box

#ifdef QUAKE2
	if(ent->v.solid == SOLID_BSP && (ent->v.angles[0] || ent->v.angles[1] || ent->v.angles[2]))
	{
		// expand for rotation
		float max, v;
		int i;

		max = 0;
		for(i = 0; i < 3; i++)
		{
			v = fabs(ent->v.mins[i]);
			if(v > max)
				max = v;
			v = fabs(ent->v.maxs[i]);
			if(v > max)
				max = v;
		};

		for(i = 0; i < 3; i++)
		{
			ent->v.absmin[i] = ent->v.origin[i] - max;
			ent->v.absmax[i] = ent->v.origin[i] + max;
		};
	}
	else
#endif
	{
		VectorAdd(ent->v.origin, ent->v.mins, ent->v.absmin);
		VectorAdd(ent->v.origin, ent->v.maxs, ent->v.absmax);
	};

	//
	// to make items easier to pick up and allow them to be grabbed off
	// of shelves, the abs sizes are expanded
	//
	if((int)ent->v.flags & FL_ITEM)
	{
		ent->v.absmin[0] -= 15;
		ent->v.absmin[1] -= 15;
		ent->v.absmax[0] += 15;
		ent->v.absmax[1] += 15;
	}
	else
	{
		// because movement is clipped an epsilon away from an actual edge,
		// we must fully check even when bounding boxes don't quite touch
		ent->v.absmin[0] -= 1;
		ent->v.absmin[1] -= 1;
		ent->v.absmin[2] -= 1;
		ent->v.absmax[0] += 1;
		ent->v.absmax[1] += 1;
		ent->v.absmax[2] += 1;
	};

	// link to PVS leafs
	ent->num_leafs = 0;
	if(ent->v.modelindex)
		SV_FindTouchedLeafs(ent, sv.worldmodel->nodes);

	if(ent->v.solid == SOLID_NOT)
		return;

	// find the first node that the ent's box crosses
	node = sv_areanodes;
	while(1)
	{
		if(node->axis == -1)
			break;
		if(ent->v.absmin[node->axis] > node->dist)
			node = node->children[0];
		else if(ent->v.absmax[node->axis] < node->dist)
			node = node->children[1];
		else
			break; // crosses the node
	};

	// link it in

	if(ent->v.solid == SOLID_TRIGGER)
		InsertLinkBefore(&ent->area, &node->trigger_edicts);
	else
		InsertLinkBefore(&ent->area, &node->solid_edicts);

	// if touch_triggers, touch all entities at this node and decend for more
	if(touch_triggers)
		SV_TouchLinks(ent, sv_areanodes);
};

//===========================================================================

/*
============
SV_TestEntityPosition

This could be a lot more efficient...
============
*/
edict_t *SV_TestEntityPosition(edict_t *ent)
{
	trace_t trace = SV_Move(ent->v.origin, ent->v.mins, ent->v.maxs, ent->v.origin, 0, ent);

	if(trace.startsolid)
		return sv.edicts;

	return nullptr;
};

/*
===============================================================================

LINE TESTING IN HULLS

===============================================================================
*/

/*
==================
SV_ClipMoveToEntity

Handles selection or creation of a clipping hull, and offseting (and
eventually rotation) of the end points
==================
*/
trace_t SV_ClipMoveToEntity(edict_t *ent, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end)
{
	trace_t trace;
	vec3_t offset;
	vec3_t start_l, end_l;
	hull_t *hull;

	// fill in a default trace
	memset(&trace, 0, sizeof(trace_t));
	trace.fraction = 1;
	trace.allsolid = true;
	VectorCopy(end, trace.endpos);

	// get the clipping hull
	hull = SV_HullForEntity(ent, mins, maxs, offset);

	VectorSubtract(start, offset, start_l);
	VectorSubtract(end, offset, end_l);

#ifdef QUAKE2
	// rotate start and end into the models frame of reference
	if(ent->v.solid == SOLID_BSP && (ent->v.angles[0] || ent->v.angles[1] || ent->v.angles[2]))
	{
		vec3_t a;
		vec3_t forward, right, up;
		vec3_t temp;

		AngleVectors(ent->v.angles, forward, right, up);

		VectorCopy(start_l, temp);
		start_l[0] = DotProduct(temp, forward);
		start_l[1] = -DotProduct(temp, right);
		start_l[2] = DotProduct(temp, up);

		VectorCopy(end_l, temp);
		end_l[0] = DotProduct(temp, forward);
		end_l[1] = -DotProduct(temp, right);
		end_l[2] = DotProduct(temp, up);
	};
#endif

	// trace a line through the apropriate clipping hull
	SV_RecursiveHullCheck(hull, hull->firstclipnode, 0, 1, start_l, end_l, &trace);

#ifdef QUAKE2
	// rotate endpos back to world frame of reference
	if(ent->v.solid == SOLID_BSP && (ent->v.angles[0] || ent->v.angles[1] || ent->v.angles[2]))
	{
		vec3_t a;
		vec3_t forward, right, up;
		vec3_t temp;

		if(trace.fraction != 1)
		{
			VectorSubtract(vec3_origin, ent->v.angles, a);
			AngleVectors(a, forward, right, up);

			VectorCopy(trace.endpos, temp);
			trace.endpos[0] = DotProduct(temp, forward);
			trace.endpos[1] = -DotProduct(temp, right);
			trace.endpos[2] = DotProduct(temp, up);

			VectorCopy(trace.plane.normal, temp);
			trace.plane.normal[0] = DotProduct(temp, forward);
			trace.plane.normal[1] = -DotProduct(temp, right);
			trace.plane.normal[2] = DotProduct(temp, up);
		};
	};
#endif

	// fix trace up by the offset
	if(trace.fraction != 1)
		VectorAdd(trace.endpos, offset, trace.endpos);

	// did we clip the move?
	if(trace.fraction < 1 || trace.startsolid)
		trace.ent = ent;

	return trace;
};

//===========================================================================

/*
==================
SV_MoveBounds
==================
*/
void SV_MoveBounds(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, vec3_t boxmins, vec3_t boxmaxs)
{
#if 0
	// debug to test against everything
	boxmins[0] = boxmins[1] = boxmins[2] = -9999;
	boxmaxs[0] = boxmaxs[1] = boxmaxs[2] = 9999;
#else
	int i;

	for(i = 0; i < 3; i++)
	{
		if(end[i] > start[i])
		{
			boxmins[i] = start[i] + mins[i] - 1;
			boxmaxs[i] = end[i] + maxs[i] + 1;
		}
		else
		{
			boxmins[i] = end[i] + mins[i] - 1;
			boxmaxs[i] = start[i] + maxs[i] + 1;
		};
	};
#endif
};

/*
====================
SV_ClipToLinks

Mins and maxs enclose the entire area swept by the move
====================
*/
void SV_ClipToLinks(areanode_t *node, moveclip_t *clip)
{
	link_t *l, *next;
	edict_t *touch;
	trace_t trace;

	// touch linked edicts
	for(l = node->solid_edicts.next; l != &node->solid_edicts; l = next)
	{
		next = l->next;
		touch = EDICT_FROM_AREA(l);
		if(touch->v.solid == SOLID_NOT)
			continue;
		if(touch == clip->passedict)
			continue;
		if(touch->v.solid == SOLID_TRIGGER)
			gpSystem->Error("Trigger in clipping list");

		if(clip->type == MOVE_NOMONSTERS && touch->v.solid != SOLID_BSP)
			continue;

		if(clip->boxmins[0] > touch->v.absmax[0] || clip->boxmins[1] > touch->v.absmax[1] || clip->boxmins[2] > touch->v.absmax[2] || clip->boxmaxs[0] < touch->v.absmin[0] || clip->boxmaxs[1] < touch->v.absmin[1] || clip->boxmaxs[2] < touch->v.absmin[2])
			continue;

		if(clip->passedict && clip->passedict->v.size[0] && !touch->v.size[0])
			continue; // points never interact

		// might intersect, so do an exact clip
		if(clip->trace.allsolid)
			return;
		if(clip->passedict)
		{
			if(PROG_TO_EDICT(touch->v.owner) == clip->passedict)
				continue; // don't clip against own missiles
			if(PROG_TO_EDICT(clip->passedict->v.owner) == touch)
				continue; // don't clip against owner
		};

		if((int)touch->v.flags & FL_MONSTER)
			trace = SV_ClipMoveToEntity(touch, clip->start, clip->mins2, clip->maxs2, clip->end);
		else
			trace = SV_ClipMoveToEntity(touch, clip->start, clip->mins, clip->maxs, clip->end);
		if(trace.allsolid || trace.startsolid ||
		   trace.fraction < clip->trace.fraction)
		{
			trace.ent = touch;
			if(clip->trace.startsolid)
			{
				clip->trace = trace;
				clip->trace.startsolid = true;
			}
			else
				clip->trace = trace;
		}
		else if(trace.startsolid)
			clip->trace.startsolid = true;
	};

	// recurse down both sides
	if(node->axis == -1)
		return;

	if(clip->boxmaxs[node->axis] > node->dist)
		SV_ClipToLinks(node->children[0], clip);
	if(clip->boxmins[node->axis] < node->dist)
		SV_ClipToLinks(node->children[1], clip);
};

/*
==================
SV_Move
==================
*/
trace_t SV_Move(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int type, edict_t *passedict)
{
	moveclip_t clip;
	int i;

	memset(&clip, 0, sizeof(moveclip_t));

	// clip to world
	clip.trace = SV_ClipMoveToEntity(sv.edicts, start, mins, maxs, end);

	clip.start = start;
	clip.end = end;
	clip.mins = mins;
	clip.maxs = maxs;
	clip.type = type;
	clip.passedict = passedict;

	if(type == MOVE_MISSILE)
	{
		for(i = 0; i < 3; i++)
		{
			clip.mins2[i] = -15;
			clip.maxs2[i] = 15;
		};
	}
	else
	{
		VectorCopy(mins, clip.mins2);
		VectorCopy(maxs, clip.maxs2);
	};

	// create the bounding box of the entire move
	SV_MoveBounds(start, clip.mins2, clip.maxs2, end, clip.boxmins, clip.boxmaxs);

	// clip to entities
	SV_ClipToLinks(sv_areanodes, &clip);

	return clip.trace;
};