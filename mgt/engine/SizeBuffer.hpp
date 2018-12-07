/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018 BlackPhrase
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

#pragma once

#include "sizebuf.h"

void SZ_Alloc(sizebuf_t *buf, int startsize);
void SZ_Free(sizebuf_t *buf);

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