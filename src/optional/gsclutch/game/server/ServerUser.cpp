/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2023 BlackPhrase
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
/// @brief server code for moving users

//#include "quakedef.h"

edict_t *sv_player;

/*
====================
AddLinksToPmove

====================
*/
void AddLinksToPmove ( areanode_t *node )
{
	link_t		*l, *next;
	edict_t		*check;
	int			pl;
	int			i;
	physent_t	*pe;

	pl = EDICT_TO_PROG(sv_player);

	// touch linked edicts
	for (l = node->solid_edicts.next ; l != &node->solid_edicts ; l = next)
	{
		next = l->next;
		check = EDICT_FROM_AREA(l);

		if (check->v.owner == pl)
			continue;		// player's own missile
		if (check->v.solid == SOLID_BSP 
			|| check->v.solid == SOLID_BBOX 
			|| check->v.solid == SOLID_SLIDEBOX)
		{
			if (check == sv_player)
				continue;

			for (i=0 ; i<3 ; i++)
				if (check->v.absmin[i] > pmove_maxs[i]
				|| check->v.absmax[i] < pmove_mins[i])
					break;
			if (i != 3)
				continue;
			if (pmove->numphysent == MAX_PHYSENTS)
				return;
			pe = &pmove->physents[pmove->numphysent];
			pmove->numphysent++;

			VectorCopy (check->v.origin, pe->origin);
			pe->info = NUM_FOR_EDICT(check);
			if (check->v.solid == SOLID_BSP)
				pe->model = sv.models[(int)(check->v.modelindex)];
			else
			{
				pe->model = nullptr;
				VectorCopy (check->v.mins, pe->mins);
				VectorCopy (check->v.maxs, pe->maxs);
			};
		};
	};
	
// recurse down both sides
	if (node->axis == -1)
		return;

	if ( pmove_maxs[node->axis] > node->dist )
		AddLinksToPmove ( node->children[0] );
	if ( pmove_mins[node->axis] < node->dist )
		AddLinksToPmove ( node->children[1] );
};

/*
===========
SV_RunCmd
===========
*/
void SV_RunCmd (usercmd_t *ucmd)
{
	edict_t		*ent;
	int			i, n;
	int			oldmsec;

	cmd = *ucmd;

	// chop up very long commands
	if (cmd.msec > 50)
	{
		oldmsec = ucmd->msec;
		cmd.msec = oldmsec/2;
		SV_RunCmd (&cmd);
		cmd.msec = oldmsec/2;
		cmd.impulse = 0;
		SV_RunCmd (&cmd);
		return;
	};

	if (!sv_player->v.fixangle)
		VectorCopy (ucmd->viewangles, sv_player->v.v_angle);

	sv_player->v.button0 = ucmd->buttons & 1;
	sv_player->v.button2 = (ucmd->buttons & 2)>>1;
	
	if (ucmd->impulse)
		sv_player->v.impulse = ucmd->impulse;

//
// angles
// show 1/3 the pitch angle and all the roll angle	
	if (sv_player->v.health > 0)
	{
		if (!sv_player->v.fixangle)
		{
			sv_player->v.angles[PITCH] = -sv_player->v.v_angle[PITCH]/3;
			sv_player->v.angles[YAW] = sv_player->v.v_angle[YAW];
		};
		
		sv_player->v.angles[ROLL] = V_CalcRoll (sv_player->v.angles, sv_player->v.velocity)*4;
	};

	host_frametime = ucmd->msec * 0.001;
	if (host_frametime > 0.1)
		host_frametime = 0.1;

	//if (!host_client->spectator) // TODO
	{
		gGlobalVariables.frametime = host_frametime;

		gGlobalVariables.time = sv.time;
		gEntityInterface.pfnPlayerPreThink(sv_player);

		SV_RunThink (sv_player);
	};

	for (i=0 ; i<3 ; i++)
		pmove->origin[i] = sv_player->v.origin[i] + (sv_player->v.mins[i] - player_mins[i]);
	VectorCopy (sv_player->v.velocity, pmove->velocity);
	VectorCopy (sv_player->v.v_angle, pmove->angles);

	//pmove->spectator = host_client->spectator; // TODO
	pmove->waterjumptime = sv_player->v.teleport_time;
	pmove->numphysent = 1;
	pmove->physents[0].model = sv.worldmodel;
	pmove->cmd = *ucmd;
	pmove->dead = sv_player->v.health <= 0;
	//pmove->oldbuttons = host_client->oldbuttons; // TODO

	//movevars.entgravity = host_client->entgravity; // TODO
	//movevars.maxspeed = host_client->maxspeed; // TODO

	for (i=0 ; i<3 ; i++)
	{
		pmove_mins[i] = pmove->origin[i] - 256;
		pmove_maxs[i] = pmove->origin[i] + 256;
	}
#if 1
	AddLinksToPmove ( sv_areanodes );
#else
	AddAllEntsToPmove ();
#endif

#if 0
{
	int before, after;

before = PM_TestPlayerPosition (pmove->origin);
	gEntityInterface.pfnPM_Move(pmove, true);
after = PM_TestPlayerPosition (pmove->origin);

if (sv_player->v.health > 0 && before && !after )
	gpSystem->Printf ("player %s got stuck in playermove!!!!\n", host_client->name);
}
#else
	gEntityInterface.pfnPM_Move(pmove, true);
#endif

	//host_client->oldbuttons = pmove->oldbuttons; // TODO
	sv_player->v.teleport_time = pmove->waterjumptime;
	sv_player->v.waterlevel = pmove->waterlevel;
	sv_player->v.watertype = pmove->watertype;
	if (onground != -1)
	{
		sv_player->v.flags = (int)sv_player->v.flags | FL_ONGROUND;
		sv_player->v.groundentity = EDICT_TO_PROG(EDICT_NUM(pmove->physents[onground].info));
	}
	else
		sv_player->v.flags = (int)sv_player->v.flags & ~FL_ONGROUND;
	for (i=0 ; i<3 ; i++)
		sv_player->v.origin[i] = pmove->origin[i] - (sv_player->v.mins[i] - player_mins[i]);

#if 0
	// truncate velocity the same way the net protocol will
	for (i=0 ; i<3 ; i++)
		sv_player->v.velocity[i] = (int)pmove->velocity[i];
#else
	VectorCopy (pmove->velocity, sv_player->v.velocity);
#endif

	VectorCopy (pmove->angles, sv_player->v.v_angle);

	//if (!host_client->spectator) // TODO
	{
		// link into place and touch triggers
		SV_LinkEdict (sv_player, true);

		// touch other objects
		for (i=0 ; i<pmove->numtouch ; i++)
		{
			n = pmove->physents[pmove->touchindex[i]].info;
			ent = EDICT_NUM(n);
			if ((playertouch[n/8]&(1<<(n%8))))
				continue;
			gpEntityEventDispatcher->DispatchTouch(ent, sv_player);
			playertouch[n/8] |= 1 << (n%8);
		};
	};
};

