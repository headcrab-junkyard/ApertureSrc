/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief builds an intended movement command to send to the server

cvar_t m_pitch = { "m_pitch", "0.022", FCVAR_ARCHIVE };
cvar_t m_yaw = { "m_yaw", "0.022", FCVAR_ARCHIVE };
cvar_t m_forward = { "m_forward", "1", FCVAR_ARCHIVE };
cvar_t m_side = { "m_side", "0.8", FCVAR_ARCHIVE };

cvar_t lookspring = { "lookspring", "0", FCVAR_ARCHIVE };
cvar_t lookstrafe = { "lookstrafe", "0", FCVAR_ARCHIVE };

cvar_t cl_upspeed = { "cl_upspeed", "200" }; // TODO: FCVAR_ARCHIVE?
cvar_t cl_forwardspeed = { "cl_forwardspeed", "200", FCVAR_ARCHIVE };
cvar_t cl_backspeed = { "cl_backspeed", "200", FCVAR_ARCHIVE };
cvar_t cl_sidespeed = { "cl_sidespeed", "350" }; // TODO: FCVAR_ARCHIVE?

cvar_t cl_movespeedkey = { "cl_movespeedkey", "2.0" };

cvar_t cl_yawspeed = { "cl_yawspeed", "140" };
cvar_t cl_pitchspeed = { "cl_pitchspeed", "150" };

cvar_t cl_anglespeedkey = { "cl_anglespeedkey", "1.5" };

/*
===============================================================================

KEY BUTTONS

Continuous button event tracking is complicated by the fact that two different
input sources (say, mouse button 1 and the control key) can both press the
same button, but the button should only be released when both of the
pressing key have been released.

When a key event issues a button command (+forward, +attack, etc), it appends
its key number as a parameter to the command so it can be matched up with
the release.

state bit 0 is the current state of the key
state bit 1 is edge triggered on the up to down transition
state bit 2 is edge triggered on the down to up transition

===============================================================================
*/

kbutton_t in_mlook;
kbutton_t in_klook;
kbutton_t in_jlook;
kbutton_t in_left;
kbutton_t in_right;
kbutton_t in_forward;
kbutton_t in_back;
kbutton_t in_lookup;
kbutton_t in_lookdown;
kbutton_t in_moveleft;
kbutton_t in_moveright;
kbutton_t in_strafe;
kbutton_t in_speed;
kbutton_t in_use;
kbutton_t in_jump;
kbutton_t in_attack;
kbutton_t in_attack2;
kbutton_t in_up;
kbutton_t in_down;
kbutton_t in_duck;
kbutton_t in_reload;
kbutton_t in_alt1;
kbutton_t in_score;
kbutton_t in_break;
kbutton_t in_graph;

int in_impulse;

void KeyDown(kbutton_t *b)
{
	int k;
	const char *c;

	c = Cmd_Argv(1);
	if(c[0])
		k = atoi(c);
	else
		k = -1; // typed manually at the console for continuous down

	if(k == b->down[0] || k == b->down[1])
		return; // repeating key

	if(!b->down[0])
		b->down[0] = k;
	else if(!b->down[1])
		b->down[1] = k;
	else
	{
		Con_Printf("Three keys down for a button!\n");
		return;
	}

	if(b->state & 1)
		return;        // still down
	b->state |= 1 + 2; // down + impulse down
}

void KeyUp(kbutton_t *b)
{
	int k;
	const char *c;

	c = Cmd_Argv(1);
	if(c[0])
		k = atoi(c);
	else
	{ // typed manually at the console, assume for unsticking, so clear all
		b->down[0] = b->down[1] = 0;
		b->state = 4; // impulse up
		return;
	}

	if(b->down[0] == k)
		b->down[0] = 0;
	else if(b->down[1] == k)
		b->down[1] = 0;
	else
		return; // key up without coresponding down (menu pass through)
	if(b->down[0] || b->down[1])
		return; // some other key is still holding it down

	if(!(b->state & 1))
		return;     // still up (this should not happen)
	b->state &= ~1; // now up
	b->state |= 4;  // impulse up
}

