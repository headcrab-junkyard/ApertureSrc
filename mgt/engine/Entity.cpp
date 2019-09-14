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

#include "quakedef.h"
#include "Entity.hpp"

int SV_ModelIndex(const char *name);

CEntity::CEntity(edict_t *apEdict, IGameEntity *apGameEntity) : e(apEdict), mpGameEntity(apGameEntity)
{
	//if(mpGameEntity)
		//mpGameEntity->OnSpawn();
};

CEntity::~CEntity() = default;

void CEntity::Think()
{
	//if(mpGameEntity)
		//mpGameEntity->OnThink();
};

/*
=================
PF_setorigin

This is the only valid way to move an object without using the physics of the world (setting velocity and waiting).  Directly changing origin will not set internal links correctly, so clipping would be messed up.  This should be called when an object is spawned, and then only if it is teleported.

setorigin (entity, origin)
=================
*/
void CEntity::SetOrigin(const float *org)
{
	VectorCopy(org, e->v.origin);
	SV_LinkEdict(e, false);
};

/*
=================
PF_setsize

the size box is rotated by the current angle

setsize (entity, minvector, maxvector)
=================
*/
void CEntity::SetSize(const float *min, const float *max)
{
	SetMinMaxSize(min, max, false);
};

/*
=================
PF_setmodel

setmodel(entity, model)
=================
*/
void CEntity::SetModel(const char *m)
{
	char **check;
	model_t *mod;
	int i;

	// check to see if model was properly precached
	for(i = 0, check = sv.model_precache; *check; i++, check++)
		if(!Q_strcmp(*check, m))
			break;

	if(!*check)
		Host_Error("no precache: %s\n", m);

	e->v.model = m - pr_strings;
	e->v.modelindex = i; //SV_ModelIndex (m);

	mod = sv.models[(int)e->v.modelindex]; // Mod_ForName (m, true);

	if(mod)
		SetMinMaxSize(mod->mins, mod->maxs, true);
	else
		SetMinMaxSize(vec3_origin, vec3_origin, true);
};

/*
===============
PF_walkmove

float(float yaw, float dist) walkmove
===============
*/
int CEntity::WalkMove(float yaw, float dist, int nMode) // TODO: nMode support
{
	vec3_t move;

	if(!((int)e->v.flags & (FL_ONGROUND | FL_FLY | FL_SWIM)))
		return 0;

	yaw = yaw * M_PI * 2 / 360;

	move[0] = cos(yaw) * dist;
	move[1] = sin(yaw) * dist;
	move[2] = 0;

	return SV_movestep(e, move, true);
};

/*
==============
PF_changeyaw

This was a major timewaster in progs, so it was converted to C
==============
*/
// TODO: required by ServerMove code
void PF_changeyaw(edict_t *e)
{
	float ideal, current, move, speed;

	current = anglemod(e->v.angles[1]);
	ideal = e->v.ideal_yaw;
	speed = e->v.yaw_speed;

	if(current == ideal)
		return;
	
	move = ideal - current;
	
	if(ideal > current)
	{
		if(move >= 180)
			move = move - 360;
	}
	else
	{
		if(move <= -180)
			move = move + 360;
	};
	
	if(move > 0)
	{
		if(move > speed)
			move = speed;
	}
	else
	{
		if(move < -speed)
			move = -speed;
	};

	e->v.angles[1] = anglemod(current + move);
};

void CEntity::ChangeYaw()
{
	PF_changeyaw(e);
};

/*
==============
PF_changepitch
==============
*/
void CEntity::ChangePitch()
{
	float current = anglemod(e->v.angles[0]);
	float ideal = e->v.idealpitch;

	if(current == ideal)
		return;
	
	float move = ideal - current;
	
	if(ideal > current)
	{
		if(move >= 180)
			move = move - 360;
	}
	else
	{
		if(move <= -180)
			move = move + 360;
	};
	
	float speed = 0.0f; // TODO: e->v.pitch_speed;
	
	if(move > 0)
	{
		if(move > speed)
			move = speed;
	}
	else
	{
		if(move < -speed)
			move = -speed;
	};

	e->v.angles[0] = anglemod(current + move);
};