/*
===============
SV_SetIdealPitch
===============
*/
#define MAX_FORWARD 6
void SV_SetIdealPitch()
{
	float angleval, sinval, cosval;
	trace_t tr;
	vec3_t top, bottom;
	float z[MAX_FORWARD];
	int i, j;
	int step, dir, steps;

	if(!((int)sv_player->v.flags & FL_ONGROUND))
		return;

	angleval = sv_player->v.angles[YAW] * M_PI * 2 / 360;
	sinval = sin(angleval);
	cosval = cos(angleval);

	for(i = 0; i < MAX_FORWARD; i++)
	{
		top[0] = sv_player->v.origin[0] + cosval * (i + 3) * 12;
		top[1] = sv_player->v.origin[1] + sinval * (i + 3) * 12;
		top[2] = sv_player->v.origin[2] + sv_player->v.view_ofs[2];

		bottom[0] = top[0];
		bottom[1] = top[1];
		bottom[2] = top[2] - 160;

		tr = SV_Move(top, vec3_origin, vec3_origin, bottom, 1, sv_player);
		if(tr.allsolid)
			return; // looking at a wall, leave ideal the way is was

		if(tr.fraction == 1)
			return; // near a dropoff

		z[i] = top[2] + tr.fraction * (bottom[2] - top[2]);
	};

	dir = 0;
	steps = 0;
	for(j = 1; j < i; j++)
	{
		step = z[j] - z[j - 1];
		if(step > -ON_EPSILON && step < ON_EPSILON)
			continue;

		if(dir && (step - dir > ON_EPSILON || step - dir < -ON_EPSILON))
			return; // mixed changes

		steps++;
		dir = step;
	};

	if(!dir)
	{
		sv_player->v.idealpitch = 0;
		return;
	};

	if(steps < 2)
		return;
	sv_player->v.idealpitch = -dir * sv_idealpitchscale.GetValue();
};

/*
==================
SV_UserFriction

==================
*/
void SV_UserFriction()
{
	float *vel;
	float speed, newspeed, control;
	vec3_t start, stop;
	float friction;
	trace_t trace;

	vel = velocity;

	speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);
	if(!speed)
		return;

	// if the leading edge is over a dropoff, increase friction
	start[0] = stop[0] = origin[0] + vel[0] / speed * 16;
	start[1] = stop[1] = origin[1] + vel[1] / speed * 16;
	start[2] = origin[2] + sv_player->v.mins[2];
	stop[2] = start[2] - 34;

	trace = SV_Move(start, vec3_origin, vec3_origin, stop, true, sv_player);

	if(trace.fraction == 1.0)
		friction = sv_friction.GetValue() * sv_edgefriction.GetValue();
	else
		friction = sv_friction.GetValue();

	// apply friction
	control = speed < sv_stopspeed.GetValue() ? sv_stopspeed.GetValue() : speed;
	newspeed = speed - host_frametime * control * friction;

	if(newspeed < 0)
		newspeed = 0;
	newspeed /= speed;

	vel[0] = vel[0] * newspeed;
	vel[1] = vel[1] * newspeed;
	vel[2] = vel[2] * newspeed;
};

