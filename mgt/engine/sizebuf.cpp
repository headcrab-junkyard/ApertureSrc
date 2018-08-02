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

void SZ_Alloc(sizebuf_t *buf, int startsize)
{
	if(startsize < 256)
		startsize = 256;
	buf->data = (byte*)Hunk_AllocName(startsize, "sizebuf");
	buf->maxsize = startsize;
	buf->cursize = 0;
}

void SZ_Free(sizebuf_t *buf)
{
	//      Z_Free (buf->data);
	//      buf->data = nullptr;
	//      buf->maxsize = 0;
	buf->cursize = 0;
}

void SZ_Clear(sizebuf_t *buf)
{
	buf->cursize = 0;
}

void *SZ_GetSpace(sizebuf_t *buf, int length)
{
	void *data;

	if(buf->cursize + length > buf->maxsize)
	{
		if(!buf->allowoverflow)
			Sys_Error("SZ_GetSpace: overflow without allowoverflow set");

		if(length > buf->maxsize)
			Sys_Error("SZ_GetSpace: %i is > full buffer size", length);

		buf->overflowed = true;
		Con_Printf("SZ_GetSpace: overflow");
		SZ_Clear(buf);
	}

	data = buf->data + buf->cursize;
	buf->cursize += length;

	return data;
}

void SZ_Write(sizebuf_t *buf, const void *data, int length)
{
	Q_memcpy(SZ_GetSpace(buf, length), data, length);
}

void SZ_Print(sizebuf_t *buf, const char *data)
{
	int len = Q_strlen(data) + 1;

	if(buf->data[buf->cursize - 1])
		Q_memcpy((byte *)SZ_GetSpace(buf, len), data, len); // no trailing 0
	else
		Q_memcpy((byte *)SZ_GetSpace(buf, len - 1) - 1, data, len); // write over trailing 0
}