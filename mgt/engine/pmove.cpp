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
/// @brief player movement code

#include "quakedef.h"

movevars_t movevars{};

playermove_t *pmove{nullptr};

int onground;
int waterlevel;
int watertype;

float frametime;

vec3_t forward, right, up;

vec3_t player_mins = { -16, -16, -24 };
vec3_t player_maxs = { 16, 16, 32 };

// #define	PM_GRAVITY			800
// #define	PM_STOPSPEED		100
// #define	PM_MAXSPEED			320
// #define	PM_SPECTATORMAXSPEED	500
// #define	PM_ACCELERATE		10
// #define	PM_AIRACCELERATE	0.7
// #define	PM_WATERACCELERATE	10
// #define	PM_FRICTION			6
// #define	PM_WATERFRICTION	1

void PM_InitBoxHull();

void Pmove_Init()
{
	PM_InitBoxHull();
}

#define STEPSIZE 18

#define BUTTON_JUMP 2

#define STOP_EPSILON 0.1

/*
=============
PM_GroundMove

Player is on ground, with no upwards velocity
=============
*/
void PM_GroundMove(void)
{
	vec3_t start, dest;
	pmtrace_t trace;
	vec3_t original, originalvel, down, up, downvel;
	float downdist, updist;

	pmove->velocity[2] = 0;
	if(!pmove->velocity[0] && !pmove->velocity[1] && !pmove->velocity[2])
		return;

	// first try just moving to the destination
	dest[0] = pmove->origin[0] + pmove->velocity[0] * frametime;
	dest[1] = pmove->origin[1] + pmove->velocity[1] * frametime;
	dest[2] = pmove->origin[2];

	// first try moving directly to the next spot
	VectorCopy(dest, start);
	trace = PM_PlayerMove(pmove->origin, dest);
	if(trace.fraction == 1)
	{
		VectorCopy(trace.endpos, pmove->origin);
		return;
	}

	// try sliding forward both on ground and up 16 pixels
	// take the move that goes farthest
	VectorCopy(pmove->origin, original);
	VectorCopy(pmove->velocity, originalvel);

	// slide move
	PM_FlyMove();

	VectorCopy(pmove->origin, down);
	VectorCopy(pmove->velocity, downvel);

	VectorCopy(original, pmove->origin);
	VectorCopy(originalvel, pmove->velocity);

	// move up a stair height
	VectorCopy(pmove->origin, dest);
	dest[2] += STEPSIZE;
	trace = PM_PlayerMove(pmove->origin, dest);
	if(!trace.startsolid && !trace.allsolid)
	{
		VectorCopy(trace.endpos, pmove->origin);
	}

	// slide move
	PM_FlyMove();

	// press down the stepheight
	VectorCopy(pmove->origin, dest);
	dest[2] -= STEPSIZE;
	trace = PM_PlayerMove(pmove->origin, dest);
	if(trace.plane.normal[2] < 0.7)
		goto usedown;
	if(!trace.startsolid && !trace.allsolid)
	{
		VectorCopy(trace.endpos, pmove->origin);
	}
	VectorCopy(pmove->origin, up);

	// decide which one went farther
	downdist = (down[0] - original[0]) * (down[0] - original[0]) + (down[1] - original[1]) * (down[1] - original[1]);
	updist = (up[0] - original[0]) * (up[0] - original[0]) + (up[1] - original[1]) * (up[1] - original[1]);

	if(downdist > updist)
	{
	usedown:
		VectorCopy(down, pmove->origin);
		VectorCopy(downvel, pmove->velocity);
	}
	else // copy z value from slide move
		pmove->velocity[2] = downvel[2];

	// if at a dead stop, retry the move with nudges to get around lips
}


/*
==============
PM_Accelerate
==============
*/
void PM_Accelerate(vec3_t wishdir, float wishspeed, float accel)
{
	int i;
	float addspeed, accelspeed, currentspeed;

	if(pmove->dead)
		return;
	if(pmove->waterjumptime)
		return;

	currentspeed = DotProduct(pmove->velocity, wishdir);
	addspeed = wishspeed - currentspeed;
	if(addspeed <= 0)
		return;
	accelspeed = accel * frametime * wishspeed;
	if(accelspeed > addspeed)
		accelspeed = addspeed;

	for(i = 0; i < 3; i++)
		pmove->velocity[i] += accelspeed * wishdir[i];
}


