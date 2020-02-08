/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2020 BlackPhrase
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

#pragma once

#include "CommonTypes.hpp"

typedef struct sizebuf_s
{
	//const char *name; // TODO
	
	//int flags; // TODO
	bool	allowoverflow;	// if false, do a Sys_Error
	bool	overflowed;		// set to true if the buffer size failed
	
	byte	*data;
	int		maxsize;
	int		cursize;
} sizebuf_t;

void SZ_Alloc(sizebuf_t *buf, int startsize);
void SZ_Free(sizebuf_t *buf);

void SZ_Clear(sizebuf_t *buf);
void *SZ_GetSpace(sizebuf_t *buf, int length);
void SZ_Write(sizebuf_t *buf, const void *data, int length);
void SZ_Print(sizebuf_t *buf, const char *data); // strcats onto the sizebuf

interface ISystem;

class CSizeBuffer final //: public ISizeBuffer
{
public:
	CSizeBuffer(ISystem *mpSystem, sizebuf_t *apBuffer);
	CSizeBuffer(ISystem *mpSystem, int startsize);
	~CSizeBuffer();
	
	void Clear();
	
	void *GetSpace(int length);
	
	void Write(const void *data, int length);
	
	/// strcats onto the sizebuf
	void Print(const char *data);
private:
	ISystem *mpSystem{nullptr};
	sizebuf_t *buf{nullptr};
};