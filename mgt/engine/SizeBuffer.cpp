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
};

void SZ_Free(sizebuf_t *buf)
{
	Z_Free(buf->data);
	buf->data = nullptr;
	
	buf->maxsize = 0;
	buf->cursize = 0;
};

CSizeBuffer::CSizeBuffer(ISystem *apSystem, sizebuf_t *apBuffer) : mpSystem(apSystem), buf(apBuffer){}

CSizeBuffer::CSizeBuffer(ISystem *apSystem, int startsize) : mpSystem(apSystem)
{
	SZ_Alloc(buf, startsize);
};

CSizeBuffer::~CSizeBuffer()
{
	SZ_Free(buf);
};

void CSizeBuffer::Clear()
{
	buf->cursize = 0;
};

void *CSizeBuffer::GetSpace(int length)
{
	if(buf->cursize + length > buf->maxsize)
	{
		if(!buf->allowoverflow)
			mpSystem->Error("SZ_GetSpace: overflow without allowoverflow set");

		if(length > buf->maxsize)
			mpSystem->Error("SZ_GetSpace: %i is > full buffer size", length);

		buf->overflowed = true;
		mpSystem->Printf("SZ_GetSpace: overflow");
		Clear(buf);
	};

	void *data{buf->data + buf->cursize};
	buf->cursize += length;

	return data;
};

void CSizeBuffer::Write(const void *data, int length)
{
	Q_memcpy(GetSpace(buf, length), data, length);
};

void CSizeBuffer::Print(const char *data)
{
	int len{Q_strlen(data) + 1};

	if(buf->data[buf->cursize - 1])
		Q_memcpy((byte *)GetSpace(buf, len), data, len); // no trailing 0
	else
		Q_memcpy((byte *)GetSpace(buf, len - 1) - 1, data, len); // write over trailing 0
};