/*
===================
PM_WaterMove

===================
*/
void PM_WaterMove(void)
{
	int i;
	vec3_t wishvel;
	float wishspeed;
	vec3_t wishdir;
	vec3_t start, dest;
	pmtrace_t trace;

	//
	// user intentions
	//
	for(i = 0; i < 3; i++)
		wishvel[i] = forward[i] * pmove->cmd.forwardmove + right[i] * pmove->cmd.sidemove;

	if(!pmove->cmd.forwardmove && !pmove->cmd.sidemove && !pmove->cmd.upmove)
		wishvel[2] -= 60; // drift towards bottom
	else
		wishvel[2] += pmove->cmd.upmove;

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	if(wishspeed > movevars.maxspeed)
	{
		VectorScale(wishvel, movevars.maxspeed / wishspeed, wishvel);
		wishspeed = movevars.maxspeed;
	}
	wishspeed *= 0.7;

	//
	// water acceleration
	//
	//	if (pmove->waterjumptime)
	//		Con_Printf ("wm->%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);
	PM_Accelerate(wishdir, wishspeed, movevars.wateraccelerate);

	// assume it is a stair or a slope, so press down from stepheight above
	VectorMA(pmove->origin, frametime, pmove->velocity, dest);
	VectorCopy(dest, start);
	start[2] += STEPSIZE + 1;
	trace = PM_PlayerMove(start, dest);
	if(!trace.startsolid && !trace.allsolid) // FIXME: check steep slope?
	{                                        // walked up the step
		VectorCopy(trace.endpos, pmove->origin);
		return;
	}

	PM_FlyMove();
	//	if (pmove->waterjumptime)
	//		Con_Printf ("<-wm%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);
}



/*
=============
JumpButton
=============
*/
void JumpButton(void)
{
	if(pmove->dead)
	{
		pmove->oldbuttons |= BUTTON_JUMP; // don't jump again until released
		return;
	}

	if(pmove->waterjumptime)
	{
		pmove->waterjumptime -= frametime;
		if(pmove->waterjumptime < 0)
			pmove->waterjumptime = 0;
		return;
	}

	if(waterlevel >= 2)
	{ // swimming, not jumping
		onground = -1;

		if(watertype == CONTENTS_WATER)
			pmove->velocity[2] = 100;
		else if(watertype == CONTENTS_SLIME)
			pmove->velocity[2] = 80;
		else
			pmove->velocity[2] = 50;
		return;
	}

	if(onground == -1)
		return; // in air, so no effect

	if(pmove->oldbuttons & BUTTON_JUMP)
		return; // don't pogo stick

	onground = -1;
	pmove->velocity[2] += 270;

	pmove->oldbuttons |= BUTTON_JUMP; // don't jump again until released
}




/*
=============
PlayerMove

Returns with origin, angles, and velocity modified in place.

Numtouch and touchindex[] will be set if any of the physents
were contacted during the move.
=============
*/
void PlayerMove(void)
{
	frametime = pmove->cmd.msec * 0.001;
	pmove->numtouch = 0;

	AngleVectors(pmove->angles, forward, right, up);

	if(pmove->spectator)
	{
		SpectatorMove();
		return;
	}

	NudgePosition();

	// take angles directly from command
	VectorCopy(pmove->cmd.viewangles, pmove->angles);

	// set onground, watertype, and waterlevel
	PM_CatagorizePosition();

	if(waterlevel == 2)
		CheckWaterJump();

	if(pmove->velocity[2] < 0)
		pmove->waterjumptime = 0;

	if(pmove->cmd.buttons & BUTTON_JUMP)
		JumpButton();
	else
		pmove->oldbuttons &= ~BUTTON_JUMP;

	PM_Friction();

	if(waterlevel >= 2)
		PM_WaterMove();
	else
		PM_AirMove();

	// set onground, watertype, and waterlevel for final spot
	PM_CatagorizePosition();
}