void CEntity::SetMinMaxSize(const float *min, const float *max, bool rotate)
{
	float *angles;
	vec3_t rmin, rmax;
	float bounds[2][3];
	float xvector[2], yvector[2];
	float a;
	vec3_t base, transformed;
	int i, j, k, l;

	for(i = 0; i < 3; i++)
		if(min[i] > max[i])
			Host_Error("backwards mins/maxs");

	rotate = false; // FIXME: implement rotation properly again

	if(!rotate)
	{
		VectorCopy(min, rmin);
		VectorCopy(max, rmax);
	}
	else
	{
		// find min / max for rotations
		angles = e->v.angles;

		a = angles[1] / 180 * M_PI;

		xvector[0] = cos(a);
		xvector[1] = sin(a);
		yvector[0] = -sin(a);
		yvector[1] = cos(a);

		VectorCopy(min, bounds[0]);
		VectorCopy(max, bounds[1]);

		rmin[0] = rmin[1] = rmin[2] = 9999;
		rmax[0] = rmax[1] = rmax[2] = -9999;

		for(i = 0; i <= 1; i++)
		{
			base[0] = bounds[i][0];
			for(j = 0; j <= 1; j++)
			{
				base[1] = bounds[j][1];
				for(k = 0; k <= 1; k++)
				{
					base[2] = bounds[k][2];

					// transform the point
					transformed[0] = xvector[0] * base[0] + yvector[0] * base[1];
					transformed[1] = xvector[1] * base[0] + yvector[1] * base[1];
					transformed[2] = base[2];

					for(l = 0; l < 3; l++)
					{
						if(transformed[l] < rmin[l])
							rmin[l] = transformed[l];
						if(transformed[l] > rmax[l])
							rmax[l] = transformed[l];
					};
				};
			};
		};
	};

	// set derived values
	VectorCopy(rmin, e->v.mins);
	VectorCopy(rmax, e->v.maxs);
	VectorSubtract(max, min, e->v.size);

	SV_LinkEdict(e, false);
};

// TODO
/*
void CEntity::MakeStatic(edict_t *ent)
{
	int i;

	MSG_WriteByte(&sv.signon, svc_spawnstatic);

	MSG_WriteByte(&sv.signon, SV_ModelIndex(pr_strings + ent->v.model));

	MSG_WriteByte(&sv.signon, ent->v.frame);
	MSG_WriteByte(&sv.signon, ent->v.colormap);
	MSG_WriteByte(&sv.signon, ent->v.skin);

	for(i = 0; i < 3; i++)
	{
		MSG_WriteCoord(&sv.signon, ent->v.origin[i]);
		MSG_WriteAngle(&sv.signon, ent->v.angles[i]);
	};

	// throw the entity away now
	ED_Free(ent);
};
*/

/*
=============
PF_checkbottom
=============
*/
float CEntity::CheckBottom()
{
	return SV_CheckBottom(e);
};

int CEntity::IsOnFloor() const
{
	return 0; // TODO
};

/*
===============
PF_droptofloor

void() droptofloor
===============
*/
int CEntity::DropToFloor(/* edict_t *e */)
{
	vec3_t end;
	trace_t trace;

	VectorCopy(e->v.origin, end);
	end[2] -= 256;

	trace = SV_Move(e->v.origin, e->v.mins, e->v.maxs, end, false, e);

	if(trace.fraction == 1 || trace.allsolid)
		return 0;
	else
	{
		VectorCopy(trace.endpos, e->v.origin);
		SV_LinkEdict(e, false);
		e->v.flags = (int)e->v.flags | FL_ONGROUND;
		e->v.groundentity = EDICT_TO_PROG(trace.ent);
		return 1;
	};
};