void IN_KLookDown()
{
	KeyDown(&in_klook);
}
void IN_KLookUp()
{
	KeyUp(&in_klook);
}
void IN_MLookDown()
{
	KeyDown(&in_mlook);
}
void IN_MLookUp()
{
	KeyUp(&in_mlook);
	if(!(in_mlook.state & 1) && lookspring.value)
		V_StartPitchDrift();
}
void IN_UpDown()
{
	KeyDown(&in_up);
}
void IN_UpUp()
{
	KeyUp(&in_up);
}
void IN_DownDown()
{
	KeyDown(&in_down);
}
void IN_DownUp()
{
	KeyUp(&in_down);
}
void IN_LeftDown()
{
	KeyDown(&in_left);
}
void IN_LeftUp()
{
	KeyUp(&in_left);
}
void IN_RightDown()
{
	KeyDown(&in_right);
}
void IN_RightUp()
{
	KeyUp(&in_right);
}
void IN_ForwardDown()
{
	KeyDown(&in_forward);
}
void IN_ForwardUp()
{
	KeyUp(&in_forward);
}
void IN_BackDown()
{
	KeyDown(&in_back);
}
void IN_BackUp()
{
	KeyUp(&in_back);
}
void IN_LookupDown()
{
	KeyDown(&in_lookup);
}
void IN_LookupUp()
{
	KeyUp(&in_lookup);
}
void IN_LookdownDown()
{
	KeyDown(&in_lookdown);
}
void IN_LookdownUp()
{
	KeyUp(&in_lookdown);
}
void IN_MoveleftDown()
{
	KeyDown(&in_moveleft);
}
void IN_MoveleftUp()
{
	KeyUp(&in_moveleft);
}
void IN_MoverightDown()
{
	KeyDown(&in_moveright);
}
void IN_MoverightUp()
{
	KeyUp(&in_moveright);
}

void IN_SpeedDown()
{
	KeyDown(&in_speed);
}
void IN_SpeedUp()
{
	KeyUp(&in_speed);
}
void IN_StrafeDown()
{
	KeyDown(&in_strafe);
}
void IN_StrafeUp()
{
	KeyUp(&in_strafe);
}

void IN_AttackDown()
{
	KeyDown(&in_attack);
}
void IN_AttackUp()
{
	KeyUp(&in_attack);
}

void IN_Attack2Down()
{
	KeyDown(&in_attack);
}
void IN_Attack2Up()
{
	KeyUp(&in_attack);
}

void IN_UseDown()
{
	KeyDown(&in_use);
}
void IN_UseUp()
{
	KeyUp(&in_use);
}
void IN_JumpDown()
{
	KeyDown(&in_jump);
}
void IN_JumpUp()
{
	KeyUp(&in_jump);
}

void IN_Impulse()
{
	in_impulse = Q_atoi(Cmd_Argv(1));
}

/*
===============
CL_KeyState

Returns 0.25 if a key was pressed and released during the frame,
0.5 if it was pressed and held
0 if held then released, and
1.0 if held for the entire time
===============
*/
float CL_KeyState(kbutton_t *key)
{
	float val;
	qboolean impulsedown, impulseup, down;

	impulsedown = key->state & 2;
	impulseup = key->state & 4;
	down = key->state & 1;
	val = 0;

	if(impulsedown && !impulseup)
		if(down)
			val = 0.5; // pressed and held this frame
		else
			val = 0; //	I_Error ();
	if(impulseup && !impulsedown)
		if(down)
			val = 0; //	I_Error ();
		else
			val = 0; // released this frame
	if(!impulsedown && !impulseup)
		if(down)
			val = 1.0; // held the entire frame
		else
			val = 0; // up the entire frame
	if(impulsedown && impulseup)
		if(down)
			val = 0.75; // released and re-pressed this frame
		else
			val = 0.25; // pressed and released this frame

	key->state &= 1; // clear impulses

	return val;
}

/*
================
CL_AdjustAngles

Moves the local angle positions
================
*/
void CL_AdjustAngles()
{
	float speed;
	float up, down;

	if(in_speed.state & 1)
		speed = host_frametime * cl_anglespeedkey.value;
	else
		speed = host_frametime;

	if(!(in_strafe.state & 1))
	{
		cl.viewangles[YAW] -= speed * cl_yawspeed.value * CL_KeyState(&in_right);
		cl.viewangles[YAW] += speed * cl_yawspeed.value * CL_KeyState(&in_left);
		cl.viewangles[YAW] = anglemod(cl.viewangles[YAW]);
	}
	if(in_klook.state & 1)
	{
		V_StopPitchDrift();
		cl.viewangles[PITCH] -= speed * cl_pitchspeed.value * CL_KeyState(&in_forward);
		cl.viewangles[PITCH] += speed * cl_pitchspeed.value * CL_KeyState(&in_back);
	}

	up = CL_KeyState(&in_lookup);
	down = CL_KeyState(&in_lookdown);

	cl.viewangles[PITCH] -= speed * cl_pitchspeed.value * up;
	cl.viewangles[PITCH] += speed * cl_pitchspeed.value * down;

	if(up || down)
		V_StopPitchDrift();

	if(cl.viewangles[PITCH] > 80)
		cl.viewangles[PITCH] = 80;
	if(cl.viewangles[PITCH] < -70)
		cl.viewangles[PITCH] = -70;

	if(cl.viewangles[ROLL] > 50)
		cl.viewangles[ROLL] = 50;
	if(cl.viewangles[ROLL] < -50)
		cl.viewangles[ROLL] = -50;
}

