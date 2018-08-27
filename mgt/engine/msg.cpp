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

#include "quakedef.h"

/*
==============================================================================

			MESSAGE IO FUNCTIONS

Handles byte ordering and avoids alignment errors
==============================================================================
*/

usercmd_t nullcmd{}; // guaranteed to be zero

//
// writing functions
//

void MSG_WriteChar(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < -128 || c > 127)
		Sys_Error("MSG_WriteChar: range error");
#endif

	buf = (byte*)SZ_GetSpace(sb, 1);
	buf[0] = c;
}

void MSG_WriteByte(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < 0 || c > 255)
		Sys_Error("MSG_WriteByte: range error");
#endif

	buf = (byte*)SZ_GetSpace(sb, 1);
	buf[0] = c;
}

void MSG_WriteShort(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < ((short)0x8000) || c > (short)0x7fff)
		Sys_Error("MSG_WriteShort: range error");
#endif

	buf = (byte*)SZ_GetSpace(sb, 2);
	buf[0] = c & 0xff;
	buf[1] = c >> 8;
}

void MSG_WriteLong(sizebuf_t *sb, int c)
{
	byte *buf;

	buf = (byte*)SZ_GetSpace(sb, 4);
	buf[0] = c & 0xff;
	buf[1] = (c >> 8) & 0xff;
	buf[2] = (c >> 16) & 0xff;
	buf[3] = c >> 24;
}

void MSG_WriteFloat(sizebuf_t *sb, float f)
{
	union
	{
		float f;
		int l;
	} dat;

	dat.f = f;
	dat.l = LittleLong(dat.l);

	SZ_Write(sb, &dat.l, 4);
}

void MSG_WriteString(sizebuf_t *sb, const char *s)
{
	if(!s)
		SZ_Write(sb, "", 1);
	else
		SZ_Write(sb, s, Q_strlen(s) + 1);
}

void MSG_WriteCoord(sizebuf_t *sb, float f)
{
	MSG_WriteShort(sb, (int)(f * 8));
}

void MSG_WriteAngle(sizebuf_t *sb, float f)
{
	MSG_WriteByte(sb, ((int)f * 256 / 360) & 255);
}

//
// reading functions
//
int msg_readcount;
qboolean msg_badread;

void MSG_BeginReading()
{
	msg_readcount = 0;
	msg_badread = false;
}

int MSG_GetReadCount()
{
	return msg_readcount;
}

// returns -1 and sets msg_badread if no more characters are available
int MSG_ReadChar()
{
	int c;

	if(msg_readcount + 1 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	}

	c = (signed char)net_message.data[msg_readcount];
	msg_readcount++;

	return c;
}

int MSG_ReadByte()
{
	int c;

	if(msg_readcount + 1 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	}

	c = (unsigned char)net_message.data[msg_readcount];
	msg_readcount++;

	return c;
}

int MSG_ReadShort()
{
	int c;

	if(msg_readcount + 2 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	}

	c = (short)(net_message.data[msg_readcount] + (net_message.data[msg_readcount + 1] << 8));

	msg_readcount += 2;

	return c;
}

int MSG_ReadLong()
{
	int c;

	if(msg_readcount + 4 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	}

	c = net_message.data[msg_readcount] + (net_message.data[msg_readcount + 1] << 8) + (net_message.data[msg_readcount + 2] << 16) + (net_message.data[msg_readcount + 3] << 24);

	msg_readcount += 4;

	return c;
}

float MSG_ReadFloat()
{
	union
	{
		byte b[4];
		float f;
		int l;
	} dat;

	dat.b[0] = net_message.data[msg_readcount];
	dat.b[1] = net_message.data[msg_readcount + 1];
	dat.b[2] = net_message.data[msg_readcount + 2];
	dat.b[3] = net_message.data[msg_readcount + 3];
	msg_readcount += 4;

	dat.l = LittleLong(dat.l);

	return dat.f;
}

char *MSG_ReadString()
{
	static char string[2048];
	int l, c;

	l = 0;
	do
	{
		c = MSG_ReadChar();
		if(c == -1 || c == 0)
			break;
		string[l] = c;
		l++;
	} while(l < sizeof(string) - 1);

	string[l] = 0;

	return string;
}

float MSG_ReadCoord()
{
	return MSG_ReadShort() * (1.0 / 8);
}

float MSG_ReadAngle()
{
	return MSG_ReadChar() * (360.0 / 256);
}

void MSG_ReadDeltaUsercmd(usercmd_t *from, usercmd_t *move)
{
// TODO
/*
	int bits;

	memcpy (move, from, sizeof(*move));

	bits = MSG_ReadByte ();
		
// read current angles
	if (bits & CM_ANGLE1)
		move->angles[0] = MSG_ReadAngle16 ();
	if (bits & CM_ANGLE2)
		move->angles[1] = MSG_ReadAngle16 ();
	if (bits & CM_ANGLE3)
		move->angles[2] = MSG_ReadAngle16 ();
		
// read movement
	if (bits & CM_FORWARD)
		move->forwardmove = MSG_ReadShort ();
	if (bits & CM_SIDE)
		move->sidemove = MSG_ReadShort ();
	if (bits & CM_UP)
		move->upmove = MSG_ReadShort ();
	
// read buttons
	if (bits & CM_BUTTONS)
		move->buttons = MSG_ReadByte ();

	if (bits & CM_IMPULSE)
		move->impulse = MSG_ReadByte ();

// read time to run command
	move->msec = MSG_ReadByte ();
*/
}