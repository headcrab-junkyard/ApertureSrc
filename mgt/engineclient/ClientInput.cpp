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

#include "quakedef.h"

//==========================================================================

/*
================
CL_BaseMove

Send the intended movement message to the server
================
*/
void CL_BaseMove(usercmd_t *cmd)
{
	if(cls.signon != SIGNONS)
		return;

	CL_AdjustAngles();

	Q_memset(cmd, 0, sizeof(*cmd));

	if(in_strafe.state & 1)
	{
		cmd->sidemove += cl_sidespeed.value * CL_KeyState(&in_right);
		cmd->sidemove -= cl_sidespeed.value * CL_KeyState(&in_left);
	};

	cmd->sidemove += cl_sidespeed.value * CL_KeyState(&in_moveright);
	cmd->sidemove -= cl_sidespeed.value * CL_KeyState(&in_moveleft);

	cmd->upmove += cl_upspeed.value * CL_KeyState(&in_up);
	cmd->upmove -= cl_upspeed.value * CL_KeyState(&in_down);

	if(!(in_klook.state & 1))
	{
		cmd->forwardmove += cl_forwardspeed.value * CL_KeyState(&in_forward);
		cmd->forwardmove -= cl_backspeed.value * CL_KeyState(&in_back);
	};

	//
	// adjust for speed key
	//
	if(in_speed.state & 1)
	{
		cmd->forwardmove *= cl_movespeedkey.value;
		cmd->sidemove *= cl_movespeedkey.value;
		cmd->upmove *= cl_movespeedkey.value;
	};

#ifdef QUAKE2
	cmd->lightlevel = cl.light_level;
#endif
}

/*
==============
CL_SendMove
==============
*/
void CL_SendMove(usercmd_t *cmd)
{
	int i;
	int checksumIndex;
	int bits;
	int lost;
	sizebuf_t buf;
	byte data[128];

	// if we are spectator, try autocam
	if(cl.spectator)
		Cam_Track(cmd);

	CL_FinishMove(cmd);

	Cam_FinishMove(cmd);

	// send this and the previous cmds in the message, so
	// if the last packet was dropped, it can be recovered
	buf.maxsize = 128;
	buf.cursize = 0;
	buf.data = data;

	//cl.cmd = *cmd;

	//
	// send the movement message
	//
	MSG_WriteByte(&buf, clc_move);

	// save the position for a checksum byte
	checksumIndex = buf.cursize;
	MSG_WriteByte(&buf, 0);

	// write our lossage percentage
	lost = CL_CalcNet();
	MSG_WriteByte(&buf, (byte)lost);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MSG_WriteFloat(&buf, cl.mtime[0]); // so server can get ping times

	for(i = 0; i < 3; i++)
		MSG_WriteAngle(&buf, cl.viewangles[i]);

	MSG_WriteShort(&buf, cmd->forwardmove);
	MSG_WriteShort(&buf, cmd->sidemove);
	MSG_WriteShort(&buf, cmd->upmove);

	//
	// send button bits
	//
	bits = 0;

	if(in_attack.state & 3)
		bits |= 1;
	in_attack.state &= ~2;

	if(in_jump.state & 3)
		bits |= 2;
	in_jump.state &= ~2;

	MSG_WriteByte(&buf, bits);

	MSG_WriteByte(&buf, in_impulse);
	in_impulse = 0;

#ifdef QUAKE2
	//
	// light level
	//
	MSG_WriteByte(&buf, cmd->lightlevel);
#endif

	//
	// always dump the first two message, because it may contain leftover inputs
	// from the last level
	//
	//if (++cl.movemessages <= 2)
		//return;

	//
	// deliver the message
	//
	cls.netchan->Transmit(buf.cursize, buf.data);
};