/*
============
CL_InitInput
============
*/
void InitInput()
{
	gEngfuncs.Cmd_AddCommand("+moveup", IN_UpDown);
	gEngfuncs.Cmd_AddCommand("-moveup", IN_UpUp);
	gEngfuncs.Cmd_AddCommand("+movedown", IN_DownDown);
	gEngfuncs.Cmd_AddCommand("-movedown", IN_DownUp);
	gEngfuncs.Cmd_AddCommand("+left", IN_LeftDown);
	gEngfuncs.Cmd_AddCommand("-left", IN_LeftUp);
	gEngfuncs.Cmd_AddCommand("+right", IN_RightDown);
	gEngfuncs.Cmd_AddCommand("-right", IN_RightUp);
	gEngfuncs.Cmd_AddCommand("+forward", IN_ForwardDown);
	gEngfuncs.Cmd_AddCommand("-forward", IN_ForwardUp);
	gEngfuncs.Cmd_AddCommand("+back", IN_BackDown);
	gEngfuncs.Cmd_AddCommand("-back", IN_BackUp);
	gEngfuncs.Cmd_AddCommand("+lookup", IN_LookupDown);
	gEngfuncs.Cmd_AddCommand("-lookup", IN_LookupUp);
	gEngfuncs.Cmd_AddCommand("+lookdown", IN_LookdownDown);
	gEngfuncs.Cmd_AddCommand("-lookdown", IN_LookdownUp);
	gEngfuncs.Cmd_AddCommand("+strafe", IN_StrafeDown);
	gEngfuncs.Cmd_AddCommand("-strafe", IN_StrafeUp);
	gEngfuncs.Cmd_AddCommand("+moveleft", IN_MoveleftDown);
	gEngfuncs.Cmd_AddCommand("-moveleft", IN_MoveleftUp);
	gEngfuncs.Cmd_AddCommand("+moveright", IN_MoverightDown);
	gEngfuncs.Cmd_AddCommand("-moveright", IN_MoverightUp);
	gEngfuncs.Cmd_AddCommand("+speed", IN_SpeedDown);
	gEngfuncs.Cmd_AddCommand("-speed", IN_SpeedUp);
	gEngfuncs.Cmd_AddCommand("+attack", IN_AttackDown);
	gEngfuncs.Cmd_AddCommand("-attack", IN_AttackUp);
	gEngfuncs.Cmd_AddCommand("+attack2", IN_Attack2Down);
	gEngfuncs.Cmd_AddCommand("-attack2", IN_Attack2Up);
	gEngfuncs.Cmd_AddCommand("+use", IN_UseDown);
	gEngfuncs.Cmd_AddCommand("-use", IN_UseUp);
	gEngfuncs.Cmd_AddCommand("+jump", IN_JumpDown);
	gEngfuncs.Cmd_AddCommand("-jump", IN_JumpUp);
	gEngfuncs.Cmd_AddCommand("impulse", IN_Impulse);
	gEngfuncs.Cmd_AddCommand("+klook", IN_KLookDown);
	gEngfuncs.Cmd_AddCommand("-klook", IN_KLookUp);
	gEngfuncs.Cmd_AddCommand("+mlook", IN_MLookDown);
	gEngfuncs.Cmd_AddCommand("-mlook", IN_MLookUp);
	
	gEngfuncs.Cvar_RegisterVariable(&lookspring);
	gEngfuncs.Cvar_RegisterVariable(&lookstrafe);
	
	gEngfuncs.Cvar_RegisterVariable(&cl_upspeed);
	gEngfuncs.Cvar_RegisterVariable(&cl_forwardspeed);
	gEngfuncs.Cvar_RegisterVariable(&cl_backspeed);
	gEngfuncs.Cvar_RegisterVariable(&cl_sidespeed);
	
	gEngfuncs.Cvar_RegisterVariable(&cl_movespeedkey);
	gEngfuncs.Cvar_RegisterVariable(&cl_yawspeed);
	gEngfuncs.Cvar_RegisterVariable(&cl_pitchspeed);
	gEngfuncs.Cvar_RegisterVariable(&cl_anglespeedkey);
	
	gEngfuncs.Cvar_RegisterVariable(&m_pitch);
	gEngfuncs.Cvar_RegisterVariable(&m_yaw);
	gEngfuncs.Cvar_RegisterVariable(&m_forward);
	gEngfuncs.Cvar_RegisterVariable(&m_side);
	
	CAM_Init();
	IN_Init();
	KB_Init();
	V_Init();
}

/*
============
ShutdownInput
============
*/
void ShutdownInput()
{
	IN_Shutdown();
	KB_Shutdown();
};