void DropPunchAngle()
{
	float len;

	len = VectorNormalize(sv_player->v.punchangle);

	len -= 10 * host_frametime;
	if(len < 0)
		len = 0;
	VectorScale(sv_player->v.punchangle, len, sv_player->v.punchangle);
};

/*
===================
SV_WaterMove

===================
*/
void SV_WaterMove()
{
	int i;
	vec3_t wishvel;
	float speed, newspeed, wishspeed, addspeed, accelspeed;

	//
	// user intentions
	//
	AngleVectors(sv_player->v.v_angle, forward, right, up);

	for(i = 0; i < 3; i++)
		wishvel[i] = forward[i] * cmd.forwardmove + right[i] * cmd.sidemove;

	if(!cmd.forwardmove && !cmd.sidemove && !cmd.upmove)
		wishvel[2] -= 60; // drift towards bottom
	else
		wishvel[2] += cmd.upmove;

	wishspeed = Length(wishvel);
	if(wishspeed > sv_maxspeed.GetValue())
	{
		VectorScale(wishvel, sv_maxspeed.GetValue() / wishspeed, wishvel);
		wishspeed = sv_maxspeed.GetValue();
	};
	wishspeed *= 0.7;

	//
	// water friction
	//
	speed = Length(velocity);
	if(speed)
	{
		newspeed = speed - host_frametime * speed * sv_friction.GetValue();
		if(newspeed < 0)
			newspeed = 0;
		VectorScale(velocity, newspeed / speed, velocity);
	}
	else
		newspeed = 0;

	//
	// water acceleration
	//
	if(!wishspeed)
		return;

	addspeed = wishspeed - newspeed;
	if(addspeed <= 0)
		return;

	VectorNormalize(wishvel);
	accelspeed = sv_accelerate.GetValue() * wishspeed * host_frametime;
	if(accelspeed > addspeed)
		accelspeed = addspeed;

	for(i = 0; i < 3; i++)
		velocity[i] += accelspeed * wishvel[i];
};

void SV_WaterJump()
{
	if(sv.time > sv_player->v.teleport_time || !sv_player->v.waterlevel)
	{
		sv_player->v.flags = (int)sv_player->v.flags & ~FL_WATERJUMP;
		sv_player->v.teleport_time = 0;
	};
	sv_player->v.velocity[0] = sv_player->v.movedir[0];
	sv_player->v.velocity[1] = sv_player->v.movedir[1];
};

/*
===================
SV_AirMove

===================
*/
void SV_AirMove()
{
	int i;
	vec3_t wishvel;
	float fmove, smove;

	AngleVectors(sv_player->v.angles, forward, right, up);

	fmove = cmd.forwardmove;
	smove = cmd.sidemove;

	// hack to not let you back into teleporter
	if(sv.time < sv_player->v.teleport_time && fmove < 0)
		fmove = 0;

	for(i = 0; i < 3; i++)
		wishvel[i] = forward[i] * fmove + right[i] * smove;

	if((int)sv_player->v.movetype != MOVETYPE_WALK)
		wishvel[2] = cmd.upmove;
	else
		wishvel[2] = 0;

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	if(wishspeed > sv_maxspeed.GetValue())
	{
		VectorScale(wishvel, sv_maxspeed.GetValue() / wishspeed, wishvel);
		wishspeed = sv_maxspeed.GetValue();
	};

	if(sv_player->v.movetype == MOVETYPE_NOCLIP)
	{ // noclip
		VectorCopy(wishvel, velocity);
	}
	else if(onground)
	{
		SV_UserFriction();
		SV_Accelerate();
	}
	else
	{
		// not on ground, so little effect on velocity
		SV_AirAccelerate(wishvel);
	};
};

/*
===================
SV_ClientThink

the move fields specify an intended velocity in pix/sec
the angle fields specify an exact angular motion in degrees
===================
*/
void SV_ClientThink(edict_t *sv_player)
{
	vec3_t v_angle;

	if(sv_player->v.movetype == MOVETYPE_NONE)
		return;

	onground = (int)sv_player->v.flags & FL_ONGROUND;

	origin = sv_player->v.origin;
	velocity = sv_player->v.velocity;

	DropPunchAngle();

	//
	// if dead, behave differently
	//
	if(sv_player->v.health <= 0)
		return;

	//
	// angles
	// show 1/3 the pitch angle and all the roll angle
	//cmd = host_client->cmd; // TODO
	angles = sv_player->v.angles;

	VectorAdd(sv_player->v.v_angle, sv_player->v.punchangle, v_angle);
	angles[ROLL] = V_CalcRoll(sv_player->v.angles, sv_player->v.velocity) * 4;
	if(!sv_player->v.fixangle)
	{
		angles[PITCH] = -v_angle[PITCH] / 3;
		angles[YAW] = v_angle[YAW];
	};

	if((int)sv_player->v.flags & FL_WATERJUMP)
	{
		SV_WaterJump();
		return;
	};
	//
	// walk
	//
	if((sv_player->v.waterlevel >= 2) && (sv_player->v.movetype != MOVETYPE_NOCLIP))
	{
		SV_WaterMove();
		return;
	};

	SV_AirMove();
};